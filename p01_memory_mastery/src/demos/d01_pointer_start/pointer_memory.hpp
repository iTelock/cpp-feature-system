#pragma once

#include "core/DemoBase.hpp"

namespace d01 {
class DemoSmartPointers : public Demo
{
public:
    std::string name() const override;
    std::string description() const override;
    void run() override;
};

class Pointer
{
public:
	Pointer() = default;
	~Pointer() = default;
    void run(); // 演示基本指针用法.

private:

};








}


