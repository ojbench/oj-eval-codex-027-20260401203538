// Clean implementation of Statement methods and derived classes declared in header
#include "Statement.hpp"

#include <iostream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

LetStatement::LetStatement(std::string src, std::string var,
                           std::unique_ptr<Expression> expr)
    : Statement(std::move(src)), var_(std::move(var)), expr_(std::move(expr)) {}

void LetStatement::execute(VarState& state, Program&) const {
  int v = expr_->evaluate(state);
  state.setValue(var_, v);
}

PrintStatement::PrintStatement(std::string src,
                               std::unique_ptr<Expression> expr)
    : Statement(std::move(src)), expr_(std::move(expr)) {}

void PrintStatement::execute(VarState& state, Program&) const {
  std::cout << expr_->evaluate(state) << "\n";
}

InputStatement::InputStatement(std::string src, std::string var)
    : Statement(std::move(src)), var_(std::move(var)) {}

void InputStatement::execute(VarState& state, Program&) const {
  std::cout << "?" << std::endl;
  long long val;
  if (!(std::cin >> val)) {
    throw BasicError("INVALID NUMBER");
  }
  state.setValue(var_, static_cast<int>(val));
}

GotoStatement::GotoStatement(std::string src, int target)
    : Statement(std::move(src)), target_(target) {}

void GotoStatement::execute(VarState&, Program& program) const {
  program.changePC(target_);
}

IfStatement::IfStatement(std::string src, std::unique_ptr<Expression> left,
                         char op, std::unique_ptr<Expression> right,
                         int target)
    : Statement(std::move(src)), left_(std::move(left)), op_(op),
      right_(std::move(right)), target_(target) {}

void IfStatement::execute(VarState& state, Program& program) const {
  int a = left_->evaluate(state);
  int b = right_->evaluate(state);
  bool cond = false;
  switch (op_) {
    case '=':
      cond = (a == b);
      break;
    case '<':
      cond = (a < b);
      break;
    case '>':
      cond = (a > b);
      break;
    default:
      throw BasicError("SYNTAX ERROR");
  }
  if (cond) program.changePC(target_);
}

RemStatement::RemStatement(std::string src, std::string)
    : Statement(std::move(src)) {}

void RemStatement::execute(VarState&, Program&) const {}

EndStatement::EndStatement(std::string src) : Statement(std::move(src)) {}

void EndStatement::execute(VarState&, Program& program) const {
  program.programEnd();
}
