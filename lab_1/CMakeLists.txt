add_executable(lab1 lab1.c lab1-verification.c)
target_compile_options(lab1 PRIVATE -O3 -Wall -Wextra)
target_link_libraries(lab1 m)