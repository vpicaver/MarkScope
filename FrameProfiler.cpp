#include "MarkScope/FrameProfiler.h"

MarkScope::FrameProfiler::FrameProfiler(QQmlApplicationEngine *engine)
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
        auto objects = engine->rootObjects();
        rootObject = objects.first();
    }

    QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);
    if (window) {
        connect(window, &QQuickWindow::frameSwapped, this, &FrameProfiler::profileFrame, Qt::DirectConnection);
    } else {
        qWarning("Failed to get QQuickWindow, frame profiling will not work");
    }
}

MarkScope::FrameProfiler::~FrameProfiler() {
#ifdef ITT_AVAILABLE
    // __itt_domain_destroy(domain); // Uncomment if necessary
#endif
}
