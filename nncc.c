#include "nncc.h"
#include "token.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void panic(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

Token *token;

void print_asm_header(void) {
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s code\n", argv[0]);
        return 1;
    }

    char *p = argv[1];
    token = tokenize(p);

    print_asm_header();
    printf("main:\n");
    printf("    mov rax, %d\n", token_number_expect());

    while (!token_is_eof()) {
        if (token_consume("+")) {
            printf("    add rax, %d\n", token_number_expect());
        } else if (token_consume("-")) {
            printf("    sub rax, %d\n", token_number_expect());
        } else {
            panic("Unexpeced token: expected +, -  but %.*s", token->len, token->str);
            return 1;
        }
    }
    printf("    ret\n");
    return 0;
}
