#include "Interpreter.h"

double traverseUnary(Unary* unary)
{
	std::string op;
	double val;

	if (unary->op)
	{
		op = unary->op->getLexeme();
	}
	if (unary->expr)
	{
		val = *(double *) traverseExpr(unary->expr);
	}

	if (op == "-")
	{
		return -val;
	}

	if (op == "!")
	{
		return !val;
	}

	printf("Bad unary while traversing\n");
	exit(-1);
}

double traverseBinary(Binary* binary)
{
	std::string op;
	double left, right;

	if (binary->left)
	{
		left = *(double*)traverseExpr(binary->left);
	}
	else
		goto BINARY_ERROR;

	if (binary->op)
	{
		op = binary->op->getLexeme();
	}
	else
		goto BINARY_ERROR;

	if (binary->right)
	{
		right = *(double*) traverseExpr(binary->right);
	}
	else
		goto BINARY_ERROR;



	if (op == "*")
	{
		return left * right;
	}
	else if (op == "/")
	{
		return left / right;
	}
	else if (op == "+")
	{
		return left + right;
	}
	else if (op == "-")
	{
		return left - right;
	}

	else if (op == ">")
	{
		return left > right;
	}

	else if (op == "<")
	{
		return left < right;
	}
	else if (op == ">=")
	{
		return left >= right;
	}
	else if (op == "<=")
	{
		return left <= right;
	}

	else if (op == "!=") //might want to actually update this one with epsilon
	{
		return left != right;
	}
	else if (op == "==") //epsilon here as well
	{
		return left == right;
	}

BINARY_ERROR:
	printf("Error evaluating binary expression.");
	exit(-1);
}

double traverseLiteral(Literal* lit)
{
	if (lit->literal)
	{
		return stod(lit->literal->getLexeme());
	}
}

double traverseGrouping(Grouping* grouping)
{
	if (grouping->leftParen && grouping->rightParen)
	{
		if (grouping->expr)
		{
			return *(double *) traverseExpr(grouping->expr);
		}
	}
}

void *traverseExpr(Expr* expr)
{
	void *exprVal = nullptr;
	if (expr->unary)
	{
		exprVal = malloc(sizeof(double));
		*(double *)exprVal = traverseUnary(expr->unary);
	}
	if (expr->binary)
	{
		exprVal = malloc(sizeof(double));
		*(double*)exprVal = traverseBinary(expr->binary);
	}
	if (expr->literal)
	{
		exprVal = malloc(sizeof(double));
		*(double*)exprVal = traverseLiteral(expr->literal);
	}
	if (expr->grouping)
	{
		exprVal = malloc(sizeof(double));
		*(double*)exprVal = traverseGrouping(expr->grouping);
	}

	return exprVal;
}

void traversePrint(PrintStmt* stmt)
{
	if (stmt->printToken && stmt->expr && stmt->semicolon)
	{
		printf("%f\n", *(double *) traverseExpr(stmt->expr));
	}
}