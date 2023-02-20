#include "nncc.h"
#include "token.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool start_with(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

bool is_identifier_non_digit(char c) {
    return isalpha(c) || c == '_';
}

bool is_identifier(char c) {
    return is_identifier_non_digit(c) || isdigit(c);
}

bool token_consume(char *op) {
    if (token->kind == TK_RESERVED &&
        strlen(op) == token->len &&
        memcmp(token->str, op, token->len) == 0) {
        token = token->next;
        return true;
    }
    return false;
}

Token *token_ident_consume() {
    if (token->kind == TK_IDENTIFIER) {
        Token *tok = token;
        token = token->next;
        return tok;
    }
    return NULL;
}

void token_expect(char *op) {
    if (!token_consume(op)) {
        panic("Unexpected token: expected %s but %.*s", op, token->len, token->str);
    }
}

int token_number_expect() {
    if (token->kind != TK_NUM) {
        panic("Unexpected token: expected {number} but %.*s", token->len, token->str);
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool token_is_eof() {
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (start_with(p, ">=") || start_with(p, "<=") ||
            start_with(p, "==") || start_with(p, "!=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }
        if (strchr("+-*/()<=>;", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }
        // one character identifier
        if (is_identifier_non_digit(*p)) {
            char *name = p;
            int len = 1;
            while (is_identifier(*++p)) len++;
            cur = new_token(TK_IDENTIFIER, cur, name, len);
            continue;
        }
        panic("Failed to tokenize: %.7s...", p);
    }
    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

