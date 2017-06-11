//
// Created by Windrian on 19.03.2017.
//

#ifndef SRCCMAKE_SINGLETON_H
#define SRCCMAKE_SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T instance;
        return instance;
    }
protected:
    Singleton() {}
    ~Singleton() {}
public:
    Singleton(Singleton const &) = delete;
    Singleton& operator=(Singleton const &) = delete;
};

#endif //SRCCMAKE_SINGLETON_H