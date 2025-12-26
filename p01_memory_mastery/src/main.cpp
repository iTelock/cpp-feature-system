#include "core/DemoRegistry.hpp"

#include <iostream>
#include <limits>

static void print_menu()
{
    std::cout << "[main] enter main()\n";
    auto& registry = DemoRegistry::instance();
    const auto demos = registry.list_all();
    std::cout << "[main] demos.size() = " << demos.size() << "\n";

    std::cout << "================ Memory Mastery ================\n";
    std::cout << "选择要运行的 Demo（输入编号，0 退出）：\n\n";

    int index = 1;
    for (const auto& demo : demos) {
        std::cout << "  " << index << ") [" << demo.id << "] "
            << demo.name;
        if (!demo.description.empty()) {
            std::cout << " - " << demo.description;
        }
        std::cout << "\n";
        ++index;
    }

    if (demos.empty()) {
        std::cout << "  (当前没有注册任何 demo)\n";
    }

    std::cout << "\n> ";
}

int main()
{
    auto& registry = DemoRegistry::instance();

    while (true) {
        print_menu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            // 输入不是数字，清理一下输入流
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效输入，请重新输入数字。\n\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Bye ~\n";
            break;
        }

        const auto demos = registry.list_all();
        if (choice < 1 || choice > static_cast<int>(demos.size())) {
            std::cout << "编号超出范围，请重新选择。\n\n";
            continue;
        }

        const auto& info = demos[choice - 1];
        auto demo = registry.create(info.id);
        if (!demo) {
            std::cout << "创建 demo 失败，id=" << info.id << "\n\n";
            continue;
        }

        std::cout << "\n>>> 运行 Demo: " << info.name << " <<<\n\n";
        demo->run();
        std::cout << "\n>>> Demo 结束，按回车继续...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    return 0;
}
