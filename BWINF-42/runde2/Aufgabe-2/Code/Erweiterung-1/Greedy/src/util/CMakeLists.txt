# Add header and source files
add_library(util
    include/string_utils.h
    src/string_utils.cpp
)

# Include directories
target_include_directories(util PUBLIC include)