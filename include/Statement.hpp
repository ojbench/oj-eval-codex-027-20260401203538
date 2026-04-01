#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;
  virtual void execute(VarState& state, Program& program) const = 0;
  const std::string& text() const noexcept;

 private:
  std::string source_;
};

class LetStatement : public Statement {
 public:
  LetStatement(std::string src, std::string var, std::unique_ptr<Expression> expr);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string var_;
  std::unique_ptr<Expression> expr_;
};

class PrintStatement : public Statement {
 public:
  PrintStatement(std::string src, std::unique_ptr<Expression> expr);
  void execute(VarState& state, Program& program) const override;

 private:
  std::unique_ptr<Expression> expr_;
};

class InputStatement : public Statement {
 public:
  InputStatement(std::string src, std::string var);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string var_;
};

class GotoStatement : public Statement {
 public:
  GotoStatement(std::string src, int target);
  void execute(VarState& state, Program& program) const override;

 private:
  int target_;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::string src, std::unique_ptr<Expression> left, char op,
              std::unique_ptr<Expression> right, int target);
  void execute(VarState& state, Program& program) const override;

 private:
  std::unique_ptr<Expression> left_;
  char op_;
  std::unique_ptr<Expression> right_;
  int target_;
};

class RemStatement : public Statement {
 public:
  RemStatement(std::string src, std::string info);
  void execute(VarState& state, Program& program) const override;
};

class EndStatement : public Statement {
 public:
  EndStatement(std::string src);
  void execute(VarState& state, Program& program) const override;
};
