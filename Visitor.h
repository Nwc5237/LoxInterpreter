#pragma once
#include "Expr.h"

//we want this to be an abstract class eventually
class ExprVisitor
{
public:
	void visitUnary(Unary unary);
	void visitBinary(Binary binary);
	void visitLiteral(Literal literal);
	void visitGrouping(Grouping grouping);
};

//then an action will be a class like this that implements expression visitor
//and we only have to implement 
class Print : public ExprVisitor
{
	//to see why this is good design try to imagine ADDING a new type, and then adding a new
	//function. Think about how that works with this, vs how it would work without it (in functional and oop)
	void visitUnary(Unary unary)
	{

	}

	void visitBinary(Binary binary)
	{

	}
};
