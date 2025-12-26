#include "demos/d02_memory_addr/memory_address.hpp"
#include "core/DemoRegistry.hpp"

#include <iostream>
#include <memory>
#include <string>

#pragma message(">>> compiling pointer_memory.cpp")  // MSVC 下会在输出窗口提示.

namespace d02 {


std::string MemAddrDisplay::name() const
{
    return "Memory Address Basics";
}

std::string MemAddrDisplay::description() const
{
    return "内存地址演示.";
}

void MemAddrDisplay::run()
{
    std::cout << "d02" << std::endl;
}





























    // 注册到 DemoRegistry
    REGISTER_DEMO("d02_memory_addr", MemAddrDisplay,
        "内存地址演示")

}
