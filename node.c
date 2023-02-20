#include "node.h"
#include "nncc.h"

Node *codes[100];

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
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

void *program() {
    int i = 0;
    while (!token_is_eof()) {
        codes[i++] = stmt();
    }
    codes[i] = NULL;
}

Node *stmt() {
    Node *node = expr();
    token_expect(";");
    return node;
}

/**
 * expression
 * expr:
 *  assign
 */
Node *expr() {
    Node *node = assign();
    return node;
}

/**
 * assignment-expression
 * assign:
 * !equality ("=" assign)?
 */
Node *assign() {
    Node *node = equality();
    if (token_consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}


/**
 * equality-expression
 * equality:
 *  relation (("==" | "!=") relation)*
 */
Node *equality() {
    Node *node = relation();
    for (;;) {
        if (token_consume("=="))
            node = new_node(ND_EQ, node, relation());
        else if (token_consume("!="))
            node = new_node(ND_NE, node, relation());
        else
            return node;
    }
    return node;
}

/**
 * relation-expression
 * relation:
 * !shift-expression (("<" | ">" | "<=" | ">=") shift-expression)*
 */
Node *relation() {
    Node *node = add();
    for (;;) {
        if (token_consume("<"))
            node = new_node(ND_LT, node, add());
        else if (token_consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (token_consume("<="))
            node = new_node(ND_LE, node, add());
        else if (token_consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
    return node;
}

/**
 * additive-expression
 * add:
 * !mul (("+" | "-") mul)*
 *
 */
Node *add() {
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
 *  unary (("*" | "/") unary)*
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
 *  ident
 *  "(" expr ")"
 *
 */
Node *primary() {
    if (token_consume("(")) {
        Node *node = expr();
        token_expect(")");
        return node;
    }
    Token *ident_token = token;
    if (token_ident_consume()) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        node->offset = (ident_token->str[0] - 'a' + 1) * 8;
        return node;
    }
    int val = token_number_expect();
    return new_node_num(val);
}
