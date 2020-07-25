add_executable(lab0-1-hello hello.cpp)
add_executable(lab0-2-array array.cpp)
add_executable(lab0-3-sort-strings sort_strings.cpp)

# Suppress warning in assigment 3 for unknown pragma.
target_compile_options(lab0-2-array PRIVATE -Wno-unknown-pragmas)
