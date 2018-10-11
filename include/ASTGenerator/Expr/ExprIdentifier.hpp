/*
See LICENSE file in root folder
*/
#ifndef ___AST_ExprIdentifier_H___
#define ___AST_ExprIdentifier_H___
#pragma once

#include "Expr.hpp"

namespace ast::expr
{
	class Identifier
		: public Expr
	{
	public:
		Identifier( var::VariablePtr var );

		void accept( VisitorPtr vis )override;

		inline var::VariablePtr getVariable()const
		{
			return m_var;
		}

	private:
		var::VariablePtr m_var;
	};
	using IdentifierPtr = std::unique_ptr< Identifier >;

	inline IdentifierPtr makeIdentifier( var::VariablePtr var )
	{
		return std::make_unique< Identifier >( std::move( var ) );
	}
}

#endif
