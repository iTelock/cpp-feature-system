#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ============================
// 小工具：用于统一打印日志
// ============================
namespace lab {

    inline void log(const std::string& msg) {
        std::cout << msg << std::endl;
    }

} // namespace lab

// ============================
// Buffer：最小“资源类”实验体
// - 持有一段堆内存（模拟系统资源：fd/mmap/socket 等）
// - 禁拷贝（copy deleted）
// - 支持移动（move ctor / move assign）
// - 打印：构造/析构/移动/拷贝（拷贝这里会被 delete，留给你做对比实验）
// ============================
class Buffer final {
public:
    using byte = std::uint8_t;

    // 构造一个 size 字节的 buffer，并用 fill_value 填充（方便 debug）
    explicit Buffer(std::size_t size, byte fill_value = 0xAB)
        : id_(next_id_++), size_(size), data_(size ? std::make_unique<byte[]>(size) : nullptr) {
        if (data_) {
            std::memset(data_.get(), fill_value, size_);
        }
        lab::log("[ctor] Buffer#" + std::to_string(id_) +
            " size=" + std::to_string(size_) +
            " data=" + ptr_str());
    }

    // 默认构造：空 buffer
    Buffer() : id_(next_id_++), size_(0), data_(nullptr) {
        lab::log("[ctor] Buffer#" + std::to_string(id_) +
            " default (empty) data=" + ptr_str());
    }

    // 析构：释放资源（unique_ptr 自动释放）
    ~Buffer() {
        lab::log("[dtor] Buffer#" + std::to_string(id_) +
            " size=" + std::to_string(size_) +
            " data=" + ptr_str());
    }

    // ----------------------------
    // 禁拷贝：系统级资源类默认姿势
    // ----------------------------
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // ----------------------------
    // 支持移动：转移所有权（noexcept 很重要）
    // ----------------------------
    Buffer(Buffer&& other) noexcept
        : id_(next_id_++),                 // 这里故意给“新对象”分配新 id，便于看谁是谁
        size_(other.size_),
        data_(std::move(other.data_)) {  // 转移指针所有权
        other.size_ = 0;                   // 让被移动对象变成“空壳”
        lab::log("[move-ctor] Buffer#" + std::to_string(id_) +
            " <= moved-from Buffer#" + std::to_string(other.id_) +
            " size=" + std::to_string(size_) +
            " data=" + ptr_str());
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) {
            lab::log("[move-assign] self-assignment Buffer#" + std::to_string(id_));
            return *this;
        }

        // 先释放自己原来的资源（unique_ptr 自动做）
        size_ = other.size_;
        data_ = std::move(other.data_);
        other.size_ = 0;

        lab::log("[move-assign] Buffer#" + std::to_string(id_) +
            " <= moved-from Buffer#" + std::to_string(other.id_) +
            " size=" + std::to_string(size_) +
            " data=" + ptr_str());
        return *this;
    }

    // 观察接口
    std::size_t size() const noexcept { return size_; }
    const byte* data() const noexcept { return data_.get(); }
    bool empty() const noexcept { return size_ == 0 || data_ == nullptr; }

    std::string debug() const {
        return "Buffer#" + std::to_string(id_) +
            " size=" + std::to_string(size_) +
            " data=" + ptr_str() +
            (empty() ? " (empty)" : "");
    }

private:
    std::string ptr_str() const {
        // 打印指针地址（仅用于日志观察）
        const void* p = static_cast<const void*>(data_.get());
        std::string s;
        s.reserve(32);
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%p", p);
        s = buf;
        return s;
    }

private:
    inline static std::size_t next_id_ = 1;

    std::size_t id_{ 0 };
    std::size_t size_{ 0 };
    std::unique_ptr<byte[]> data_{ nullptr };
};

// ============================
// 场景 C：noexcept 的关键性（演示版本）
// 说明：我们用一个“会 throw 的移动构造”类型，观察 vector 是否会更倾向 copy。
// 注意：这里为了演示，会允许拷贝。
// ============================
class MaybeThrowMoveBuffer final {
public:
    explicit MaybeThrowMoveBuffer(std::size_t n)
        : id_(next_id_++), size_(n), data_(n ? std::make_unique<std::uint8_t[]>(n) : nullptr) {
        lab::log("[ctor] MT#" + std::to_string(id_) + " size=" + std::to_string(size_));
    }

    ~MaybeThrowMoveBuffer() {
        lab::log("[dtor] MT#" + std::to_string(id_) + " size=" + std::to_string(size_));
    }

    // 为了能看到“vector 退化为拷贝”的现象，这里允许拷贝（深拷贝）
    MaybeThrowMoveBuffer(const MaybeThrowMoveBuffer& other)
        : id_(next_id_++), size_(other.size_),
        data_(other.size_ ? std::make_unique<std::uint8_t[]>(other.size_) : nullptr) {
        lab::log("[copy-ctor] MT#" + std::to_string(id_) + " <= MT#" + std::to_string(other.id_));
        if (data_ && other.data_) {
            std::memcpy(data_.get(), other.data_.get(), size_);
        }
    }

    MaybeThrowMoveBuffer& operator=(const MaybeThrowMoveBuffer& other) {
        if (this == &other) return *this;
        lab::log("[copy-assign] MT#" + std::to_string(id_) + " <= MT#" + std::to_string(other.id_));
        size_ = other.size_;
        data_.reset(other.size_ ? new std::uint8_t[other.size_] : nullptr);
        if (data_ && other.data_) {
            std::memcpy(data_.get(), other.data_.get(), size_);
        }
        return *this;
    }

    // 关键：移动构造 NOT noexcept（演示用）
    MaybeThrowMoveBuffer(MaybeThrowMoveBuffer&& other) /* not noexcept */
        : id_(next_id_++), size_(other.size_), data_(std::move(other.data_)) {
        other.size_ = 0;
        lab::log("[move-ctor NOT noexcept] MT#" + std::to_string(id_) +
            " <= moved-from MT#" + std::to_string(other.id_));
    }

    MaybeThrowMoveBuffer& operator=(MaybeThrowMoveBuffer&& other) /* not noexcept */ {
        if (this == &other) return *this;
        size_ = other.size_;
        data_ = std::move(other.data_);
        other.size_ = 0;
        lab::log("[move-assign NOT noexcept] MT#" + std::to_string(id_) +
            " <= moved-from MT#" + std::to_string(other.id_));
        return *this;
    }

private:
    inline static std::size_t next_id_ = 1;
    std::size_t id_{ 0 };
    std::size_t size_{ 0 };
    std::unique_ptr<std::uint8_t[]> data_{ nullptr };
};