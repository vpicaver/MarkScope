#ifndef FRAMEPROFILER_H
#define FRAMEPROFILER_H

// Qt includes
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

//Our includes
#include "MarkScope/Profiler.h"
#include "MarkScope_global.h"

#ifdef ITT_AVAILABLE
#include "ittnotify.h"
#endif

#ifdef __APPLE__
#include <os/log.h>
#include <os/signpost.h>
#endif

namespace MarkScope {

class MARKSCOPE_EXPORT FrameProfiler : public QObject {
    Q_OBJECT

public:
    FrameProfiler(QQmlApplicationEngine* engine);

    ~FrameProfiler();

private:
    bool firstFrame = true;
#ifdef ITT_AVAILABLE
    __itt_domain* domain;
#endif
#ifdef __APPLE__
    os_log_t log;
#endif

    void profileFrame() {
        if(firstFrame) {
            Profiler::setThreadName("QtRenderer");
            startFrame();
            firstFrame = false;
        } else {
            endFrame();
            startFrame();
        }
    }

    inline void startFrame() {
#ifdef ITT_AVAILABLE
        __itt_string_handle* startHandle = __itt_string_handle_create(L"startFrame");
        __itt_task_begin(domain, __itt_null, __itt_null, startHandle);
#endif
#ifdef __APPLE__
        os_signpost_interval_begin(log, signpost_id, "frame");
#endif
    }

    inline void endFrame() {
#ifdef ITT_AVAILABLE
        __itt_task_end(domain);
#endif
#ifdef __APPLE__
        os_signpost_interval_end(log, signpost_id, "frame");
#endif
    }

#ifdef __APPLE__
    os_signpost_id_t signpost_id;
#endif
};

};

#endif // FRAMEPROFILER_H
