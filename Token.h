#pragma once
#include <string>

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	EOF_TOKEN
};

//Token class - just stores a token at this point
class Token {

private:
	TokenType tokenType;
	std::string lexeme;
	int line;

public:
	Token(TokenType tokenType, std::string lexeme, int line)
	{
		this->tokenType = tokenType;
		this->lexeme = lexeme;
		this->line = line;
	}

	Token() {}

	std::string getLexeme()
	{
		return this->lexeme;
	}

	TokenType getTokenType()
	{
		return this->tokenType;
	}
};