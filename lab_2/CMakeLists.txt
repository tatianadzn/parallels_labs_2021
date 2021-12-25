add_executable(lab2 lab2.c lab2-verification.c)
target_compile_options(lab2 PRIVATE -O3 -Wall -Wextra)
target_link_libraries(lab2 m)