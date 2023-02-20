#ifndef NNCC_LVAR_H
#define NNCC_LVAR_H

#include "token.h"

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

extern LVar *locals;

LVar *lvar_find(Token *);

#endif //NNCC_LVAR_H
