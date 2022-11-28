#include "Parser.h"

//constructor
Parser::Parser(std::vector<Token *> tokens)
{
	this->cur = 0;
	this->tokens = tokens;
}

//check the current token without changing any state
Token *Parser::peek()
{
	return this->tokens.at(cur);
}

//advance the token and return the one we just ate
Token *Parser::advance()
{
	if (cur + 1 < this->tokens.size())
	{
		cur++;
		return previous();
	}
	return peek();
}

//return token before cur
Token *Parser::previous()
{
	return this->tokens.at(cur - 1);
}

//checks if current token is of the TokenType `type`
bool Parser::check(TokenType type)
{
	return type == tokens.at(cur)->getTokenType();
}

//Checks if the current token is in a list of TokenTypes --- returns true / false accordingly
bool Parser::match(std::vector<TokenType> tokenTypes)
{
	for (TokenType type : tokenTypes)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}

	return false;
}

/*
Seems like the next thing to do will probably be to return null pointers when
a parse fails. Then you can do an if(ptr) and if it evaluates to false, you
can check the rest of the potential matches exist in the grammar
*/

PrintStmt* Parser::printStmt()
{
	PrintStmt* stmt = (PrintStmt*)malloc(sizeof(PrintStmt));
	Expr* expr;
	Token *print, *semicolon;

	if (match(std::vector<TokenType>{PRINT}))
		print = previous();
	else
		return nullptr;

	if (!(expr = expression()))
		return nullptr;

	if (match(std::vector<TokenType>{SEMICOLON}))
		semicolon = previous();
	else
		return nullptr;

	*stmt = PrintStmt(print, expr, semicolon);
	return stmt;
}

Expr* Parser::expression()
{
	return equality();
}

Expr* Parser::equality()
{
	Expr* expr = comparison();
	while (match(std::vector<TokenType>{BANG_EQUAL, EQUAL_EQUAL}))
	{
		Expr* left = expr;
		Token* op = previous();
		Expr* right = comparison();

		Binary* binary = (Binary*)malloc(sizeof(Binary));
		*binary = Binary(left, op, right);

		expr = (Expr*)malloc(sizeof(Expr));
		*expr = Expr(binary);
	}

	return expr;
}

//returns Expressions like A < B or A <= B or A > B or A >= B
Expr* Parser::comparison()
{
	Expr* expr = term();
	while (match(std::vector<TokenType>{LESS, LESS_EQUAL, GREATER, GREATER_EQUAL}))
	{
		Expr* left = expr;
		Token* op = previous();
		Expr* right = term();

		Binary* binary = (Binary*)malloc(sizeof(Binary));
		*binary = Binary(left, op, right);

		expr = (Expr*)malloc(sizeof(Expr));
		*expr = Expr(binary);
	}

	return expr;
}


//Returns A + B or A - B expressions
Expr* Parser::term()
{
	Expr* expr = factor();

	while (match(std::vector<TokenType>{PLUS, MINUS}))
	{
		Expr* left = expr;
		Token* op = previous();
		Expr* right = factor(); // come back

		Binary* binary = (Binary*)malloc(sizeof(Binary));
		*binary = Binary(left, op, right);

		expr = (Expr*)malloc(sizeof(Expr));
		*expr = Expr(binary);
	}

	return expr;
}

//Returns A * B or A / B types expressions
Expr* Parser::factor()
{
	Expr* expr = unary();

	while (match(std::vector<TokenType>{STAR, SLASH}))
	{
		Expr* left = expr;
		Token* op = previous();
		Expr* right = unary(); //come back

		Binary* binary = (Binary*)malloc(sizeof(Binary));
		*binary = Binary(left, op, right);

		expr = (Expr*)malloc(sizeof(Expr));
		*expr = Expr(binary);
	}

	return expr;
}

Expr* Parser::unary()
{
	Expr* expr = (Expr*)malloc(sizeof(Expr));

	if (match(std::vector<TokenType>{BANG, MINUS}))
	{
		Unary* un = (Unary*)malloc(sizeof(Unary));

		Token* op = previous();
		expr = unary();

		*un = Unary(op, expr);
		expr = (Expr*)malloc(sizeof(Expr));
		*expr = Expr(un);

		return expr;
	}
	expr = primary();
	return expr;
}

Expr* Parser::primary()
{
	Expr* expr = (Expr*)malloc(sizeof(Expr));
	Literal* literal = (Literal*)malloc(sizeof(Literal));
	switch (peek()->getTokenType())
	{
	case NUMBER:
	case STRING:
	case TRUE:
	case FALSE:
	case NIL:
		*literal = Literal(advance()); //here we might want to create this, then pass it to that base class Expr type (so one more line underneath with expr constructor that takes a literal pointer) then return that instead
		*expr = Expr(literal);
		return expr;

	case LEFT_PAREN:
		Grouping* grouping = (Grouping *)malloc(sizeof(Grouping));
		Token* leftParen = advance();

		Expr* expr = expression();
		
		Token* rightParen;
		
		if (match(std::vector<TokenType>{RIGHT_PAREN}))
		{
			rightParen = previous();
			*grouping = Grouping(leftParen, expr, rightParen);
			expr = (Expr*)malloc(sizeof(Expr));
			*expr = Expr(grouping);
			return expr;
		}
		
		printf("Error::missing right parenthesis on line:%d\n", tokens.at(cur)->getLine());
	}
}