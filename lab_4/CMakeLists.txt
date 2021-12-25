add_executable(lab4 lab4.c)
target_compile_options(lab4 PRIVATE -O3 -Wall -Wextra)
target_link_libraries(lab4 m)