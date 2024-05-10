#include<iostream>

#include "log.hpp"
#include "util.hpp"

int main(){
    sylar::Logger::ptr logger(new sylar::Logger);

    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    // sylar::LogEvent::ptr event(new sylar::LogEvent(logger,sylar::LogLevel::DEBUG,__FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time(0)));

    // logger->log(sylar::LogLevel::DEBUG,event);


    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("log/log.txt"));
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%p%T%m%n"));
    file_appender->setFormatter(fmt);

    file_appender->setLevel(sylar::LogLevel::ERROR);

    logger->addAppender(file_appender);

    SYLAR_LOG_INFO(logger) << "test micro";
    SYLAR_LOG_ERROR(logger) << "test micro error";

    SYLAR_LOG_FMT_DEBUG(logger,"test micro error %s"," 测试fmt") ;

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l)<<"xxx";
    

    return 0;
}