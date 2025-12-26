#include "demos/d01_pointer_start/pointer_memory.hpp"
#include "core/DemoRegistry.hpp"

#include <iostream>
#include <memory>
#include <string>

#pragma message(">>> compiling pointer_memory.cpp")  // MSVC 下会在输出窗口提示.

namespace d01 {


std::string DemoSmartPointers::name() const
{
    return "Smart Pointer Basics";
}

std::string DemoSmartPointers::description() const
{
    return "指针与内存的基本概念.";
}

void DemoSmartPointers::run()
{
    Pointer p;
    p.run();
}

void Pointer::run()
{
    int* p = new int;
    std::unique_ptr<int> sp(new int);
    std::cout << "int大小:" << sizeof(int) << std::endl;
    std::cout << "p大小:" << sizeof(p) << std::endl;
    std::cout << "p地址:" << p << std::endl;
    std::cout << "sp大小:" << sizeof(sp) << std::endl;
    std::cout << "sp地址:" << sp << std::endl;
    delete p;
    std::cout << "p地址:" << p << std::endl;
    p = nullptr;
    std::cout << "p地址:" << p << std::endl;
}




























// 注册到 DemoRegistry
REGISTER_DEMO("d01_pointer_start", DemoSmartPointers,
    "智能指针基础用法")

}

// 用一个额外的静态对象来确认这个 cpp 的“全局构造阶段”有没有执行
namespace {
struct DebugPointerMemoryStatic {
    DebugPointerMemoryStatic() {
        std::cout << "[pointer_memory] global static constructed\n";
    }
};
DebugPointerMemoryStatic g_debugPointerMemoryStatic;
}