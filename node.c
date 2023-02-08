#include "node.h"

Node* new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

/**
 * expression
 * expr:
 *  mul ("+" mul | "-" mul)*
 *
 */
Node *expr() {
    Node *node = mul();

    for (;;) {
        if (token_consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (token_consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}


/**
 * multiplicative-expression
 * mul:
 *  unary ("*" unary | "/" unary)*
 *
 */
Node *mul() {
    Node *node = unary();

    for (;;) {
        if (token_consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (token_consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

/**
 * unary-expression
 * unary:
 *  primary
 *  "+" primary
 *  "-" primary
 */
Node *unary() {
    if (token_consume("+"))
        return primary();
    else if (token_consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    else
        return primary();
}

/**
 * multiplicative-expression
 * mul:
 *  num
 *  "(" expr ")"
 *
 */
Node *primary() {
    if (token_consume("(")) {
        Node *node = expr();
        token_expect(")");
        return node;
    }
    int val = token_number_expect();
    return new_node_num(val);
}
