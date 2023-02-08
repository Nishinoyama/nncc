#ifndef NNCC_TOKEN_H
#define NNCC_TOKEN_H

#include <stdbool.h>

typedef enum {
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    char *str;
    int len;
    int val;
};

extern Token *token;

bool token_consume(char *op);
void token_expect(char *op);
bool token_is_eof();
int token_number_expect();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

#endif //NNCC_TOKEN_H
