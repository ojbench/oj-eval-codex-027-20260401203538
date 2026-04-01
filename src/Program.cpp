#include "Program.hpp"

#include <iostream>

#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(0), programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run() {
  programEnd_ = false;
  int first = recorder_.nextLine(-1);
  if (first == -1) return;  // nothing
  programCounter_ = first;
  while (programCounter_ != -1 && !programEnd_) {
    const Statement* stmt = recorder_.get(programCounter_);
    int current = programCounter_;
    int next = recorder_.nextLine(programCounter_);
    try {
      if (stmt) const_cast<Statement*>(stmt)->execute(vars_, *this);
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
      break;
    }
    if (programCounter_ == current) programCounter_ = next;
  }
  resetAfterRun();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear() {
  recorder_.clear();
  vars_.clear();
}

void Program::execute(Statement* stmt) {
  try {
    stmt->execute(vars_, *this);
  } catch (const BasicError& e) {
    std::cout << e.message() << "\n";
  }
  delete stmt;
}

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) { programCounter_ = line; }

void Program::programEnd() { programEnd_ = true; }

void Program::resetAfterRun() noexcept { programCounter_ = -1; }
