#ifndef FRAMEPROFILER_H
#define FRAMEPROFILER_H

// Qt includes
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

//Our includes
#include "Profiler.h"
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
    FrameProfiler(QQmlApplicationEngine* engine) :
        QObject(engine)
    {
#ifdef ITT_AVAILABLE
        domain = __itt_domain_create(L"FrameProfiler");
#endif
#ifdef __APPLE__
        log = os_log_create("com.markscope.frameprofiler", OS_LOG_CATEGORY_POINTS_OF_INTEREST);
        signpost_id = os_signpost_id_generate(log);
        Q_ASSERT(signpost_id != OS_SIGNPOST_ID_INVALID);
        Q_ASSERT(signpost_id != OS_SIGNPOST_ID_NULL);
#endif

        QObject *rootObject = nullptr;
        if (!engine->rootObjects().isEmpty()) {
            rootObject = engine->rootObjects().first();
        }

        QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);
        if (window) {
            connect(window, &QQuickWindow::frameSwapped, this, &FrameProfiler::profileFrame, Qt::DirectConnection);
        } else {
            qWarning("Failed to get QQuickWindow, frame profiling will not work");
        }
    }

    ~FrameProfiler() {
#ifdef ITT_AVAILABLE
        // __itt_domain_destroy(domain); // Uncomment if necessary
#endif
    }

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
