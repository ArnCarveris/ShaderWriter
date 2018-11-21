/*
See LICENSE file in root folder
*/
#include "ShaderWriter/HLSL/HlslExprVisitor.hpp"

#include "ShaderWriter/HLSL/HlslImageAccessNames.hpp"
#include "ShaderWriter/HLSL/HlslIntrinsicNames.hpp"
#include "ShaderWriter/HLSL/HlslTextureAccessNames.hpp"

#include <sstream>

namespace sdw::hlsl
{
	std::string ExprVisitor::submit( expr::Expr * expr )
	{
		std::string result;
		ExprVisitor vis{ result };
		expr->accept( &vis );
		return result;
	}

	ExprVisitor::ExprVisitor( std::string & result )
		: m_result{ result }
	{
	}

	void ExprVisitor::wrap( expr::Expr * expr )
	{
		bool noParen = expr->getKind() == expr::Kind::eFnCall
			|| expr->getKind() == expr::Kind::eIdentifier
			|| expr->getKind() == expr::Kind::eLiteral
			|| expr->getKind() == expr::Kind::eMbrSelect
			|| expr->getKind() == expr::Kind::eCast
			|| expr->getKind() == expr::Kind::eSwizzle;

		if ( noParen )
		{
			expr->accept( this );
		}
		else
		{
			m_result += "(";
			expr->accept( this );
			m_result += ")";
		}
	}

	void ExprVisitor::visitAssignmentExpr( expr::Binary * expr )
	{
		wrap( expr->getLHS() );
		m_result += " " + getOperatorName( expr->getKind() ) + " ";
		expr->getRHS()->accept( this );
	}

	void ExprVisitor::visitUnaryExpr( expr::Unary * expr )
	{
		if ( isUnaryPre( expr->getKind() ) )
		{
			m_result += getOperatorName( expr->getKind() );
			wrap( expr->getOperand() );
		}
		else
		{
			wrap( expr->getOperand() );
			m_result += getOperatorName( expr->getKind() );
		}
	}

	void ExprVisitor::visitBinaryExpr( expr::Binary * expr )
	{
		wrap( expr->getLHS() );
		m_result += " " + getOperatorName( expr->getKind() ) + " ";
		wrap( expr->getRHS() );
	}

