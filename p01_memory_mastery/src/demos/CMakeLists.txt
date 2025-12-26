# 把所有 demo 的 .cpp 汇总到 DEMO_SOURCES 变量中
set(DEMO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/d01_pointer_start/pointer_memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/d02_memory_addr/memory_address.cpp
    PARENT_SCOPE
)
