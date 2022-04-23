#pragma once

namespace ant2d {
template <class T>
class Singleton : public T
{
public:
    static Singleton& shared()
    {
        static auto* _instance = new Singleton();
        return *_instance;
    }
    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&)  = delete;
private:
    Singleton() {}
};
} //namespace ant2d
