#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./nncc "$input" > ./tmp.s
    cc -o ./tmp ./tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        printf "\033[32m [PASSED] \033[m"
        echo "$input => $actual"
    else
        printf "\033[31m![FAILED] \033[m"
        echo "$input => $expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42

printf "\033[32m[[PRETEST PASSED]]\033[m\n"

