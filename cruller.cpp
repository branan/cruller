#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include "cruller.hpp"

#include <map>
#include <vector>
#include <iostream>
using namespace std;


map<string, Object*> symbol_table;
llvm::Module* m;
llvm::ExecutionEngine *e;
llvm::IRBuilder<> Builder(llvm::getGlobalContext());


Object* Number::eval() {
  return this;
}

Object* Fun::eval() {
  return this;
}

Object* List::eval() {
  Object* the_fun = car->eval();
  Fun* fun = dynamic_cast<Fun*>(the_fun);
  return fun->apply(cdr);
}

Object* Identifier::eval() {
  return symbol_table[name];
}

Object* Fun::apply(List* args) {
  std::vector<llvm::GenericValue> vals;
  while(args) {
    Object* obj = args->car->eval();
    llvm::GenericValue v;
    v.PointerVal = obj;
    vals.push_back(v);
    args = args->cdr;
  }
  llvm::GenericValue result = e->runFunction(the_fun, vals);
  return (Object*)(result.PointerVal);
}

extern "C" {
  Object* add(Object* a, Object* b) {
    Number *an, *bn, *result;
    an = dynamic_cast<Number*>(a);
    bn = dynamic_cast<Number*>(b);
    result = new Number(an->val + bn->val);
    return result;
  }
}

Fun* buildAddFunction() {
  std::vector<llvm::Type*> args;
  args.push_back(Builder.getInt8PtrTy());
  args.push_back(Builder.getInt8PtrTy());
  llvm::FunctionType* funType = llvm::FunctionType::get(Builder.getInt8PtrTy(), args, false);
  llvm::Constant* addFunc = m->getOrInsertFunction("add", funType);
  llvm::Function* the_func = (llvm::Function*)addFunc;
  Fun* final_fun = new Fun;
  final_fun->the_fun = the_func;
  return final_fun;
}
extern int yyparse();
extern Object* program;

int main() {
  m = new llvm::Module("test", llvm::getGlobalContext());
  llvm::FunctionPassManager fpm(m);
  e = llvm::EngineBuilder(m).create();

  symbol_table["add"] = buildAddFunction();
  

  yyparse();

  Object* result = program->eval();
  Number* num = (Number*)result;
  cout << num->val << endl;
}