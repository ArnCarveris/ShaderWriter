/*
See LICENSE file in root folder
*/
namespace sdw
{
	template< typename ValueT >
	Mat3x2T< ValueT >::Mat3x2T( Shader * shader
		, expr::ExprPtr expr )
		: Value{ shader, std::move( expr ) }
	{
	}

	template< typename ValueT >
	Mat3x2T< ValueT >::Mat3x2T( Mat3x2T const & rhs )
		: Value{ rhs.getShader(), makeExpr( rhs ) }
	{
	}

	template< typename ValueT >
	Mat3x2T< ValueT > & Mat3x2T< ValueT >::operator=( Mat3x2T< ValueT > const & rhs )
	{
		if ( this->getContainer() )
		{
			addStmt( *findContainer( *this, rhs )
				, sdw::makeSimple( sdw::makeAssign( Mat3x2T< ValueT >::makeType()
					, makeExpr( *this )
					, makeExpr( rhs ) ) ) );
		}
		else
		{
			Value::operator=( rhs );
		}

		return *this;
	}

	template< typename ValueT >
	template< typename RhsT >
	Mat3x2T< ValueT > & Mat3x2T< ValueT >::operator=( RhsT const & rhs )
	{
		updateContainer( rhs );
		addStmt( *findContainer( *this, rhs )
			, sdw::makeSimple( sdw::makeAssign( Mat3x2T< ValueT >::makeType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	template< typename IndexT >
	Vec2T< ValueT > Mat3x2T< ValueT >::operator[]( IndexT const & rhs )const
	{
		return Vec2T< ValueT >{ findShader( *this, rhs )
			, sdw::makeArrayAccess( Vec2T< ValueT >::makeType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	inline ast::type::TypePtr Mat3x2T< ValueT >::makeType()
	{
		return sdw::makeType< Mat3x2T< ValueT > >();
	}
}
