#pragma once
#include "Token.h"
#include "Expr.h"
#include <vector>

class Parser
{
public:
	std::vector<Token *> tokens;
	int cur;

	Parser(std::vector<Token *> tokens);
	
	//class bookkeeping functions
	Token *peek();
	Token *advance();
	Token *previous();
	bool check(TokenType type);
	bool match(std::vector<TokenType> tokenTypes);

	//parsing functions
	Expr* expression();
	Expr* equality();
	Expr* comparison();
	Expr* term();
	Expr* factor();
	Expr* unary();
	Expr* primary();
};