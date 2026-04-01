#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      TokenStream ts = lexer.tokenize(line);
      const Token* first = ts.peek();
      if (first && first->type == TokenType::RUN) {
        program.run();
      } else if (first && first->type == TokenType::LIST) {
        program.list();
      } else if (first && first->type == TokenType::CLEAR) {
        program.clear();
      } else if (first && first->type == TokenType::QUIT) {
        break;
      } else if (first && first->type == TokenType::HELP) {
        std::cout << "Type BASIC commands." << std::endl;
      } else {
        auto parsed = parser.parseLine(ts, line);
        auto ln = parsed.getLine();
        if (ln.has_value()) {
          Statement* stmt = parsed.fetchStatement();
          if (stmt == nullptr) {
            program.removeStmt(*ln);
          } else {
            program.addStmt(*ln, stmt);
          }
        } else {
          Statement* stmt = parsed.fetchStatement();
          program.execute(stmt);
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}
