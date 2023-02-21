#ifndef NNCC_NODE_H
#define NNCC_NODE_H

#include "token.h"
#include <stdlib.h>

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_ASSIGN,
    ND_LVAR,
    ND_RETURN,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};

extern Node *codes[100];

void *program();

Node *assign();

Node *stmt();

Node *expr();

Node *equality();

Node *relation();

Node *add();

Node *mul();

Node *unary();

Node *primary();

#endif //NNCC_NODE_H
