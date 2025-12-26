#include "core/DemoRegistry.hpp"

#include <stdexcept>
#include <iostream>

DemoRegistry::DemoRegistry()
{
    std::cout << "[DemoRegistry] ctor\n";
}

DemoRegistry& DemoRegistry::instance()
{
    static DemoRegistry inst;   // static + ·µ»ØÒýÓÃ.
    std::cout << "[DemoRegistry] instance() called\n";
    return inst;
}

void DemoRegistry::register_demo(const std::string& id,
    const std::string& name,
    const std::string& description,
    std::function<std::unique_ptr<Demo>()> factory)
{
    std::cout << "[DemoRegistry] register_demo called, id = " << id << std::endl;
    demos_[id] = DemoInfo{ id, name, description, std::move(factory) };
}

std::vector<DemoInfo> DemoRegistry::list_all() const
{
    std::vector<DemoInfo> result;
    result.reserve(demos_.size());
    for (const auto& kv : demos_) {
        result.push_back(kv.second);
    }
    return result;
}

std::unique_ptr<Demo> DemoRegistry::create(const std::string& id) const
{
    auto it = demos_.find(id);
    if (it == demos_.end()) {
        return nullptr;
    }
    return it->second.factory();
}
