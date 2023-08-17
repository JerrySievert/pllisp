.PHONY: lintcheck format dep deps/quickjs

PLLISP_VERSION = 0.1.0

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

CP = cp
OBJS = lisp.o extension.o micro-lisp.o
MODULE_big = pllisp
EXTENSION = pllisp
DATA = pllisp.control pllisp--$(PLLISP_VERSION).sql

include $(PGXS)

