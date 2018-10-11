/*
See LICENSE file in root folder
*/
#ifndef ___AST_StmtDoWhile_H___
#define ___AST_StmtDoWhile_H___
#pragma once

#include "StmtCompound.hpp"

#include "ASTGenerator/Expr/Expr.hpp"

namespace ast::stmt
{
	class DoWhile
		: public Compound
	{
	public:
		DoWhile( expr::ExprPtr ctrlExpr );

		void accept( VisitorPtr vis )override;

		inline expr::Expr * getCtrlExpr()const
		{
			return m_ctrlExpr.get();
		}

	private:
		expr::ExprPtr m_ctrlExpr;
	};
	using DoWhilePtr = std::unique_ptr< DoWhile >;

	inline DoWhilePtr makeDoWhile( expr::ExprPtr ctrlExpr )
	{
		return std::make_unique< DoWhile >( std::move( ctrlExpr ) );
	}
}

#endif
