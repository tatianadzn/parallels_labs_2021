add_executable(lab3 lab3.c lab3_schedule.c)
target_compile_options(lab3 PRIVATE -O3 -Wall -Wextra)
target_link_libraries(lab3 m)