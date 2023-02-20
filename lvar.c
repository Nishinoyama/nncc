#include "lvar.h"
#include "nncc.h"
#include <string.h>

LVar *locals;

LVar *lvar_find(Token *tok) {
    for (LVar *lvar = locals; lvar; lvar = lvar->next) {
        if (lvar->len == tok->len &&
            !memcmp(tok->str, lvar->name, lvar->len)) {
            return lvar;
        }
    }
    return NULL;
}

