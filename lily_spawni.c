/**
library spawni

Provide an interpreter as a value, so you can interpret while you interpret.
This can be installed using Lily's `garden` via:

`garden install github FascinatedBox/spawni`
*/
#include "lily_api_embed.h"
#include "lily_api_msgbuf.h"
#include "lily_api_value.h"

/** Begin autogen section. **/
typedef struct lily_spawni_Interpreter_ {
    LILY_FOREIGN_HEADER
    lily_state *subi;
} lily_spawni_Interpreter;
#define ARG_Interpreter(state, index) \
(lily_spawni_Interpreter *)lily_arg_generic(state, index)
#define ID_Interpreter(state) lily_cid_at(state, 0)
#define INIT_Interpreter(state)\
(lily_spawni_Interpreter *) lily_new_foreign(state, ID_Interpreter(state), (lily_destroy_func)destroy_Interpreter, sizeof(lily_spawni_Interpreter))

const char *lily_spawni_table[] = {
    "\01Interpreter\0"
    ,"C\06Interpreter"
    ,"m\0<new>\0:Interpreter"
    ,"m\0error\0:String"
    ,"m\0error_message\0:String"
    ,"m\0parse_expr\0(String,String):Option[String]"
    ,"m\0parse_file\0(String):Boolean"
    ,"m\0parse_string\0(String,String):Boolean"
    ,"Z"
};
#define Interpreter_OFFSET 1
void lily_spawni_Interpreter_new(lily_state *);
void lily_spawni_Interpreter_error(lily_state *);
void lily_spawni_Interpreter_error_message(lily_state *);
void lily_spawni_Interpreter_parse_expr(lily_state *);
void lily_spawni_Interpreter_parse_file(lily_state *);
void lily_spawni_Interpreter_parse_string(lily_state *);
void *lily_spawni_loader(lily_state *s, int id)
{
    switch (id) {
        case Interpreter_OFFSET + 1: return lily_spawni_Interpreter_new;
        case Interpreter_OFFSET + 2: return lily_spawni_Interpreter_error;
        case Interpreter_OFFSET + 3: return lily_spawni_Interpreter_error_message;
        case Interpreter_OFFSET + 4: return lily_spawni_Interpreter_parse_expr;
        case Interpreter_OFFSET + 5: return lily_spawni_Interpreter_parse_file;
        case Interpreter_OFFSET + 6: return lily_spawni_Interpreter_parse_string;
        default: return NULL;
    }
}
/** End autogen section. **/

/**
foreign class Interpreter() {
    layout {
        lily_state *subi;
    }
}

This class wraps over an interpreter, providing useful methods. Each interpreter
created is completely separate from the rest, and creating a sub interpreter
does not have side-effects.

The parse methods of this class will all execute code on success. On failure,
they will rewind state. In theory, variables declared in a failed parse should
not be available to subsequent passes.
*/

static void destroy_Interpreter(lily_spawni_Interpreter *lsi)
{
    lily_free_state(lsi->subi);
}

void lily_spawni_Interpreter_new(lily_state *s)
{
    lily_spawni_Interpreter *lsi = INIT_Interpreter(s);
    lsi->subi = lily_new_state();

    lily_return_foreign(s, (lily_foreign_val *)lsi);
}

/**
define Interpreter.error: String

Get the full error (message and traceback) from the last parse. If there is no
error, then the result is a `String`.
*/
void lily_spawni_Interpreter_error(lily_state *s)
{
    lily_spawni_Interpreter *lsi = ARG_Interpreter(s, 0);

    lily_return_string(s, lily_new_string(lily_get_error(lsi->subi)));
}

/**
define Interpreter.error_message: String

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
define Interpreter.parse_expr(context: String, data: String): Option[String]

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
        if (out_text == NULL)
            out_text = "";

        lily_container_val *somev = lily_new_some();
        lily_nth_set(somev, 0, lily_box_string(s, lily_new_string(out_text)));
        lily_return_variant(s, somev);
    }
    else
        lily_return_none(s);
}

/**
define Interpreter.parse_file(filename: String): Boolean

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
define Interpreter.parse_string(context: String, data: String): Boolean

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
