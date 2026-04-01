#include "Recorder.hpp"

#include <iostream>

#include "Statement.hpp"

Recorder::~Recorder() { clear(); }

void Recorder::add(int line, Statement* stmt) {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    delete it->second;
    it->second = stmt;
  } else {
    lines_[line] = stmt;
  }
}

void Recorder::remove(int line) {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    delete it->second;
    lines_.erase(it);
  }
}

const Statement* Recorder::get(int line) const noexcept {
  auto it = lines_.find(line);
  return it == lines_.end() ? nullptr : it->second;
}

bool Recorder::hasLine(int line) const noexcept { return lines_.count(line); }

void Recorder::clear() noexcept {
  for (auto& kv : lines_) delete kv.second;
  lines_.clear();
}

void Recorder::printLines() const {
  for (const auto& kv : lines_) {
    std::cout << kv.first << " " << kv.second->text() << "\n";
  }
}

int Recorder::nextLine(int line) const noexcept {
  auto it = lines_.upper_bound(line);
  return it == lines_.end() ? -1 : it->first;
}
