#include "postgres.h"

#include "access/htup.h"
#include "catalog/pg_proc.h"
#include "fmgr.h"
#include "nodes/parsenodes.h"

#include "lisp.h"

PG_MODULE_MAGIC;

Datum pllisp_inline_handler(PG_FUNCTION_ARGS);
Datum pllisp_call_handler(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pllisp_inline_handler);
PG_FUNCTION_INFO_V1(pllisp_call_handler);

/*
 * Inline handler, gets a copy of the source code and runs it through
 * the interpreter.
 */
Datum pllisp_inline_handler(PG_FUNCTION_ARGS) {
  InlineCodeBlock *code_block =
      (InlineCodeBlock *)DatumGetPointer(PG_GETARG_DATUM(0));
  char *sourcecode = code_block->source_text;

  HeapTuple proctuple;
  Form_pg_proc pg_proc_entry = NULL;
  execute_lisp(sourcecode);

  PG_RETURN_VOID( );
}

/*
 * Call handler, currently just a placeholder.
 */
Datum pllisp_call_handler(PG_FUNCTION_ARGS) {
  elog(ERROR, "%s", "Only inline pllisp calls are currently implemented");
}

/*
 * Sends an elog(NOTICE) to Postgres of a message sent from the interpreter.
 */
void notice(char *message) { elog(NOTICE, "%s", message); }

/*
 * Memory allocation, using postgres, to be fed to the interpreter.
 */
void *internal_alloc(size_t size) { return palloc(size); }
void internal_free(void *ptr) { pfree(ptr); }