#ifndef QICRUNTIME_H
#define QICRUNTIME_H

#include <QString>

#if defined QIC_DLL
#define QIC_EXPORT Q_DECL_EXPORT
#else
#define QIC_EXPORT Q_DECL_IMPORT
#endif

class qicRuntimePrivate;

/**
    \class qicRuntime
    The qicRuntime class provides the runtime build and execution environment.
    It utilizes the locally installed Qt build system `qmake` and native build
    toolchain.

    The exec() method takes a piece of self-contained C++ source code, wraps it
    in a shared library `qmake` project and builds it using the installed C++
    toolchain and Qt SDK. The source code must define and export the
    qic_entry() function. Upon successful compilation of this code into a
    shared library, this library is loaded, and the qic_entry() function is
    resolved and called.

    Use the setCtxVar() and getCtxVar() methods to exchange data with the
    runtime-compiled code.

    Use the various setters to control the build environment. You can override
    environment variables, path to the `qmake` and `make` programs, add defines,
    include paths and linked libraries. By default, the compiled library does
    not link with Qt. You can override and link with Qt libraries using
    setQtLibs(). Use setQtConfig() to configure additional build options.

    \fn qicRuntime::qicRuntime()
    Constructs a default build and runtime environment. Environment variables
    are inherited from the parent process. The `qmake` and `make` (or `nmake`
    on Windows) utilities are expected to be on PATH.

    \fn qicRuntime::~qicRuntime()
    Destroys the runtime environment. Destroys all objects registered by the
    runtime-compiled code via qicContext::set() and unloads all libraries in
    reverse order.

    \fn qicRuntime::setEnv()
    Sets an environment variable for the build process.

    \fn qicRuntime::addEnv()
    Appends an environment variable, using the system's native path delimiter.
    Useful for appending the PATH env variable.

    \fn qicRuntime::loadEnv()
    Sets one or more environment variables loaded from a file. Useful for
    configuring a complete build environment.

    \fn qicRuntime::setQmake()
    Sets the path to the `qmake` utility. Useful when multiple installations
    of the Qt SDK are present.

    \fn qicRuntime::setMake()
    Sets the path to the `make` utility, or `nmake` on Windows.

    \fn qicRuntime::setDefines()
    Sets the content of the **DEFINES** `qmake` variable.

    \fn qicRuntime::setIncludePath()
    Sets the content of the **INCLUDEPATH** `qmake` variable.

    \fn qicRuntime::setLibs()
    Sets the content of the **LIBS** `qmake` variable.

    \fn qicRuntime::setQtLibs()
    Sets the content of the **QT** `qmake` variable. This controls which Qt
    libraries will be linked with the binary. By default Qt is not linked.

    \fn qicRuntime::setQtConfig()
    Sets the content of the **CONFIG** `qmake` variable. This is used to control
    build options such as debug/release, rtti, exceptions, etc. If the host
    application is compiled using `CONFIG=debug`, make sure, the runtime code
    is also compiled with the same option. Otherwise the host application and
    the runtime code will be linked with different Qt libraries and different
    versions of the CRT runtime which will cause unpredictable fatal errors.

    \fn qicRuntime::getCtxVar()
    Returns a pointer to an object set either by the runtime-compiled code,
    via qicContext::set() or previously set by setCtxVar().

    \fn qicRuntime::setCtxVar()
    Registers a pointer to an object with the runtime. This variable will be
    available to the runtime-compiled code via qicContext::get(). If a \a
    deleter is provided, the object will be disposed of when the qicRuntime is
    destroyed.

    \fn qicRuntime::exec()
    Compiles and executes the provided C++ code. This method is blocking and
    returns only after the build process completes and the qic_entry() function
    returns.

    \fn qicRuntime::execFile()
    Same as exec() except the source code is loaded from the \a filename;
 */
class QIC_EXPORT qicRuntime
{
public:
    qicRuntime();
    ~qicRuntime();

    //TODO: make exec() async, return a quasi future object qicExecutable

    bool exec(QString source);
    bool execFile(QString filename);

    // build env

    void setEnv(QString name, QString value);
    void addEnv(QString name, QString value);
    void loadEnv(QString path);
    void setQmake(QString path);
    void setMake(QString path);
    void setDefines(QStringList defines);
    void setIncludePath(QStringList dirs);
    void setLibs(QStringList libs);
    void setQtLibs(QStringList qtlibs);
    void setQtConfig(QStringList qtconf);

    // ctx

    void *getCtxVar(const char *name);
    void *setCtxVar(void *ptr, const char *name, void(*deleter)(void*));

private:
    bool compile(QString src);

private:
    qicRuntimePrivate *p;
};

#endif // QICRUNTIME_H
