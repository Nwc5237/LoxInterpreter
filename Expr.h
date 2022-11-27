#pragma once
#include "Token.h"

//for this forward declaration to work, all of the types need to be pointers since
//otherwise we'd be trying to use the default constructor on an incomplete type
class Unary;
class Binary;
class Literal;
class Grouping;

class Expr
{
public:
	Expr() {}

	Expr(Literal* literal) { this->literal = literal; }

	Expr(Unary* unary) { this->unary = unary; }

	Expr(Binary* binary) { this->binary = binary; }

	Expr(Grouping* grouping) { this->grouping = grouping; }

	Literal *literal = nullptr;
	Unary *unary = nullptr;
	Binary *binary = nullptr;
	Grouping *grouping = nullptr;
};

class Unary : public Expr
{
public:
	Unary() {}

	Unary(Token* op, Expr* expr)
	{
		this->op = op;
		this->expr = expr;
	}

	Token *op = nullptr;
	Expr *expr = nullptr;
};

class Binary : public Expr
{
public:
	Binary() {}

	Binary(Expr* left, Token* op, Expr* right)
	{
		this->left = left;
		this->op = op;
		this->right = right;
	}

	Expr *left = nullptr;
	Token *op = nullptr;
	Expr *right = nullptr;
};

class Grouping : public Expr
{
public:
	Grouping() {}

	Grouping(Token* leftParen, Expr* expr, Token* rightParen)
	{
		this->leftParen = leftParen;
		this->expr = expr;
		this->rightParen = rightParen;
	}

	Token *leftParen = nullptr;
	Expr *expr = nullptr;
	Token *rightParen = nullptr;
};

class Literal : public Expr
{
public:
	Literal() {}

	Literal(Token* literal)
	{
		this->literal = literal;
	}

	Token *literal = nullptr;
};