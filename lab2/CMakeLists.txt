add_executable(lab2-prelab-I instructor/person)
add_executable(lab2-prelab-II prelab.cpp)
add_executable(lab2 lab2.cpp)

# Disable literal suffixes warning due to bug in GCC.
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65923
target_compile_options(lab2-prelab-II PRIVATE -Wno-literal-suffix)
target_compile_options(lab2 PRIVATE -Wno-literal-suffix)
