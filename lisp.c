#include <stdio.h>
#include <string.h>

#include "lisp.h"
#include "micro-lisp.h"

static int current;
static char *source_code;

/*
 * A hack to read into the micro-lisp parser, which uses getchar().
 */
char get_nextchar( ) {
  if (source_code[ current ] == '\0') {
    return EOF;
  }

  return source_code[ current++ ];
}

/*
 * Copies the output of a variable into a string.
 *
 * Warning, this will crash if over 10k.
 */
char *char_print_obj(List *ob, int head_of_list) {
  char *buf = (char *)internal_alloc(10240);
  buf[ 0 ]  = '\0';

  if (!is_pair(ob)) {
    sprintf(buf, "%s", ob ? (char *)ob : "null");
  } else {
    if (head_of_list) {
      sprintf(buf, "(");
    }

    char *more = char_print_obj(car(ob), 1);
    strcat(buf, more);
    internal_free(more);

    if (cdr(ob) != 0) {
      strcat(buf, " ");
      more = char_print_obj(cdr(ob), 0);
      strcat(buf, more);
      internal_free(more);
    } else
      strcat(buf, ")");
  }

  return buf;
}

/*
 * Lisp primitive to write to postgres.
 */
List *pwriteobj(List *a) {
  char *buf = char_print_obj(car(a), 1);
  notice(buf);
  internal_free(buf);
  return e_true;
}

void execute_lisp(char *source) {
  current     = 0;
  source_code = source;
  List *env   = cons(
      cons(intern("car"), cons((void *)fcar, 0)),
      cons(cons(intern("cdr"), cons((void *)fcdr, 0)),
             cons(cons(intern("cons"), cons((void *)fcons, 0)),
                  cons(cons(intern("eq?"), cons((void *)feq, 0)),
                       cons(cons(intern("pair?"), cons((void *)fpair, 0)),
                            cons(cons(intern("symbol?"), cons((void *)fatom, 0)),
                                 cons(cons(intern("null?"),
                                           cons((void *)fnull, 0)),
                                      cons(cons(intern("read"),
                                                cons((void *)freadobj, 0)),
                                           cons(cons(intern("write"),
                                                     cons((void *)pwriteobj, 0)),
                                                cons(cons(intern("null"),
                                                          cons(0, 0)),
                                                     0))))))))));

  look = get_nextchar( );
  gettoken( );
  pwriteobj(eval(getobj( ), env));
}
