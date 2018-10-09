/*
See LICENSE file in root folder
*/
#include "ASTGenerator/Expr/Expr.hpp"

namespace ast
{
	Expr::Expr( Type type, Kind kind )
		: m_kind{ kind }
		, m_type{ type }
	{
	}

	Expr::~Expr()
	{
	}
}
