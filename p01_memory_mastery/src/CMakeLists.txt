# 核心框架库
add_library(memory_core
    core/DemoBase.hpp
    core/DemoRegistry.hpp
    core/DemoRegistry.cpp
)

target_include_directories(memory_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 让 demos 子目录把 DEMO_SOURCES 传上来
add_subdirectory(demos)

# 主程序：把 demo 的 cpp 直接编进来
add_executable(p01_memory_mastery
    main.cpp
    ${DEMO_SOURCES}
)

target_link_libraries(p01_memory_mastery
    PRIVATE
        memory_core
)

target_include_directories(p01_memory_mastery
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
