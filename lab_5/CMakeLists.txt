add_executable(lab5 lab5.c)
target_compile_options(lab5 PRIVATE -O3 -Wall -Wextra)
target_link_libraries(lab5 m)