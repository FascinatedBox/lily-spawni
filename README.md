# spawni

Provide an interpreter as a value, so you can interpret while you interpret.
This can be installed using Lily's `garden` via:

`garden install github FascinatedBox/spawni`

## class Interpreter

This class wraps over an interpreter, providing useful methods.

### constructor Interpreter`: Interpreter`

Constructs a new `Interpreter`. This operation does not have any side-effects,
except the underlying allocation required. It is therefore possible to create as
many `Interpreter` instances as one may need.

The parse methods of this class will all execute code on success. On failure,
they will rewind state. In theory, variables declared in a failed parse should
not be available to subsequent passes.

### method Interpreter.error`(self: Interpreter): String`

Get the full error (message and traceback) from the last parse. If there is no
error, then the result is a `String`.

### method Interpreter.error_message`(self: Interpreter): String`

Get just the error message of the last parse as a `String`. If there is no
error, then the result is an empty `String`.

### method Interpreter.parse_expr`(self: Interpreter, context: String, data: String): Option[String]`

This parses `data` as an expression that has a result.

On success, a `Some` is returned with the result of the expression turned into a
`String`. The `String` may be empty if the expression returned an empty result.

On failure, a `None` is returned.

### method Interpreter.parse_file`(self: Interpreter, filename: String): Boolean`

This attempts to open `filename` and parse it. If parsing succeeds, then the
interpreter will attempt to execute the instructions.

### method Interpreter.parse_string`(self: Interpreter, context: String, data: String): Boolean`

This parses the content of `data` as-is. `context` is used as the source
filename in the event of an error.
