# MarkScope Profiler

A cross-platform performance profiler leveraging Intel's ITT and Apple's signposts for low-overhead, in-depth analysis.

## MarkScope Profiler Components

MarkScope Profiler provides two main components:

- **FrameProfiler** – Integrates with a QML application's `QQuickWindow` to profile frame rendering.
- **Profiler** – A general-purpose profiler for marking the start and end of arbitrary tasks, with an RAII helper (`ScopedProfiler`) for scoped profiling.

## Features

- **Cross-Platform:** Supports Intel ITT on platforms where it's available and Apple signposts on macOS.
- **Frame Profiling:** Automatically hooks into the QML engine to profile frame timings.
- **Task Profiling:** Easily mark the start and end of tasks or code blocks.
- **Scoped Profiling:** Use RAII with `ScopedProfiler` to automatically handle start/stop calls.
- **Thread Identification:** Set the current thread name for better traceability.

## Requirements

- **Qt Framework:** Qt 6.
- **Intel ITT Library:** Define `ITT_AVAILABLE` during compilation if you want to use Intel ITT profiling.
- **macOS:** Apple signposts are automatically enabled on macOS (`__APPLE__`).

## Installation

1. **Add the Headers:** Copy `FrameProfiler.h` and `Profiler.h` (along with `MarkScope_global.h`) into your project.
2. **Configure Compiler Flags:**
   - Define `ITT_AVAILABLE` if you have the Intel ITT libraries and want to enable ITT profiling.
   - On macOS, no extra configuration is needed for `os_signpost`; it's automatically enabled when compiling on Apple platforms.
3. **Link Qt Libraries:** Ensure your project links against the necessary Qt modules (e.g., `QtQuick`, `QtQml`).

## Usage

### FrameProfiler

The `FrameProfiler` class automatically connects to the QML engine's primary window. It hooks into the `frameSwapped` signal to start and end profiling intervals for each frame.

#### Example:

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MarkScope/FrameProfiler.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    
    //FrameProfiler isn't a child of engine so it can be allocated on the stack
    MarkScope::FrameProfiler frameProfiler(&engine);
    
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    
    return app.exec();
}
```

### Profiler

The `Profiler` class allows you to profile arbitrary tasks. You can manually start and stop profiling intervals or use `ScopedProfiler` for automatic scope-based profiling.

#### Manual Profiling:

```cpp
#include "MarkScope/Profiler.h"

void performTask() {
    MarkScope::Profiler profiler("MyDomain");
    profiler.start("LongTask");
    
    // ... perform the long task ...
    
    profiler.stop();
}
```

#### Scoped Profiling:

```cpp
#include "MarkScope/Profiler.h"

void anotherTask() {
    MarkScope::Profiler profiler("MyDomain");
    {
        MarkScope::ScopedProfiler scoped(profiler, "ScopedTask");
        // ... code block to be profiled ...
    } // Profiling automatically stops at the end of the scope.
}
```

### Setting Thread Name

For better identification in profiling tools, you can set the name of the current thread:

```cpp
#include "MarkScope/Profiler.h"

int main() {
    MarkScope::Profiler::setThreadName("MainThread");
    // ... rest of your code ...
}
```

## How It Works

The code uses conditional compilation to select the appropriate profiling backend:

- **Intel ITT:** When `ITT_AVAILABLE` is defined, the code uses ITT APIs (like `__itt_task_begin` and `__itt_task_end`) to mark profiling intervals.
- **Apple Signposts:** On macOS (when `__APPLE__` is defined), it uses Apple's `os_signpost` APIs (`os_signpost_interval_begin` and `os_signpost_interval_end`) for low-overhead performance marking.
- **FrameProfiler Hooks:** The `FrameProfiler` hooks into the QML window's `frameSwapped` signal. On the first frame, it sets the thread name (using `Profiler::setThreadName("QtRenderer")`) and then alternates between starting and ending the frame profiling interval.

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests if you have improvements or find bugs.

## License

MIT License
