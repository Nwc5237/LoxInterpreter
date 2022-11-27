#include "Main.h"

//a lexeme is an instance of a token
std::vector<Token> lexemes;

//the program source
std::string sourceCode;

/*
Working expressions
123 / 4 * 5 + 6
2 + 3 * 4 - 1
2+3*4-1 //fixed lexer so that these get lexed correctly without spaces now

Failing expressions
123 / 4 * (5 + 6) //because we're not parsing groupings yet

*/

int main()
{
	Lexer lexer(std::string("=><! >=<=!====")); //looks like we have an error lexing with 
	lexer.lex();

	for (Token t : lexer.getLexemes())
	{
		printf("%s\n", t.getLexeme().c_str());
	}

	Parser parser(lexer.getLexemesRef());
	Expr* expr = parser.expression();
	double x = *(double *) traverseExpr(expr);
	printf("Oh dear God\n");
}