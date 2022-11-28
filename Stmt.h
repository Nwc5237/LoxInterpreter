#pragma once
#include "Token.h"
#include "Expr.h"

//Base statement class
class Stmt { };

//statement type for printing
class PrintStmt : Stmt
{
public:
	Token* printToken;
	Expr* expr;
	Token* semicolon;

	PrintStmt(Token* printToken, Expr* expr, Token* semicolon)
	{
		this->printToken = printToken;
		this->expr = expr;
		this->semicolon = semicolon;
	}
};