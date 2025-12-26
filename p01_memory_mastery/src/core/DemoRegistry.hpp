#pragma once

#include "core/DemoBase.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct DemoInfo
{
    std::string id;          // 内部 ID（菜单编号对应）
    std::string name;        // 显示名称
    std::string description; // 描述
    std::function<std::unique_ptr<Demo>()> factory; // 创建对象的工厂函数
};

class DemoRegistry
{
public:
    static DemoRegistry& instance();

    // 注册一个 demo（通常由宏在每个 demo cpp 中自动调用）
    void register_demo(const std::string& id,
        const std::string& name,
        const std::string& description,
        std::function<std::unique_ptr<Demo>()> factory);

    // 列出所有 demo（用于菜单）
    std::vector<DemoInfo> list_all() const;

    // 根据 id 创建一个 demo 实例
    std::unique_ptr<Demo> create(const std::string& id) const;

private:
    DemoRegistry();

    std::unordered_map<std::string, DemoInfo> demos_;
};

// 方便在每个 demo 源文件里一行注册
#define REGISTER_DEMO(ID, TYPE, DESC)                    \
    namespace {                                          \
        struct TYPE##Registrar {                         \
            TYPE##Registrar() {                          \
                DemoRegistry::instance().register_demo(  \
                    ID,                                  \
                    #TYPE,                               \
                    DESC,                                \
                    [] { return std::make_unique<TYPE>(); } \
                );                                       \
            }                                            \
        };                                               \
        static TYPE##Registrar global_##TYPE##Registrar; \
    }
