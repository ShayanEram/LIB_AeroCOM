cmake_minimum_required(VERSION 3.10)
project(AeroCOM VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------
# 1. Build Unified com Shared Library
# -----------------------------
add_library(com SHARED
    # Comm Drivers
    Comm/ARINC429/Src/arinc429.c
    Comm/ARINC664/Src/arinc664.c
    Comm/MILSTD1553B/Src/milstd1553b.c
)

# -----------------------------
# 2. Export Header Directories
# -----------------------------
target_include_directories(com PUBLIC
    ARINC429/Inc
    ARINC664/Inc
    MILSTD1553B/Inc
)