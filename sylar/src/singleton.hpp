#pragma once

#include <memory>

namespace sylar
{

    template <class T, class X = void, int N = 0>
    class Singleton
    {
    private:
    public:
        static T *GetInstance()
        {
            static T v;
            return &v;
        }
    };

    template <class T, class X = void, int N = 0>
    class SingletonPtr
    {
    private:
    public:
        static std::shared_ptr<T> GetInstance()
        {
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };

}