/*
See LICENSE file in root folder
*/
#include "ShaderAST/Expr/ExprGreater.hpp"
#include "ShaderAST/Type/Type.hpp"

#include "ShaderAST/Expr/ExprVisitor.hpp"

namespace ast::expr
{
	Greater::Greater( ExprPtr lhs
		, ExprPtr rhs )
		: Binary{ std::move( type::getBool() )
			, std::move( lhs )
			, std::move( rhs )
			, Kind::eGreater }
	{
	}

	void Greater::accept( VisitorPtr vis )
	{
		vis->visitGreaterExpr( this );
	}
}