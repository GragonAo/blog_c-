#pragma once
#include <drogon/DrObject.h>

// 使用模板实现 CRTP
template <typename T>
class ServiceBase : public drogon::DrObject<T>
{
public:
    /**
     * 获取子类单例的静态方法
     * 子类继承后，直接调用 SubClass::instance() 即可
     */
    static std::shared_ptr<T> instance() {
        return drogon::DrClassMap::getSingleInstance<T>();
    }
};