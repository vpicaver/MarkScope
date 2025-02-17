#ifndef PROFILER_H
#define PROFILER_H

// Qt includes
#include <QString>
#include <QDebug>
#include <QThread>

#ifdef ITT_AVAILABLE
#include "ittnotify.h"
#endif

#ifdef __APPLE__
#include <os/log.h>
#include <os/signpost.h>
#endif

//Our includes
#include "MarkScope_global.h"

namespace MarkScope {

class MARKSCOPE_EXPORT Profiler
{
public:
    Profiler(const QString& domainName)
    {
#ifdef ITT_AVAILABLE
        domain = __itt_domain_create(domainName.toStdWString().c_str());
#endif
#ifdef __APPLE__
        log = os_log_create(domainName.toUtf8().constData(), OS_LOG_CATEGORY_POINTS_OF_INTEREST);
        signpost_id = os_signpost_id_generate(log);
#endif
    }

    //Takes in a string literal only
    inline void start(const QString& taskName)
    {
#ifdef ITT_AVAILABLE
        __itt_string_handle* startHandle = __itt_string_handle_create(taskName.toStdWString().c_str());
        __itt_task_begin(domain, __itt_null, __itt_null, startHandle);
#endif
#ifdef __APPLE__
        os_signpost_interval_begin(log, signpost_id, "Profile", "task: %s", taskName.toLocal8Bit().constData());
#endif
    }

    inline void stop()
    {
#ifdef ITT_AVAILABLE
        __itt_task_end(domain);
#endif
#ifdef __APPLE__
        os_signpost_interval_end(log, signpost_id, "Profile");
#endif
    }

    static void setThreadName(const QString& threadName) {
        QThread::currentThread()->setObjectName(threadName);
#ifdef ITT_AVAILABLE
        __itt_thread_set_name(threadName.toStdWString().c_str());
#endif
    }

private:
#ifdef ITT_AVAILABLE
    __itt_domain* domain;
#endif
#ifdef __APPLE__
    os_log_t log;
    os_signpost_id_t signpost_id;
#endif
};

class ScopedProfiler
{
public:
    // Constructor
    ScopedProfiler(Profiler& profiler, const QString& taskName)
        : m_profiler(profiler)
    {
        m_profiler.start(taskName);
    }

    // Destructor
    ~ScopedProfiler()
    {
        m_profiler.stop();
    }

private:
    Profiler& m_profiler; // Reference to Profiler object
};

};


#endif // PROFILER_H
