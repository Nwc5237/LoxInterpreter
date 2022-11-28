#include "Lexer.h"

Lexer::Lexer(std::string sourceCode)
{
	this->sourceCode = sourceCode;
	this->lexemes = std::vector<Token>();
	this->lineNum = 0;
}

/*
This naming could be updated. The idea is check whether a lexeme is itself (e.g. `>`)
or if an equals sign follows (e.g. `>=`). It updates forward for us and creates a token
on the token stack
*/
void Lexer::trailingEqualsLexemeCheck(char op, int* start, int* forward)
{
	TokenType tokenType;
	bool trailingEqual = match('=', forward);
	switch (op)
	{
	case '<':
		tokenType =  trailingEqual ? LESS_EQUAL : LESS;
		break;
	case '>':
		tokenType = trailingEqual ? GREATER_EQUAL : GREATER;
		break;
	case '=':
		tokenType = trailingEqual ? EQUAL_EQUAL : EQUAL;
		break;
	case '!':
		tokenType = trailingEqual ? BANG_EQUAL : BANG;
		break;
	}
	(*forward)++;
	createToken(tokenType, start, forward);
}

void Lexer::lex()
{
	int* start, * forward;

	start = (int*)malloc(sizeof(int));
	forward = (int*)malloc(sizeof(int));

	*start = 0;
	*forward = *start;

	while (!isAtEnd(forward)) //need to update start after every created token
	{
		char cur = sourceCode.at(*forward);
		switch (cur)
		{
			//single character cases
		case '(':
			(*forward)++;
			createToken(LEFT_PAREN, start, forward);
			break;

		case ')':
			(*forward)++;
			createToken(RIGHT_PAREN, start, forward);
			break;

		case '{':
			(*forward)++;
			createToken(LEFT_BRACE, start, forward);
			break;

		case '}':
			(*forward)++;
			createToken(RIGHT_BRACE, start, forward);
			break;

		case ';':
			(*forward)++;
			createToken(SEMICOLON, start, forward);
			break;

		case ',':
			(*forward)++;
			createToken(COMMA, start, forward);
			break;

		case '.':
			(*forward)++;
			createToken(DOT, start, forward);
			break;

		case '-':
			(*forward)++;
			createToken(MINUS, start, forward);
			break;

		case '+':
			(*forward)++;
			createToken(PLUS, start, forward);
			break;

		case '*':
			(*forward)++;
			createToken(STAR, start, forward);
			break;

		case '/':
			(*forward)++;
			createToken(SLASH, start, forward);
			break;

			//up to two character lexemes
		case '=':
		case '!':
		case '>':
		case '<':
			trailingEqualsLexemeCheck(cur, start, forward);
			break;

		//arbitrary length lexemes
		default:
			if (matchSkip(start, forward))
				break;

			resetForward(start, forward);
			if (matchNum(start, forward))
			{
				createToken(NUMBER, start, forward);
				break;
			}
			
			resetForward(start, forward);
			if (matchString(start, forward))
			{
				createToken(STRING, start, forward);
				break;
			}
			
			resetForward(start, forward);
			if(matchIdentifierKeyword(start, forward))
				break;
			
			error(start, forward); // the real length is whatever the max forward length we got to was that we couldn't match
			break;
		} //switch cases end

		*start = *forward;
	}
}

std::vector<Token> Lexer::getLexemes()
{
	return this->lexemes;
}

//pushes a token onto the token vector
void Lexer::createToken(TokenType tokenType, int* start, int* forward)
{
	//(*forward)++;
	Token t = Token(tokenType, sourceCode.substr(*start, *forward - *start), lineNum);
	lexemes.push_back(t);
}

//checks if a character at `location` in sourceCode matches a character value
bool Lexer::peek(int location, char value)
{
	return location < sourceCode.size() && value == sourceCode.at(location);
}

