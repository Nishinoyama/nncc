#include "nncc.h"
#include "token.h"
#include "node.h"
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

void gen_code(Node *node) {
    if (node->kind == ND_NUM) {
        printf("    push %d\n", node->val);
        return;
    }

    gen_code(node->lhs);
    gen_code(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
    }
    printf("    push rax\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s code\n", argv[0]);
        return 1;
    }

    char *p = argv[1];
    token = tokenize(p);

    Node *node = expr();

    print_asm_header();
    printf("main:\n");

    gen_code(node);
    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}
