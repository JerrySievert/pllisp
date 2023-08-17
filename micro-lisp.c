/* micro lisp (C) A. Carl Douglas */

/*

MIT License

Copyright (c) 2017 A. Carl Douglas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "micro-lisp.h"

#define debug(m,e) printf("%s:%d: %s:",__FILE__,__LINE__,m); print_obj(e,1); puts("");

List *symbols = 0;
int look; /* look ahead character */
#define SYMBOL_MAX  32
static char token[SYMBOL_MAX]; /* token */
int is_space(char x)  { return x == ' ' || x == '\n'; }
int is_parens(char x) { return x == '(' || x == ')'; }

void gettoken() {
  int index = 0;
  while(is_space(look)) { look = get_nextchar(); }
  if (is_parens(look)) {
    token[index++] = look;  look = get_nextchar();
  } else {
    while(index < SYMBOL_MAX - 1 && look != EOF && !is_space(look) && !is_parens(look)) {
      token[index++] = look;  look = get_nextchar();
    }
  }
  token[index] = '\0';
}

List * cons(void *_car, void *_cdr) {
  List *_pair = calloc( 1, sizeof (List) );
  _pair->data = _car;
  _pair->next = _cdr;
  return (List*) tag(_pair);
}

void *intern(char *sym) {
  List *_pair = symbols;
  for ( ; _pair ; _pair = cdr(_pair))
    if (strncmp(sym, (char*) car(_pair), 32)==0) return car(_pair);
  symbols = cons(strdup(sym), symbols);
  return car(symbols);
}

List * getlist();

void * getobj() {
  if (token[0] == '(') return getlist();
  return intern(token);
}

List * getlist() {
  List *tmp;
  gettoken();
  if (token[0] == ')') return 0;
  tmp = getobj();
  return cons(tmp, getlist());
}

void print_obj(List *ob, int head_of_list) {
  if (!is_pair(ob) ) {
    printf("%s", ob ? (char*) ob : "null" );
  } else {
    if (head_of_list) printf("(");
    print_obj(car(ob), 1);
    if (cdr(ob) != 0) {
      printf(" ");
      print_obj(cdr(ob), 0);
    } else  printf(")");
  }
}

List *fcons(List *a)    {  return cons(car(a), car(cdr(a)));  }
List *fcar(List *a)     {  return car(car(a));  }
List *fcdr(List *a)     {  return cdr(car(a));  }
List *feq(List *a)      {  return car(a) == car(cdr(a)) ? e_true : e_false;  }
List *fpair(List *a)    {  return is_pair(car(a))       ? e_true : e_false;  }
List *fatom(List *a)    {  return is_atom(car(a))       ? e_true : e_false;  }
List *fnull(List *a)    {  return car(a) == 0           ? e_true : e_false; }
List *freadobj(List *a) {  look = get_nextchar(); gettoken(); return getobj();  }
List *fwriteobj(List *a){  print_obj(car(a), 1); puts(""); return e_true;  }

List * eval(List *exp, List *env);

List * evlist(List *list, List *env) {
  /* http://cslibrary.stanford.edu/105/LinkedListProblems.pdf */
  List *head = 0, **args = &head;
  for ( ; list ; list = cdr(list) ) {
    *args = cons( eval(car(list), env) , 0);
    args = &( (List *) untag(*args) )->next;
  }
  return head;
}

List * apply_primitive(void *primfn, List *args) {
  return ((List * (*) (List *)) primfn)  ( args );
}

List * eval(List *exp, List *env) {
  if (is_atom(exp) ) {
    for ( ; env != 0; env = cdr(env) )
      if (exp == car(car(env)))  return car(cdr(car(env)));
    return 0;
  } else if (is_atom( car (exp))) { /* special forms */
    if (car(exp) == intern("quote")) {
      return car(cdr(exp));
    } else if (car(exp) == intern("if")) {
      if (eval (car(cdr(exp)), env) != 0)
        return eval (car(cdr(cdr(exp))), env);
      else
        return eval (car(cdr(cdr(cdr(exp)))), env);
    } else if (car(exp) == intern("lambda")) {
      return exp; /* todo: create a closure and capture free vars */
    } else if (car(exp) == intern("apply")) { /* apply function to list */
      List *args = evlist (cdr(cdr(exp)), env);
      args = car(args); /* assumes one argument and that it is a list */
      return apply_primitive( eval(car(cdr(exp)), env), args);
    } else { /* function call */
      List *primop = eval (car(exp), env);
      if (is_pair(primop)) { /* user defined lambda, arg list eval happens in binding  below */
        return eval( cons(primop, cdr(exp)), env );
      } else if (primop) { /* built-in primitive */
        return apply_primitive(primop, evlist(cdr(exp), env));
      }
    }
  } else if (car(car(exp)) == intern("lambda")) { /* should be a lambda, bind names into env and eval body */
    List *extenv = env, *names = car(cdr(car(exp))), *vars = cdr(exp);
    for (  ; names ; names = cdr(names), vars = cdr(vars) )
      extenv = cons (cons(car(names),  cons(eval (car(vars), env), 0)), extenv);
    return eval (car(cdr(cdr(car(exp)))), extenv);
  }
  puts("cannot evaluate expression");
  return 0;
}
#if 0
int main(int argc, char *argv[]) {
  List *env = cons (cons(intern("car"), cons((void *)fcar, 0)),
              cons (cons(intern("cdr"), cons((void *)fcdr, 0)),
              cons (cons(intern("cons"), cons((void *)fcons, 0)),
              cons (cons(intern("eq?"), cons((void *)feq, 0)),
              cons (cons(intern("pair?"), cons((void *)fpair, 0)),
              cons (cons(intern("symbol?"), cons((void *)fatom, 0)),
              cons (cons(intern("null?"), cons((void *)fnull, 0)),
              cons (cons(intern("read"), cons((void *)freadobj, 0)),
              cons (cons(intern("write"), cons((void *)fwriteobj, 0)),
              cons (cons(intern("null"), cons(0,0)), 0))))))))));
  look = getchar();
  gettoken();
  print_obj( eval(getobj(), env), 1 );
  printf("\n");
  return 0;
}
#endif