# spawni

### Version: 0.1

This package provides a wrapper over an interpreter.

You can install this package via

`garden install github FascinatedBox/spawni`

and then use it like so

`import spawni`

## Classes

#### `class Interpreter`

This class provides a wrapper around a Lily interpreter. This class provides a
constructor that does not take arguments. The parse methods will rewind state on
failure.

`method Interpreter.error: String`

Returns the error (message and traceback) from the last parse. If there was no
error, then the result is an empty `String`.

`method Interpreter.error_message: String`

Get just the error message of the last parse as a `String`. If there is no
error, then the result is an empty `String`.

`method Interpreter.parse_expr(context: String, data: String): Option[String]`

Parse an expression that should return a value. 'context' is used as the
filename in the event of an error. The result is a `Some` containing the
expression's value into a `String`, or `None`.

`method Interpreter.parse_file(filename: String): Boolean`

Try to open 'filename', parse the contents, and execute them.

`method Interpreter.parse_string(context: String, data: String): Boolean`

Similar to 'parse_expr', except that the contents are not limited to a single
expression.
