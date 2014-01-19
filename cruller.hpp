#pragma once

#include <string>

namespace llvm {
  class Function;
}

struct Object {
  virtual Object* eval()=0;
};

struct Number : public Object {
  int val;
  virtual Object* eval();

  Number(int v) : val(v) {}
};

struct List : public Object {
  List* cdr;
  Object* car;
  virtual Object* eval();

  List(Object* car, List* cdr) : car(car), cdr(cdr) {}
};

struct Identifier : public Object {
  std::string name;
  virtual Object* eval();

  Identifier(std::string n) : name(n) {}
};

struct Fun : public Object {
  llvm::Function* the_fun;

  virtual Object* eval();
  Object* apply(List* args);
};
