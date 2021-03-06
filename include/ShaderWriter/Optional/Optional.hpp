/*
See LICENSE file in root folder
*/
#ifndef ___SDW_Optional_H___
#define ___SDW_Optional_H___
#pragma once

#include "ShaderWriter/Value.hpp"

namespace sdw
{
	template< typename ValueT >
	struct Optional
		: public ValueT
	{
		inline Optional( Shader * shader
			, expr::ExprPtr expr
			, bool enabled );
		inline Optional( ValueT const & other
			, bool enabled );
		inline Optional( Optional const & rhs );

		inline Optional< ValueT > & operator=( Optional< ValueT > const & rhs );
		template< typename T >
		inline Optional< ValueT > & operator=( T const & rhs );
		inline bool isEnabled()const;

		template< typename T >
		Optional< ValueT > & operator+=( T const & rhs );
		template< typename T >
		Optional< ValueT > & operator-=( T const & rhs );
		template< typename T >
		Optional< ValueT > & operator*=( T const & rhs );
		template< typename T >
		Optional< ValueT > & operator/=( T const & rhs );
		Optional< ValueT > operator-()const;
		Optional< ValueT > operator+()const;

	private:
		bool m_enabled;
	};

	template< typename T >
	struct IsOptional
		: public std::false_type
	{
	};

	template< typename T >
	struct IsOptional< Optional< T > >
		: public std::true_type
	{
	};

	template< typename T >
	static bool constexpr isOptional = IsOptional< T >::value;

	template< typename T >
	inline expr::ExprPtr makeExpr( Shader & shader
		, Optional< T > const & value
		, bool force = true );

	template< typename T >
	inline bool isOptionalEnabled( T const & value );

	template< typename ... ParamsT >
	inline bool areOptionalEnabled( ParamsT const & ... values );

	template< typename ReturnT, typename OperandT, typename CreatorT >
	inline Optional< ReturnT > writeUnOperator( Optional< OperandT > const & operand
		, CreatorT creator );

	template< typename ReturnT, typename LhsT, typename RhsT, typename CreatorT >
	inline Optional< ReturnT > writeBinOperator( Optional< LhsT > const & lhs
		, RhsT const & rhs
		, CreatorT creator );
	template< typename ReturnT, typename LhsT, typename RhsT, typename CreatorT >
	inline Optional< ReturnT > writeBinOperator( LhsT const & lhs
		, Optional< RhsT > const & rhs
		, CreatorT creator );
	template< typename ReturnT, typename LhsT, typename RhsT, typename CreatorT >
	inline Optional< ReturnT > writeBinOperator( Optional< LhsT > const & lhs
		, Optional< RhsT > const & rhs
		, CreatorT creator );

	template< typename ReturnT, typename LhsT, typename RhsT, typename CreatorT >
	inline void writeAssignOperator( LhsT const & lhs
		, RhsT const & rhs
		, CreatorT creator );
}

#include "Optional.inl"

#endif
