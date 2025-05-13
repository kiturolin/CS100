#ifndef EXPR_TYPE_CHECK_HPP
#define EXPR_TYPE_CHECK_HPP

#include "expr.hpp"

class TypeCheckVisitor : public ExprNodeVisitor
{
private:
  ExprType mCheckedType;

public:
  TypeCheckVisitor () : mCheckedType (ExprType::Unevaluated) {}

  ExprType getDeductedType () { return mCheckedType; }

  void visit (IntegerConstant &) override { mCheckedType = ExprType::Integer; }

  void visit (FloatConstant &) override { mCheckedType = ExprType::Float; }

  void visit (BooleanConstant &) override { mCheckedType = ExprType::Boolean; }

  void visit (Variable &x) override { mCheckedType = x.getType (); }

  void visit (UnaryArithOp &x) override
  {
    x.getOperand ().accept (*this);
    ExprType operandType = mCheckedType;
    if (operandType != ExprType::Integer && operandType != ExprType::Float) mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }

  void visit (BinaryArithOp &x) override
  {
    x.getLHS ().accept (*this);
    ExprType LHSType = mCheckedType;
    x.getRHS ().accept (*this);
    ExprType RHSType = mCheckedType;

    if (LHSType == ExprType::ErrorType || RHSType == ExprType::ErrorType) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (LHSType == ExprType::Boolean || RHSType == ExprType::Boolean) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (LHSType == ExprType::Float || RHSType == ExprType::Float) {
      mCheckedType = ExprType::Float;
    } else {
      mCheckedType = ExprType::Integer;
    }
    x.setType (mCheckedType);
  }

  void visit (EqualityOp &x) override
  {
    x.getLHS ().accept (*this);
    ExprType LHSType = mCheckedType;
    x.getRHS ().accept (*this);
    ExprType RHSType = mCheckedType;

    if (LHSType == ExprType::ErrorType || RHSType == ExprType::ErrorType) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (LHSType == RHSType) {
      mCheckedType = ExprType::Boolean;
      x.setType (mCheckedType);
      return;
    }

    if (LHSType != ExprType::Boolean && RHSType != ExprType::Boolean) {
      mCheckedType = ExprType::Boolean;
      x.setType (mCheckedType);
      return;
    }

    mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }

  void visit (RelationalOp &x) override
  {
    x.getLHS ().accept (*this);
    ExprType LHSType = mCheckedType;
    x.getRHS ().accept (*this);
    ExprType RHSType = mCheckedType;

    if (LHSType == ExprType::ErrorType || RHSType == ExprType::ErrorType) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (LHSType != ExprType::Boolean && RHSType != ExprType::Boolean) {
      mCheckedType = ExprType::Boolean;
      x.setType (mCheckedType);
      return;
    }

    mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }

  void visit (BinaryLogicalOp &x) override
  {
    x.getLHS ().accept (*this);
    ExprType LHSType = mCheckedType;
    x.getRHS ().accept (*this);
    ExprType RHSType = mCheckedType;

    if (LHSType == ExprType::Boolean && LHSType == RHSType) {
      mCheckedType = ExprType::Boolean;
      x.setType (mCheckedType);
      return;
    }
    mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }

  void visit (LogicalNotOp &x) override
  {
    x.getOperand ().accept (*this);
    ExprType operandType = mCheckedType;
    if (operandType != ExprType::Boolean) mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }

  void visit (ConditionalOp &x) override
  {
    x.getCondition ().accept (*this);
    ExprType condType = mCheckedType;
    x.getExprTrue ().accept (*this);
    ExprType trueType = mCheckedType;
    x.getExprFalse ().accept (*this);
    ExprType falseType = mCheckedType;

    if (condType != ExprType::Boolean) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (trueType == ExprType::ErrorType || falseType == ExprType::ErrorType) {
      mCheckedType = ExprType::ErrorType;
      x.setType (mCheckedType);
      return;
    }

    if (trueType == falseType) {
      mCheckedType = trueType;
      x.setType (mCheckedType);
      return;
    }

    if (trueType != ExprType::Boolean && falseType != ExprType::Boolean) {
      if (trueType == ExprType::Integer && falseType == ExprType::Integer) {
	mCheckedType = ExprType::Integer;
	x.setType (mCheckedType);
	return;
      } else {
	mCheckedType = ExprType::Float;
	x.setType (mCheckedType);
	return;
      }
    }

    mCheckedType = ExprType::ErrorType;
    x.setType (mCheckedType);
  }
};

/// @brief Performs type checking on the expression @c expr, including all of
/// its subexpressions.
/// @param expr The expression, which is the root of the AST.
void
typeCheck (Expr &expr)
{
  TypeCheckVisitor typeChecker;
  expr.accept (typeChecker);
  expr.setType (typeChecker.getDeductedType ());
};

#endif	  // EXPR_TYPE_CHECK_HPP
