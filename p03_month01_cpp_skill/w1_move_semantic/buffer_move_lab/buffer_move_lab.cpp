#include "buffer_move_lab.h"

// ============================
// 场景 A：返回值与 RVO/NRVO/move
// ============================

// 1) 直接 return 临时对象：通常会触发（或等价于）RVO
Buffer make_buffer_rvo(std::size_t n) {
    lab::log("\n--- make_buffer_rvo ---");
    return Buffer{ n, 0x11 }; // 观察：很多情况下不会打印 move-ctor
}

// 2) NRVO：返回具名局部变量 b
Buffer make_buffer_nrvo(std::size_t n) {
    lab::log("\n--- make_buffer_nrvo ---");
    Buffer b{ n, 0x22 };
    lab::log("   about to return: " + b.debug());
    return b; // 观察：很多情况下也不会打印 move-ctor（NRVO）
}

// 3) 强行 std::move：很多时候会“放弃 NRVO”，导致真的 move 一次
Buffer make_buffer_force_move(std::size_t n) {
    lab::log("\n--- make_buffer_force_move ---");
    Buffer b{ n, 0x33 };
    lab::log("   about to return std::move(b): " + b.debug());
    return std::move(b); // 观察：通常会出现 move-ctor
}

// ============================
// 场景 B：vector 行为观察
// ============================

void vector_push_emplace_resize_lab() {
    lab::log("\n============================");
    lab::log("=== vector push/emplace/resize lab ===");
    lab::log("============================");

    std::vector<Buffer> v;

    // reserve：预留容量，避免扩容搬家干扰观察
    v.reserve(3);
    lab::log("\n--- after reserve(3) ---");

    lab::log("\n[1] emplace_back(64): in-place construct");
    v.emplace_back(64, 0xAA); // 就地构造，通常只有 ctor

    lab::log("\n[2] push_back(Buffer{32}): construct temp then move into vector");
    v.push_back(Buffer{ 32, 0xBB }); // 临时对象 -> move into vector

    lab::log("\n[3] push_back(std::move(x)): move an existing object into vector");
    Buffer x{ 16, 0xCC };
    v.push_back(std::move(x)); // x 被移动后应为空壳
    lab::log("   after move, x: " + x.debug());

    // 触发扩容：当前 capacity=3，size=3，再插入会扩容搬家
    lab::log("\n[4] push_back triggers reallocation (observe moves during growth)");
    v.push_back(Buffer{ 8, 0xDD }); // 扩容时会搬家：优先用 noexcept move

    lab::log("\n[5] resize bigger: may default-construct new elements");
    v.resize(6); // 增加 2 个默认构造元素（注意：Buffer 有默认构造）

    lab::log("\n[6] resize smaller: destroys extra elements");
    v.resize(2); // 会析构被裁掉的元素

    lab::log("\n--- end of vector lab scope ---");
}


void vector_noexcept_story_lab() {
    lab::log("\n============================");
    lab::log("=== noexcept story lab (vector reallocation) ===");
    lab::log("============================");

    std::vector<MaybeThrowMoveBuffer> v;
    v.reserve(2);

    lab::log("\n[1] emplace_back 2 elements (no reallocation)");
    v.emplace_back(10);
    v.emplace_back(20);

    lab::log("\n[2] push third element -> reallocation; observe whether copy happens");
    v.emplace_back(30);

    lab::log("\n--- end of noexcept story lab scope ---");
}

// ============================
// main：按顺序跑三大场景
// ============================
int main() {
    lab::log("========== Buffer Move Lab ==========");

    // 场景 A：返回值观察
    {
        lab::log("\n============================");
        lab::log("=== return / RVO / NRVO / move lab ===");
        lab::log("============================");

        Buffer a = make_buffer_rvo(128);
        lab::log("returned a: " + a.debug());

        Buffer b = make_buffer_nrvo(256);
        lab::log("returned b: " + b.debug());

        Buffer c = make_buffer_force_move(512);
        lab::log("returned c: " + c.debug());

        lab::log("\n--- end of return lab scope ---");
    }

    // 场景 B：vector 行为观察
    vector_push_emplace_resize_lab();

    // 场景 C：noexcept 的故事（演示）
    vector_noexcept_story_lab();

    lab::log("\n========== Done ==========");
    return 0;
}