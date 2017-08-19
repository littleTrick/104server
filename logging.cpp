#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "logging.h"
#include "timestamp.h"

using namespace std;

static void defaultOutput(const char * data, size_t n) {
    printf("%.*s", static_cast<int>(n), data);
}

static void defaultFlush() {
}

typedef SimpleLogger::LogLevel LogLevel;

LogLevel  SimpleLogger::g_logLevel = TRACE;

FlushFunc  SimpleLogger::g_flushFunc  = defaultFlush;
OutputFunc SimpleLogger::g_outputFunc = defaultOutput;

SimpleLogger::SimpleLogger(
        LogLevel level, const char *file, const char *func, unsigned int line):
    level_(level),
    file_(file),
    func_(func),
    line_(line),
    buff_()
{
    buff_.reserve(128);
    std::string curr_time = Timestamp::current().to_string();
    buff_.append(curr_time).append(" ");
    buff_.append(level==TRACE ? " DEBUG " :
                    level==INFO  ? " INFO  " :
                    level==WARN  ? " WARN  " :
                    level==ERROR ? " ERROR " :
                                   " FATAL ");
}

SimpleLogger::~SimpleLogger() {
    char sline[10];
	const char *basename = strrchr(file_, '/');
	basename = (basename == NULL) ? file_ : basename + 1;
    snprintf(sline, sizeof sline, "%d", line_);
    buff_.append(" - ").append(basename);
    buff_.append(":").append(sline);
    buff_.append(" ").append(func_);
    buff_.append("\n");

    g_outputFunc(buff_.data(), static_cast<unsigned int>(buff_.size()));

    if (level_ == FATAL) {
        g_flushFunc();
        abort();
    }
}

void SimpleLogger::setOutput(OutputFunc func) {
    g_outputFunc = func;
}

void SimpleLogger::setFlush(FlushFunc flush) {
    g_flushFunc = flush;
}

LogLevel SimpleLogger::logLevel() {
    return g_logLevel;
}

void SimpleLogger::setLogLevel(LogLevel level) {
    g_logLevel = level;
}

SimpleLogger & SimpleLogger::operator <<(const char * s) {
    return *this << string(s);
}

SimpleLogger & SimpleLogger::operator <<(int n) {
	return *this << static_cast<long>(n);
}

SimpleLogger & SimpleLogger::operator <<(unsigned int n) {
    return *this << static_cast<unsigned long>(n);
}

SimpleLogger & SimpleLogger::operator <<(long n) {
    char sline[10];
    snprintf(sline, sizeof sline, "%ld", n);
    return *this << sline;
}

SimpleLogger & SimpleLogger::operator <<(unsigned long n) {
    char sline[20];
    snprintf(sline, sizeof sline, "%lu", n);
    return *this << sline;
}

SimpleLogger & SimpleLogger::operator <<(double d) {
    char sn[20];
    snprintf(sn, sizeof sn, "%f", d);
    return *this << sn;
}

SimpleLogger & SimpleLogger::operator<<(const string & msg) {
    buff_.append(msg);
    return *this;
}
