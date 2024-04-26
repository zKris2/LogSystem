#include "../config.cpp"
#include "../log.hpp"

sylar::ConfigVar<int>::ptr g_init_value_config =
    sylar::Config::Lookup("system.port", (int)8080, "system port");

int main()
{
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << g_init_value_config->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << g_init_value_config->toString();
    return 0;
}