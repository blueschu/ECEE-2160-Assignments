add_executable(lab1-prelab prelab.cpp)
add_executable(lab1 lab1.cpp double_vec.cpp)

# Disable literal suffixes warning due to bug in GCC.
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65923
target_compile_options(lab1 PRIVATE -Wno-literal-suffix)
