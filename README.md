# Lisp Language Plugin for PostgreSQL

_Status:_ very early, it will crash if you look at it wrong.

This project started as what was supposed to be a humorous quip for a presentation on building extensions to Postgres, and then I went ahead and started it.

Currently, the lisp code itself is based off of [micro-lisp](https://github.com/carld/micro-lisp) but has been hacked up some to work with a Postgres extension. You can view the MIT license for this micro-lisp in either its repo or in [micro-lisp.c](micro-lisp.c) or [micro-lisp.h](micro-lisp.h).

Before this is released into alpha or beta, it will switch to an in-house written lisp interpreter.

## Where are we?

It will crash, and is in no way ready for even development use, but it can run some basics, like:

```
DO $$ (quote ((hello world))) $$ LANGUAGE pllisp;
```

There is currently only support for inline functions. Full support will come later, after the lisp support has been nailed down completely.

But for now, enjoy it for what it is: an interesting way to learn about language plugins.
