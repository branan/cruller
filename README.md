# Cruller: A lisp on LLVM

## What's up with the name?

Two things:

1. It has two 'l's, to go with LLVM
2. Its syntax and special forms are heavily influenced by Clojure, and the words are kinda similar

## TODOs

* Replace the single yyparse() with some sort of real repl
* Make it easier to hook builtin functions into LLVM
* Much better code generation. Currently we are basically a lisp
  interpreter using LLVM for function lookup. Generating larger chunks
  of LLVM bytecode for a given expression is basically required.
