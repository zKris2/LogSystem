#include<iostream>

#include "log.cpp"
#include "util/util.cpp"

int main(){
    sylar::Logger::ptr logger(new sylar::Logger);

    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    // sylar::LogEvent::ptr event(new sylar::LogEvent(logger,sylar::LogLevel::DEBUG,__FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time(0)));

    // logger->log(sylar::LogLevel::DEBUG,event);


    SYLAR_LOG_INFO(logger) << "test micro";
    SYLAR_LOG_ERROR(logger) << "test micro error";

    return 0;
}