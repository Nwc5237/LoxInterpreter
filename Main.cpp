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
	Lexer lexer(std::string("print 3*4+8;"));
	lexer.lex();

	Parser parser(lexer.getLexemesRef());
	//Expr* expr = parser.expression();
	//double x = *(double *) traverseExpr(expr);
	traversePrint(parser.printStmt());
	printf("Oh dear God\n");
}