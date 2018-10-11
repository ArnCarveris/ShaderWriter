/*
See LICENSE file in root folder
*/
#include "ASTGenerator/Expr/ExprDivide.hpp"

#include "ASTGenerator/Expr/ExprVisitor.hpp"

namespace ast::expr
{
	Divide::Divide( type::TypePtr type
		, ExprPtr lhs
		, ExprPtr rhs )
		: Binary{ std::move( type )
			, std::move( lhs )
			, std::move( rhs )
			, Kind::eDivide }
	{
	}

	void Divide::accept( VisitorPtr vis )
	{
		vis->visitDivideExpr( this );
	}
}