//if we see the expected token, we advance the forward pointer, otherwise
//we don't want that character in the token
bool Lexer::match(char expected, int* forward)
{
	int inc = (*forward) + 1;

	if (inc < sourceCode.size() && sourceCode.at(inc) == expected)
	{
		*forward = inc;
		return true;
	}

	return false;
}

//checks if we're at the end of the file
bool Lexer::isAtEnd(int* forward)
{
	return *forward >= sourceCode.size();
}

bool Lexer::isDigit(char value)
{
	return value >= '0' && value <= '9';
}

bool Lexer::matchSkip(int* start, int* forward)
{
	bool matched = false;
	while (in(sourceCode.at(*forward), std::string("\n\t\r ")))
	{
		matched = true;
		if (sourceCode.at(*forward) == '\n')
			lineNum++;
		(*forward)++;
	}
	return matched;
}

bool Lexer::matchNum(int* start, int* forward)
{
	bool matched = false;
	while (*forward < sourceCode.length() && in(sourceCode.at(*forward), std::string("0123456789")))
	{
		matched = true;
		if (sourceCode.at(*forward) == '\n')
			lineNum++;
		(*forward)++;
	}

	if (*forward < sourceCode.length() && !(sourceCode.at(*forward) == '.') || *forward >= sourceCode.length())
	{
		(*forward)++;
		goto MATCHED;
	}

	matched = false;
	(*forward)++;
	while (*forward < sourceCode.length() && in(sourceCode.at(*forward), std::string("0123456789")))
	{
		matched = true;
		if (sourceCode.at(*forward) == '\n')
			lineNum++;
		(*forward)++;
	}

MATCHED:
	if (!matched)
		return false;

	(*forward)--;
	return true;
}

//if val is in set return true;
bool Lexer::in(char val, std::string set)
{
	for (char c : set)
	{
		if (c == val)
			return true;
	}
	return false;
}

void Lexer::error(int* start, int* forward)
{

	(*forward)++;
	printf("Error on line %d\n", lineNum);
}

bool Lexer::matchString(int* start, int* forward)
{
	if (!(sourceCode.at(*forward) == '\"'))
		return false;
	else
		(*forward)++;

	while (*forward < sourceCode.size() && sourceCode.at(*forward) != '\"')
		(*forward)++;

	return sourceCode.at(*forward) == '\"';
}

//lexes out a keyword returns true when works and false on failure
bool Lexer::keywordReader(std::string keyword, TokenType type, int* start, int* forward)
{
	if (sourceCode.substr(*start, keyword.length()) == keyword)
	{
		*forward += keyword.length();
		createToken(type, start, forward);
		return true;
	}

	return false;
}

//Match an identifier, then check if it's a reserve word
bool Lexer::matchIdentifierKeyword(int* start, int* forward)
{
	if (!in(sourceCode.at(*forward), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
		return false;
	(*forward)++;

	while (in(sourceCode.at(*forward), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789"))
		(*forward)++;

	if (sourceCode.substr(*start, *forward - *start) == "class")
		createToken(CLASS, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "else")
		createToken(ELSE, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "false")
		createToken(FALSE, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "for")
		createToken(FOR, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "fun")
		createToken(FUN, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "if")
		createToken(IF, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "nil")
		createToken(NIL, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "or")
		createToken(OR, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "print") //TODO make a function that takes in the keyword string, checks if the next strlen(keyword) characters are the keyword, then sets forward correctly. pass the keyword and the token type
		createToken(PRINT, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "return")
		createToken(RETURN, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "super")
		createToken(SUPER, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "this")
		createToken(THIS, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "true")
		createToken(TRUE, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "var")
		createToken(VAR, start, forward);
	else if (sourceCode.substr(*start, *forward - *start) == "while")
		createToken(WHILE, start, forward);
	else
		createToken(IDENTIFIER, start, forward);
}

void Lexer::resetForward(int* start, int* forward)
{
	*forward = *start;
}

std::vector<Token*> Lexer::getLexemesRef()
{
	std::vector<Token*> references;
	for(int i=0; i<lexemes.size(); i++)
		references.push_back(&lexemes[i]);

	return references;
}