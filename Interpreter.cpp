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
		left = *(double *) traverseExpr(binary->left);
	}
	if (binary->op)
	{
		op = binary->op->getLexeme();
	}
	if (binary->right)
	{
		right = *(double*) traverseExpr(binary->right);
	}

	if (op == "*")
	{
		return left * right;
	}
	if (op == "/")
	{
		return left / right;
	}
	if (op == "+")
	{
		return left + right;
	}
	if (op == "-")
	{
		return left - right;
	}
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

/*void traverseUnary(Unary* unary)
{
	if (unary->op)
	{
		printf("%s ", unary->op->getLexeme().c_str());
	}
	if (unary->expr)
	{
		traverseTree(unary->expr);
	}
}

void traverseBinary(Binary* binary)
{
	if (binary->left)
	{
		traverseTree(binary->left);
	}
	if (binary->op)
	{
		printf("%s ", binary->op->getLexeme().c_str());
	}
	if (binary->right)
	{
		traverseTree(binary->right);
	}
}

void traverseLiteral(Literal* lit)
{
	if (lit->literal)
	{
		printf("%s ", lit->literal->getLexeme().c_str());
	}
}

void traverseGrouping(Grouping* grouping)
{
	if (grouping->leftParen)
	{
		printf("%s ", grouping->leftParen->getLexeme().c_str());
	}
	if (grouping->expr)
	{
		traverseTree(grouping->expr);
	}
	if (grouping->rightParen)
	{
		printf("%s ", grouping->rightParen->getLexeme().c_str());
	}
}

void traverseTree(Expr* expr)
{
	if (expr->unary)
	{
		traverseUnary(expr->unary);
	}
	if (expr->binary)
	{
		traverseBinary(expr->binary);
	}
	if (expr->literal)
	{
		traverseLiteral(expr->literal);
	}
	if (expr->grouping)
	{
		traverseGrouping(expr->grouping);
	}
}*/