#pragma once

#include <sys/types.h>

extern void execute_lisp(char *source);
extern void *internal_alloc(size_t size);
extern void internal_free(void *ptr);
void notice(char *message);
