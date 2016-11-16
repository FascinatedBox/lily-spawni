#include "lily_api_alloc.h"
#include "lily_api_code_iter.h"
#include "lily_api_embed.h"
#include "lily_api_msgbuf.h"
#include "lily_api_value.h"

#include "extras_spawni.h"

/**
package spawni

Provide an interpreter as a value, so you can interpret while you interpret.
This can be installed using Lily's `garden` via:

`garden install github FascinatedBox/spawni`
*/

/**
class Interpreter

This class wraps over an interpreter, providing useful methods.
*/
typedef struct {
    LILY_FOREIGN_HEADER
    lily_state *subi;
} lily_spawni_Interpreter;

static void destroy_Interpreter(lily_spawni_Interpreter *lsi)
{
    lily_free_state(lsi->subi);
    lily_free(lsi);
}

/**
constructor Interpreter: Interpreter

Constructs a new `Interpreter`. This operation does not have any side-effects,
except the underlying allocation required. It is therefore possible to create as
many `Interpreter` instances as one may need.

The parse methods of this class will all execute code on success. On failure,
they will rewind state. In theory, variables declared in a failed parse should
not be available to subsequent passes.
*/
void lily_spawni_Interpreter_new(lily_state *s)
{
    lily_spawni_Interpreter *lsi;
    INIT_Interpreter(s, lsi); 
    lsi->subi = lily_new_state(lily_new_options());

    lily_return_foreign(s, ID_Interpreter(s), (lily_foreign_val *)lsi);
}

/**
method Interpreter.error(self: Interpreter): String

Get the full error (message and traceback) from the last parse. If there is no
error, then the result is a `String`.
*/
void lily_spawni_Interpreter_error(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);

    lily_return_string(s, lily_new_string(lily_get_error(lsi->subi)));
}

/**
method Interpreter.error_message(self: Interpreter): String

Get just the error message of the last parse as a `String`. If there is no
error, then the result is an empty `String`.
*/
void lily_spawni_Interpreter_error_message(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);

    lily_return_string(s,
            lily_new_string(lily_get_error_message(lsi->subi)));
}

/**
method Interpreter.parse_expr(self: Interpreter, context: String, data: String): Option[String]

This parses `data` as an expression that has a result.

On success, a `Some` is returned with the result of the expression turned into a
`String`. The `String` may be empty if the expression returned an empty result.

On failure, a `None` is returned.
*/
void lily_spawni_Interpreter_parse_expr(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);
    char *context = lily_arg_string_raw(s, 1);
    char *text = lily_arg_string_raw(s, 2);
    lily_value *value;
    const char *out_text;

    int ok = lily_parse_expr(lsi->subi, context, text, &out_text);

    if (ok) {
        lily_variant_val *somev = lily_new_variant(1);
        lily_variant_set_string(somev, 0, lily_new_string(out_text));
        lily_return_variant(s, LILY_SOME_ID, somev);
    }
    else
        lily_return_empty_variant(s, LILY_NONE_ID);
}

/**
method Interpreter.parse_file(self: Interpreter, filename: String): Boolean

This attempts to open `filename` and parse it. If parsing succeeds, then the
interpreter will attempt to execute the instructions.
*/
void lily_spawni_Interpreter_parse_file(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);
    char *filename = lily_arg_string_raw(s, 1);

    lily_return_boolean(s, lily_parse_file(lsi->subi, filename));
}

/**
method Interpreter.parse_string(self: Interpreter, context: String, data: String): Boolean

This parses the content of `data` as-is. `context` is used as the source
filename in the event of an error.
*/
void lily_spawni_Interpreter_parse_string(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);
    char *context = lily_arg_string_raw(s, 1);
    char *text = lily_arg_string_raw(s, 2);

    lily_return_boolean(s, lily_parse_string(lsi->subi, context, text));
}
#include "dyna_spawni.h"
