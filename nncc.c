#include "nncc.h"
#include "token.h"
#include "node.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void panic(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    assert(false);
    exit(1);
}

Token *token;

void print_asm_header(void) {
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
}

/// return address of variable
void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        panic("Assignee is not variable");
    }
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

int label_number = 0;

void gen_code(Node *node) {
    if (!node) {
        printf("    push 0\n");
        return;
    }
    if (node->kind == ND_NUM) {
        printf("    push %d\n", node->val);
        return;
    }
    if (node->kind == ND_LVAR) {
        gen_lval(node);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    }
    if (node->kind == ND_ASSIGN) {
        gen_lval(node->lhs);
        gen_code(node->rhs);
        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    }
    if (node->kind == ND_RETURN) {
        gen_code(node->lhs);
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
    }
    if (node->kind == ND_IF) {
        int lbn = label_number++;
        gen_code(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je  .Lelse%03d\n", lbn);
        gen_code(node->rhs->lhs);
        printf("    jmp .Lend%03d\n", lbn);
        printf(".Lelse%03d:\n", lbn);
        gen_code(node->rhs->rhs);
        printf(".Lend%03d:\n", lbn);
        return;
    }
    if (node->kind == ND_WHILE) {
        int lbn = label_number++;
        printf(".Lloop_begin%03d:\n", lbn);
        gen_code(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je  .Lloop_end%03d\n", lbn);
        gen_code(node->rhs);
        printf("    jmp .Lloop_begin%03d\n", lbn);
        printf(".Lloop_end%03d:\n", lbn);
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
        case ND_EQ:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NE:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LT:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LE:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        default:
            panic("Unimplemented node");
    }
    printf("    push rax\n");
}

int nncc(char *p) {
    token = tokenize(p);
    program();

    print_asm_header();
    printf("main:\n");

    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n");

    for (int i = 0; codes[i]; ++i) {
        gen_code(codes[i]);
        printf("    pop rax\n");
    }

    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
    return 0;
}
