#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <string>

class ExprNode
{
public:
  ExprNode () = default;

  virtual double evaluate (double x) const = 0;

  virtual double derivative (double x) const = 0;

  virtual std::string toString () const = 0;

  virtual ~ExprNode () = default;
};

class Expr
{
  friend Expr operator+ (const Expr &arg);
  friend Expr operator- (const Expr &arg);
  friend Expr operator+ (const Expr &lhs, const Expr &rhs);
  friend Expr operator- (const Expr &lhs, const Expr &rhs);
  friend Expr operator* (const Expr &lhs, const Expr &rhs);
  friend Expr operator/ (const Expr &lhs, const Expr &rhs);

  std::shared_ptr<ExprNode> mNode;

  Expr (std::shared_ptr<ExprNode> ptr) : mNode{ std::move (ptr) } {}

public:
  Expr (double);

  Expr (const Expr &other) = default;

  Expr (Expr &&other) noexcept = default;

  Expr &operator= (const Expr &other) = default;

  Expr &operator= (Expr &&other) noexcept = default;

  double evaluate (double x) const { return mNode->evaluate (x); };

  double derivative (double x) const { return mNode->derivative (x); }

  std::string toString () const { return mNode->toString (); }

  static const Expr x;
};

class Variable : public ExprNode
{
public:
  double evaluate (double x) const override { return x; }

  double derivative (double) const override { return 1.0; }

  std::string toString () const override { return std::string{ "x" }; }
};

const Expr Expr::x{ std::make_shared<Variable> () };

class Constant : public ExprNode
{
  double mValue;

public:
  explicit Constant (double value) : mValue{ value } {}

  double evaluate (double) const override { return mValue; }

  double derivative (double) const override { return 0.0; }

  std::string toString () const override { return std::to_string (mValue); }
};

Expr::Expr (double value) : mNode{ std::make_shared<Constant> (value) } {}

enum class UnaryOpKind { UOK_Plus, UOK_Minus };

class UnaryOperator : public ExprNode
{
  UnaryOpKind mOpKind;
  Expr mOperand;

public:
  UnaryOperator (UnaryOpKind op, Expr operand) : mOpKind{ op }, mOperand{ std::move (operand) } {}

  double evaluate (double x) const override
  {
    return mOpKind == UnaryOpKind::UOK_Plus ? mOperand.evaluate (x) : -mOperand.evaluate (x);
  }

  double derivative (double x) const override
  {
    return mOpKind == UnaryOpKind::UOK_Plus ? mOperand.derivative (x) : -mOperand.derivative (x);
  }

  std::string toString () const override
  {
    return mOpKind == UnaryOpKind::UOK_Plus ? mOperand.toString () : "-(" + mOperand.toString () + ")";
  }
};

class BinaryOperator : public ExprNode
{
protected:
  Expr mLeft;
  Expr mRight;

public:
  BinaryOperator (Expr left, Expr right) : mLeft{ std::move (left) }, mRight{ std::move (right) } {}
};

class PlusOp : public BinaryOperator
{
public:
  using BinaryOperator::BinaryOperator;

  double evaluate (double x) const override { return mLeft.evaluate (x) + mRight.evaluate (x); }

  double derivative (double x) const override { return mLeft.derivative (x) + mRight.derivative (x); }

  std::string toString () const override
  {
    return "(" + mLeft.toString () + ")" + " + " + "(" + mRight.toString () + ")";
  }
};

class MinusOp : public BinaryOperator
{
public:
  using BinaryOperator::BinaryOperator;

  double evaluate (double x) const override { return mLeft.evaluate (x) - mRight.evaluate (x); }

  double derivative (double x) const override { return mLeft.derivative (x) - mRight.derivative (x); }

  std::string toString () const override
  {
    return "(" + mLeft.toString () + ")" + " - " + "(" + mRight.toString () + ")";
  }
};

class MultiplyOp : public BinaryOperator
{
public:
  using BinaryOperator::BinaryOperator;

  double evaluate (double x) const override { return mLeft.evaluate (x) * mRight.evaluate (x); }

  double derivative (double x) const override
  {
    return mLeft.derivative (x) * mRight.evaluate (x) + mLeft.evaluate (x) * mRight.derivative (x);
  }

  std::string toString () const override
  {
    return "(" + mLeft.toString () + ")" + " * " + "(" + mRight.toString () + ")";
  }
};

class DivideOp : public BinaryOperator
{
public:
  using BinaryOperator::BinaryOperator;

  double evaluate (double x) const override { return mLeft.evaluate (x) / mRight.evaluate (x); }

  double derivative (double x) const override
  {
    return (mLeft.derivative (x) * mRight.evaluate (x) - mLeft.evaluate (x) * mRight.derivative (x))
	   / (mRight.evaluate (x) * mRight.evaluate (x));
  }

  std::string toString () const override
  {
    return "(" + mLeft.toString () + ")" + " / " + "(" + mRight.toString () + ")";
  }
};

Expr
operator+ (const Expr &arg)
{
  return { std::make_shared<UnaryOperator> (UnaryOpKind::UOK_Plus, arg) };
}

Expr
operator- (const Expr &arg)
{
  return { std::make_shared<UnaryOperator> (UnaryOpKind::UOK_Minus, arg) };
}

Expr
operator+ (const Expr &lhs, const Expr &rhs)
{
  return { std::make_shared<PlusOp> (lhs, rhs) };
}

Expr
operator- (const Expr &lhs, const Expr &rhs)
{
  return { std::make_shared<MinusOp> (lhs, rhs) };
}

Expr
operator* (const Expr &lhs, const Expr &rhs)
{
  return { std::make_shared<MultiplyOp> (lhs, rhs) };
}

Expr
operator/ (const Expr &lhs, const Expr &rhs)
{
  return { std::make_shared<DivideOp> (lhs, rhs) };
}
#endif	  // EXPR_HPP
