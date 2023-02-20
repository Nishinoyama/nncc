cmake_minimum_required(VERSION 3.14)
project(nncc C)

set(CMAKE_C_STANDARD 11)

add_executable(nncc nncc.c nncc.h token.c token.h node.c node.h lvar.c lvar.h main.c)

enable_testing()
add_test(NAME pretests COMMAND bash ../test/test.sh)
