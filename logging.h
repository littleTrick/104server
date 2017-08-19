#ifndef LOGGING_H
#define LOGGING_H

#include <string>

typedef void (*OutputFunc)(const char *, size_t);
typedef void (*FlushFunc)();

class SimpleLogger
{
public:
    typedef SimpleLogger Self;
    enum LogLevel
    {
        TRACE,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    SimpleLogger(LogLevel level, const char *file, const char *func, unsigned int line);
    ~SimpleLogger();

    static void setOutput(OutputFunc func);
    static void setFlush(FlushFunc flush);

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

	Self & operator << (int);
    Self & operator << (unsigned int);
    Self & operator << (long);
    Self & operator << (unsigned long);
    Self & operator << (double);
    Self & operator << (const char *);
    Self & operator << (const std::string &);

private:
    static LogLevel     g_logLevel;
    static FlushFunc    g_flushFunc;
    static OutputFunc   g_outputFunc;

    LogLevel    level_;
    const char  *file_;
    const char  *func_;
    unsigned int line_;

    std::string buff_;
};

#define TRACE   SimpleLogger::TRACE
#define INFO    SimpleLogger::INFO
#define WARN    SimpleLogger::WARN
#define ERROR   SimpleLogger::ERROR
#define FATAL   SimpleLogger::FATAL

#define LOG(level)  if (level >= SimpleLogger::logLevel()) \
    SimpleLogger(level, __FILE__, "", __LINE__)

#define setLogLevel(level) SimpleLogger::setLogLevel(level)

#endif 