	void ExprVisitor::visitAddAssignExpr( expr::AddAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitAndAssignExpr( expr::AndAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitAssignExpr( expr::Assign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitDivideAssignExpr( expr::DivideAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitLShiftAssignExpr( expr::LShiftAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitMinusAssignExpr( expr::MinusAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitModuloAssignExpr( expr::ModuloAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitOrAssignExpr( expr::OrAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitRShiftAssignExpr( expr::RShiftAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitTimesAssignExpr( expr::TimesAssign * expr )
	{
		visitAssignmentExpr( expr );
	}
	void ExprVisitor::visitXorAssignExpr( expr::XorAssign * expr )
	{
		visitAssignmentExpr( expr );
	}

	void ExprVisitor::visitAggrInitExpr( expr::AggrInit * expr )
	{
		if ( expr->getIdentifier() )
		{
			m_result += getTypeName( expr->getType() ) + " ";
			expr->getIdentifier()->accept( this );
			m_result += getTypeArraySize( expr->getIdentifier()->getType() );
			m_result += " = ";
		}
		else
		{
			m_result += getTypeName( expr->getType() );
			m_result += getTypeArraySize( expr->getType() );
		}

		m_result += "{";
		std::string sep;

		for ( auto & init : expr->getInitialisers() )
		{
			m_result += sep + submit( init.get() );
			sep = ", ";
		}

		m_result += "}";
	}

	void ExprVisitor::visitArrayAccessExpr( expr::ArrayAccess * expr )
	{
		wrap( expr->getLHS() );
		m_result += "[";
		wrap( expr->getRHS() );
		m_result += "]";
	}

	void ExprVisitor::visitCastExpr( expr::Cast * expr )
	{
		m_result += getTypeName( expr->getType() ) + "(";
		expr->getOperand()->accept( this );
		m_result += ")";
	}

	void ExprVisitor::visitCompositeConstructExpr( expr::CompositeConstruct * expr )
	{
		m_result += getCtorName( expr->getComposite(), expr->getComponent() ) + "(";
		std::string sep;

		for ( auto & arg : expr->getArgList() )
		{
			m_result += sep;
			arg->accept( this );
			sep = ", ";
		}

		m_result += ")";
	}

	void ExprVisitor::visitMbrSelectExpr( expr::MbrSelect * expr )
	{
		wrap( expr->getOuterExpr() );
		m_result += ".";
		expr->getOperand()->accept( this );
	}

	void ExprVisitor::visitFnCallExpr( expr::FnCall * expr )
	{
		if ( expr->isMember() )
		{
			wrap( expr->getInstance() );
			m_result += ".";
		}

		expr->getFn()->accept( this );
		m_result += "(";
		std::string sep;

		for ( auto & arg : expr->getArgList() )
		{
			m_result += sep;
			arg->accept( this );
			sep = ", ";
		}

		m_result += ")";
	}

	void ExprVisitor::visitIdentifierExpr( expr::Identifier * expr )
	{
		//if ( expr->getVariable()->isMember() 
		//	&& getOutermost( expr->getVariable() )->isUniform() )
		//{
		//	auto result = expr->getVariable()->getName();
		//	auto outer = expr->getVariable()->getOuter();

		//	while ( outer )
		//	{
		//		result = outer->getName() + "." + result;
		//		outer = outer->getOuter();
		//	}

		//	m_result += result;
		//}
		//else
		{
			m_result += expr->getVariable()->getName();
		}
	}

	void ExprVisitor::visitImageAccessCallExpr( expr::ImageAccessCall * expr )
	{
		if ( expr->getImageAccess() < expr::ImageAccess::eImageLoad1DF
			|| expr->getImageAccess() > expr::ImageAccess::eImageLoad2DMSArrayU )
		{
			m_result += getHlslName( expr->getImageAccess() ) + "(";
			std::string sep;

			for ( auto & arg : expr->getArgList() )
			{
				m_result += sep;
				arg->accept( this );
				sep = ", ";
			}

			m_result += ")";
		}
		else
		{
			expr->getArgList()[0]->accept( this );
			m_result += "." + getHlslName( expr->getImageAccess() ) + "(";
			expr->getArgList()[1]->accept( this );

			for ( size_t i = 2; i < expr->getArgList().size(); ++i )
			{
				auto & arg = expr->getArgList()[i];
				m_result += ", ";
				arg->accept( this );
			}

			m_result += ")";
		}
	}

	void ExprVisitor::visitInitExpr( expr::Init * expr )
	{
		m_result += getTypeName( expr->getType() ) + " ";
		expr->getIdentifier()->accept( this );
		m_result += getTypeArraySize( expr->getIdentifier()->getType() );
		m_result += " = ";
		expr->getInitialiser()->accept( this );
	}

	void ExprVisitor::visitIntrinsicCallExpr( expr::IntrinsicCall * expr )
	{
		m_result += getHlslName( expr->getIntrinsic() ) + "(";
		std::string sep;

		for ( auto & arg : expr->getArgList() )
		{
			m_result += sep;
			arg->accept( this );
			sep = ", ";
		}

		m_result += ")";
	}

	void ExprVisitor::visitLiteralExpr( expr::Literal * expr )
	{
		std::locale loc{ "C" };
		std::stringstream stream;
		stream.imbue( loc );

		switch ( expr->getLiteralType() )
		{
		case expr::LiteralType::eBool:
			stream << ( expr->getValue< expr::LiteralType::eBool >()
				? std::string{ "true" }
			: std::string{ "false" } );
			break;
		case expr::LiteralType::eInt:
			stream << expr->getValue< expr::LiteralType::eInt >();
			break;
		case expr::LiteralType::eUInt:
			stream << expr->getValue< expr::LiteralType::eUInt >() << "u";
			break;
		case expr::LiteralType::eFloat:
			{
				auto v = expr->getValue< expr::LiteralType::eFloat >();
				stream << v;

				if ( v == int64_t( v ) )
				{
					stream << ".0";
				}
			}
			break;
		case expr::LiteralType::eDouble:
			{
				auto v = expr->getValue< expr::LiteralType::eDouble >();
				stream << v;

				if ( v == int64_t( v ) )
				{
					stream << ".0";
				}
			}
			break;
		default:
			assert( false && "Unsupported literal type" );
			break;
		}

		m_result += stream.str();
	}

	void ExprVisitor::visitQuestionExpr( expr::Question *expr )
	{
		m_result += "(";
		wrap( expr->getCtrlExpr() );
		m_result += " ? ";
		wrap( expr->getTrueExpr() );
		m_result += " : ";
		wrap( expr->getFalseExpr() );
		m_result += ")";
	}

	void ExprVisitor::visitSwitchCaseExpr( expr::SwitchCase *expr )
	{
		expr->getLabel()->accept( this );
	}

	void ExprVisitor::visitSwitchTestExpr( expr::SwitchTest *expr )
	{
		expr->getValue()->accept( this );
	}

	void ExprVisitor::visitSwizzleExpr( expr::Swizzle * expr )
	{
		wrap( expr->getOuterExpr() );
		m_result += "." + getName( expr->getSwizzle() );
	}

	void ExprVisitor::visitTextureAccessCallExpr( expr::TextureAccessCall * expr )
	{
		if ( expr->getTextureAccess() >= expr::TextureAccess::eTextureSize1DF
				&& expr->getTextureAccess() <= expr::TextureAccess::eTextureQueryLevelsCubeArrayU )
		{
			doProcessNonMemberTexture( expr );
		}
		else if ( expr->getTextureAccess() >= expr::TextureAccess::eTextureGather2DF
				&& expr->getTextureAccess() <= expr::TextureAccess::eTextureGatherOffsets2DRectUComp )
		{
			doProcessTextureGather( expr );
		}
		else
		{
			doProcessMemberTexture( expr );
		}
	}

	void ExprVisitor::doProcessMemberTexture( expr::TextureAccessCall * expr )
	{
		expr->getArgList()[0]->accept( this );
		m_result += "." + getHlslName( expr->getTextureAccess() ) + "(";
		expr->getArgList()[1]->accept( this );

		for ( size_t i = 2; i < expr->getArgList().size(); ++i )
		{
			auto & arg = expr->getArgList()[i];
			m_result += ", ";
			arg->accept( this );
		}

		m_result += ")";
	}

	void ExprVisitor::doProcessNonMemberTexture( expr::TextureAccessCall * expr )
	{
		m_result += getHlslName( expr->getTextureAccess() ) + "(";
		std::string sep;

		for ( auto & arg : expr->getArgList() )
		{
			m_result += sep;
			arg->accept( this );
			sep = ", ";
		}

		m_result += ")";
	}

	void ExprVisitor::doProcessTextureGather( expr::TextureAccessCall * expr )
	{
		expr->getArgList()[0]->accept( this );
		uint32_t compValue = 0u;

		// Component value will determine Gather function name.
		auto component = expr->getArgList().back().get();

		if ( component->getKind() == expr::Kind::eLiteral )
		{
			auto lit = static_cast< expr::Literal const * >( component );

			if ( lit->getLiteralType() == expr::LiteralType::eInt )
			{
				compValue = uint32_t( lit->getValue< expr::LiteralType::eInt >() );
			}
			else
			{
				assert( lit->getLiteralType() == expr::LiteralType::eUInt );
				compValue = lit->getValue< expr::LiteralType::eUInt >();
			}
		}

		auto name = getHlslName( expr->getTextureAccess() );

		switch ( compValue )
		{
		case 0:
			name = "GatherRed";
			break;
		case 1:
			name = "GatherGreen";
			break;
		case 2:
			name = "GatherBlue";
			break;
		case 3:
			name = "GatherAlpha";
			break;
		}

		m_result += "." + name + "(";
		// Sampler
		expr->getArgList()[1]->accept( this );
		// Coord
		m_result += ", ";
		expr->getArgList()[2]->accept( this );

		for ( size_t i = 3u; i < expr->getArgList().size() - 1u; ++i )
		{
			auto & arg = expr->getArgList()[i];
			m_result += ", ";
			arg->accept( this );
		}

		m_result += ")";
	}
}
