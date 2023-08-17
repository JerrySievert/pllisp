#pragma once

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
typedef struct List {
  struct List *next;
  void *data;
} List;

List *cons(void *_car, void *_cdr);

List *fcons(List *a);
List *fcar(List *a);
List *fcdr(List *a);
List *feq(List *a);
List *fpair(List *a);
List *fatom(List *a);
List *fnull(List *a);
List *freadobj(List *a);
List *fwriteobj(List *a);

List *eval(List *exp, List *env);

void *intern(char *sym);
char get_nextchar( );
void gettoken( );
void *getobj( );

extern int look;

#define is_pair(x)                                                             \
  (((uintptr_t)x & 0x1) ==                                                     \
   0x1) /* tag pointer to pair with 0x1 (alignment dependent)*/
#define is_atom(x) (((uintptr_t)x & 0x1) == 0x0)
#define untag(x) ((uintptr_t)x & ~0x1)
#define tag(x) ((uintptr_t)x | 0x1)
#define car(x) (((List *)untag(x))->data)
#define cdr(x) (x ? ((List *)untag(x))->next : 0)
#define e_true cons(intern("quote"), cons(intern("t"), 0))
#define e_false 0
