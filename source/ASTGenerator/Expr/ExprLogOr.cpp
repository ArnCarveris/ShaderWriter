/*
See LICENSE file in root folder
*/
#include "ASTGenerator/Expr/ExprLogOr.hpp"

#include "ASTGenerator/Expr/ExprVisitor.hpp"

namespace ast::expr
{
	LogOr::LogOr( ExprPtr lhs
		, ExprPtr rhs )
		: Binary{ std::move( type::getBool() )
			, std::move( lhs )
			, std::move( rhs )
			, Kind::eLogOr }
	{
	}

	void LogOr::accept( VisitorPtr vis )
	{
		vis->visitLogOrExpr( this );
	}
}
