#pragma once

#include <string>

// 所有 demo 的统一接口
class Demo
{
public:
    virtual ~Demo() = default;

    // demo 的短名称（菜单中显示）
    virtual std::string name() const = 0;

    // 简要描述（可选）
    virtual std::string description() const { return {}; }

    // 具体执行逻辑
    virtual void run() = 0;
};
