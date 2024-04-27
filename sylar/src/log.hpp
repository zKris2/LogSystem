#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>

#include "singleton.hpp"
#include "util.hpp"

#define SYLAR_LOG_LEVEL(logger, level)                                                                                       \
    if (logger->getLevel() <= level)                                                                                         \
    sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                 sylar::GetFiberId(), time(0))))                             \
        .getss()

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                                         \
    if (logger->getLevel() <= level)                                                                                         \
    sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                 sylar::GetFiberId(), time(0))))                             \
        .getEvent()                                                                                                          \
        ->format(fmt, __VA_ARGS__)

#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

namespace sylar
{

    class Logger;

    class LogLevel
    {
    public:
        enum Level
        {
            /// 未知级别
            UNKNOW = 0,
            /// DEBUG 级别
            DEBUG = 1,
            /// INFO 级别
            INFO = 2,
            /// WARN 级别
            WARN = 3,
            /// ERROR 级别
            ERROR = 4,
            /// FATAL 级别
            FATAL = 5
        };

        static const char *ToString(LogLevel::Level level);

        static LogLevel::Level FromString(const std::string &str);
    };

    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id, uint64_t time);

        ~LogEvent();

        const char *getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        std::string getContent() const { return m_ss.str(); }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }
        LogLevel::Level getLevel() const { return m_level; }

        std::stringstream &getss() { return m_ss; }

        void format(const char *fmt, ...);
        void format(const char *fmt, va_list al);

    private:
        /// 文件名
        const char *m_file = nullptr;
        /// 行号
        int32_t m_line = 0;
        /// 程序启动开始到现在的毫秒数
        uint32_t m_elapse = 0;
        /// 线程ID
        uint32_t m_threadId = 0;
        /// 协程ID
        uint32_t m_fiberId = 0;
        /// 时间戳
        uint64_t m_time = 0;
        /// 日志内容流
        std::stringstream m_ss;
        /// 日志器
        std::shared_ptr<Logger> m_logger;
        /// 日志等级
        LogLevel::Level m_level;
    };

    class LogEventWrap
    {
    public:
        LogEventWrap(LogEvent::ptr e);

        ~LogEventWrap();

        LogEvent::ptr getEvent() const { return m_event; }
        std::stringstream &getss();

    private:
        LogEvent::ptr m_event;
    };

    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        /**
         *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
         */
        LogFormatter(const std::string &pattern);

        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;

            virtual ~FormatItem() {}

            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        void init();

        bool isError() const { return m_error; }

        const std::string getPattern() const { return m_pattern; }

    private:
        /// 日志格式模板
        std::string m_pattern;
        /// 日志格式解析后格式
        std::vector<FormatItem::ptr> m_items;
        /// 是否有错误
        bool m_error = false;
    };

    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender() {}

        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val);
        LogFormatter::ptr getFormatter();

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

    protected:
        /// 日志级别
        LogLevel::Level m_level = LogLevel::DEBUG;
        /// 日志格式器
        LogFormatter::ptr m_formatter;
    };

    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string &name = "root");

        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);

        void info(LogEvent::ptr event);

        void warn(LogEvent::ptr event);

        void error(LogEvent::ptr event);

        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        void clearAppenders();

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

        const std::string &getName() const { return m_name; }

        void setFormatter(LogFormatter::ptr val);
        void setFormatter(const std::string &val);

        LogFormatter::ptr getFormatter();

    private:
        /// 日志名称
        std::string m_name;
        /// 日志级别
        LogLevel::Level m_level;
        /// 日志目标集合
        std::list<LogAppender::ptr> m_appenders;
        /// 日志格式器
        LogFormatter::ptr m_formatter;
    };

    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string &filename);
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen();

    private:
        /// 文件路径
        std::string m_filename;
        /// 文件流
        std::ofstream m_filestream;
        /// 上次重新打开时间
        uint64_t m_lastTime = 0;
    };

    /*******
     *
     * LoggerManager
     *
     ********/

    class LoggerManager
    {
    public:
        LoggerManager();

        Logger::ptr getLogger(const std::string &name);

        void init();

        Logger::ptr getRoot() const { return m_root; }

        std::string toYamlString();

    private:
        /// 日志器容器
        std::map<std::string, Logger::ptr> m_loggers;
        /// 主日志器
        Logger::ptr m_root;
    };

    /// 日志器管理类单例模式
    typedef sylar::Singleton<LoggerManager> LoggerMgr;
}


#endif