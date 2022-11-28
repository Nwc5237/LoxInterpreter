#pragma once
#include "Token.h"
#include <string>
#include <vector>

class Lexer
{
public:
	Lexer(std::string sourceCode);
	void lex();
	std::vector<Token> getLexemes();
	std::vector<Token*> getLexemesRef();

private:
	std::vector<Token> lexemes;
	std::string sourceCode;
	int lineNum;

	bool isDigit(char value);
	bool isAtEnd(int* forward);
	bool match(char expected, int* forward);
	bool peek(int location, char value);
	void createToken(TokenType tokenType, int* start, int* forward);
	bool matchSkip(int* start, int* forward);
	bool in(char val, std::string set);
	bool matchNum(int* start, int* forward);
	void error(int* start, int* forward);
	bool matchString(int* start, int* forward);
	bool matchIdentifierKeyword(int* start, int* forward);
	void resetForward(int* start, int* forward);
	void trailingEqualsLexemeCheck(char op, int* start, int* forward);
	bool keywordReader(std::string keyword, TokenType type, int* start, int* forward);
};