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
assert 13 "1+2+3+7"
assert 12 "10 - 3 + 5"
assert 5 "   1 - 2  + 3 - 4 + 5 - 6  +7 - 8 + 9  "
assert 14 "1 + 2 * 3 + 4 / 5 + 6 - 7 + 8"
assert 10 "7 + 9 / (1 + 2)"
assert 10 "2 * 9 - (7 + 1)"

printf "\033[32m[[PRETESTS PASSED]]\033[m\n"

