#include "token.h"
#include <stdlib.h>

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node* new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
/**
 * expression
 * expr:
 *  mul ("+" mul | "-" mul)*
 */
Node *expr();
/**
 * multiplicative-expression
 * mul:
 *  primary ("*" primary | "/" primary)*
 */
Node *mul();
/**
 * multiplicative-expression
 * mul:
 *  num
 *  "(" expr ")"
 */
Node *primary();
