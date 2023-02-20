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

assert 0 "0;"
assert 42 "42;"
assert 13 "1+2+3+7;"
assert 12 "10 - 3 + 5;"
assert 5 "   1 - 2  + 3 - 4 + 5 - 6  +7 - 8 + 9  ;"
assert 14 "1 + 2 * 3 + 4 / 5 + 6 - 7 + 8;"
assert 10 "7 + 9 / (1 + 2);"
assert 10 "2 * 9 - (7 + 1);"
assert 32 "+( -4 * -8 );"
assert 3 "- 4 * 8 + (7 * 5);"
assert 0 "4 > 4;"
assert 1 "4 >= 4;"
assert 0 "4 < 4;"
assert 1 "4 <= 4;"
assert 0 "4 > 5;"
assert 0 "4 >= 5;"
assert 1 "4 < 5;"
assert 1 "4 <= 5;"
assert 1 "4 > 3;"
assert 1 "4 >= 3;"
assert 0 "4 < 3;"
assert 0 "4 <= 3;"
assert 1 "5 * 4 > 2 * (3 + 4);"
assert 20 "a = 4; b = 5; a * b;"
assert 20 "a = 1; b = 3; c = a < b; d = c + 3; e = (d + 1) * 4;"
assert 5 "f1 = 1; f2 = 1; f3 = f1 + f2; f4 = f2 + f3; f5 = f3 + f4;"
assert 42 "people = 1; Answer_to_the_Ultimate_Question_of_Life_the_Universe_and_Everything = 11 * 2 * (people == people) + (10 * (1 + (people < 4)));"

printf "\033[32m[[PRETESTS PASSED]]\033[m\n"

