#include "node.h"
#include "nncc.h"
#include "lvar.h"

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

/**
 * statement
 * stmt:
 *  expr";"
 *  "return" expr ";"
 *  "if" "(" expr ")" stmt ("else" stmt)?
 *  "while" "(" expr ")" stmt ";"
 */
Node *stmt() {
    Node *node;
    if (token_type_consume(TK_RETURN)) {
        node = new_node(ND_RETURN, expr(), NULL);
    } else if (token_type_consume(TK_IF)) {
        token_expect("(");
        Node *cond_expr = expr();
        token_expect(")");
        Node *true_stmt = stmt();
        Node *false_stmt = token_type_consume(TK_ELSE) ? stmt() : NULL;
        Node *stmts = new_node(ND_IF, true_stmt, false_stmt);
        node = new_node(ND_IF, cond_expr, stmts);
        return node;
    } else if (token_type_consume(TK_WHILE)) {
        token_expect("(");
        Node *cond_expr = expr();
        token_expect(")");
        node = new_node(ND_WHILE, cond_expr, stmt());
        return node;
    } else {
        node = expr();
    }
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
    Token *ident_token = token_ident_consume();
    if (ident_token) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = lvar_find(ident_token);
        if (!lvar) {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = ident_token->str;
            lvar->len = ident_token->len;
            if (locals) lvar->offset = locals->offset + 8;
            else lvar->offset = 8;
            locals = lvar;
        }
        node->offset = lvar->offset;
        return node;
    }
    int val = token_number_expect();
    return new_node_num(val);
}
