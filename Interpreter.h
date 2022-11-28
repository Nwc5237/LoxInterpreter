#pragma once
#include "Expr.h"
#include "Stmt.h"

void *traverseExpr(Expr* expr);
double traverseUnary(Unary* unary);
double traverseBinary(Binary* binary);
double traverseLiteral(Literal* lit);
double traverseGrouping(Grouping* grouping);

void traversePrint(PrintStmt* stmt);