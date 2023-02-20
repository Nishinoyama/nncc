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
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

Node *expr();

Node *equality();

Node *relation();

Node *add();

Node *mul();

Node *unary();

Node *primary();

#endif //NNCC_NODE_H
