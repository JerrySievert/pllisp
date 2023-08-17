CREATE OR REPLACE FUNCTION pllisp_inline_handler(internal) RETURNS void
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE OR REPLACE FUNCTION pllisp_call_handler() RETURNS language_handler
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE OR REPLACE LANGUAGE pllisp
  HANDLER pllisp_call_handler
  INLINE pllisp_inline_handler;
