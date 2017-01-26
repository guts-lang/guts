#define _XOPEN_SOURCE 500
#include "input.h"
#include "macro.h"
#include "strtab.h"
#include "tokenize.h"
#include <jayl/context.h>
#include <jayl/hash.h>

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#define HASH_TABLE_BUCKETS 1024

static struct hash_table macro_hash_table;
static int new_macro_added;

typedef array_of(String) ExpandStack;

/* Keep track of arrays being recycled. */
static array_of(TokenArray) arrays;
static array_of(ExpandStack) stacks;

static int is_expanded(const ExpandStack *scope, String name)
{
    int i;
    for (i = 0; i < array_len(scope); ++i) {
        if (!str_cmp(array_get(scope, i), name)) {
            return 1;
        }
    }
    return 0;
}

TokenArray get_token_array(void)
{
    TokenArray list = {0};
    if (array_len(&arrays)) {
        list = array_pop_back(&arrays);
        array_zero(&list);
        array_empty(&list);
    }

    return list;
}

void release_token_array(TokenArray list)
{
    array_push_back(&arrays, list);
}

static ExpandStack get_expand_stack(void)
{
    ExpandStack stack = {0};
    if (array_len(&stacks)) {
        stack = array_pop_back(&stacks);
        array_zero(&stack);
        array_empty(&stack);
    }

    return stack;
}

static void release_expand_stack(ExpandStack stack)
{
    array_push_back(&stacks, stack);
}

static int macrocmp(const struct macro *a, const struct macro *b)
{
    int i;

    if ((a->type != b->type) || (a->params != b->params))
        return 1;

    if (str_cmp(a->name, b->name))
        return 1;

    if (array_len(&a->replacement) != array_len(&b->replacement))
        return 1;

    for (i = 0; i < array_len(&a->replacement); ++i) {
        if (tok_cmp(
                array_get(&a->replacement, i),
                array_get(&b->replacement, i)))
            return 1;
    }

    return 0;
}

static String macro_hash_key(void *ref)
{
    return ((struct macro *) ref)->name;
}

static void macro_hash_del(void *ref)
{
    struct macro *macro = (struct macro *) ref;
    release_token_array(macro->replacement);
    free(macro);
}

static void *macro_hash_add(void *ref)
{
    struct macro *macro, *arg;

    arg = (struct macro *) ref;
    macro = calloc(1, sizeof(*macro));
    *macro = *arg;
    /*
     * Signal that the hash table has ownership now, and it will not be
     * freed in define().
     */
    new_macro_added = 1;
    return macro;
}

static void cleanup(void)
{
    int i;
    TokenArray list;
    ExpandStack stack;

    hash_destroy(&macro_hash_table);
    for (i = 0; i < array_len(&arrays); ++i) {
        list = array_get(&arrays, i);
        array_clear(&list);
    }
    for (i = 0; i < array_len(&stacks); ++i) {
        stack = array_get(&stacks, i);
        array_clear(&stack);
    }
    array_clear(&arrays);
    array_clear(&stacks);
}

static void ensure_initialized(void)
{
    static int done;

    if (!done) {
        hash_init(
            &macro_hash_table,
            HASH_TABLE_BUCKETS,
            macro_hash_key,
            macro_hash_add,
            macro_hash_del);
        atexit(cleanup);
        done = 1;
    }
}

static jayl_token_t get__line__token(void)
{
    int len;
    char buf[32];
    jayl_token_t t = basic_token[PREP_NUMBER];

    len = snprintf(buf, sizeof(buf), "%d", current_file_line);
    t.d.string = str_register(buf, len);
    return t;
}

static jayl_token_t get__file__token(void)
{
    jayl_token_t t = {STRING};
    t.d.string = current_file_path;
    return t;
}

/*
 * Replace __FILE__ with file name, and __LINE__ with line number, by
 * mutating the replacement list on the fly.
 */
const struct macro *definition(String name)
{
    struct macro *ref;

    ensure_initialized();
    ref = hash_lookup(&macro_hash_table, name);
    if (ref) {
        if (ref->is__file__) {
            array_get(&ref->replacement, 0) = get__file__token();
        } else if (ref->is__line__) {
            array_get(&ref->replacement, 0) = get__line__token();
        }
    }

    return ref;
}

static int has_stringify_replacement(const struct macro *def)
{
    int i;
    unsigned len = array_len(&def->replacement);

    if (len > 1) {
        for (i = 0; i < len - 1; ++i) {
            if (array_get(&def->replacement, i).token == '#'
                && array_get(&def->replacement, i + 1).token == PARAM)
            {
                return 1;
            }
        }
    }

    return 0;
}

void define(struct macro macro)
{
    struct macro *ref;
    static String
        builtin__file__ = SHORT_STRING_INIT("__FILE__"),
        builtin__line__ = SHORT_STRING_INIT("__LINE__");

    ensure_initialized();
    new_macro_added = 0;
    ref = hash_insert(&macro_hash_table, &macro);
    if (macrocmp(ref, &macro)) {
        error("Redefinition of macro '%s' with different substitution.",
            str_raw(macro.name));
        exit(1);
    } else {
        ref->stringify = has_stringify_replacement(ref);
        ref->is__file__ = !str_cmp(builtin__file__, ref->name);
        ref->is__line__ = !str_cmp(builtin__line__, ref->name);
        if (!new_macro_added) {
            release_token_array(macro.replacement);
        }
    }
}

void undef(String name)
{
    ensure_initialized();
    hash_remove(&macro_hash_table, name);
}

void print_token_array(const TokenArray *list)
{
    int i;
    String s;
    jayl_token_t t;

    putchar('[');
    for (i = 0; i < array_len(list); ++i) {
        if (i) {
            printf(", ");
        }
        t = array_get(list, i);
        if (t.token == PARAM) {
            printf("<param %ld>", t.d.val.i);
        } else if (t.token == EMPTY_ARG) {
            printf("<no-arg>");
        } else {
            putchar('\'');
            if (t.leading_whitespace > 0) {
                printf("%*s", t.leading_whitespace, " ");
            }
            if (t.token == NEWLINE) {
                printf("\\n");
            } else {
                s = jayl_token_str(t);
                printf("%s", str_raw(s));
            }
            putchar('\'');
        }
    }

    printf("] (%u)\n", array_len(list));
}

static jayl_token_t paste(jayl_token_t left, jayl_token_t right)
{
    jayl_token_t res;
    char *buf, *endptr;
    String ls, rs;

    if (left.token == EMPTY_ARG) {
        return right;
    } else if (right.token == EMPTY_ARG) {
        return left;
    }

    ls = jayl_token_str(left);
    rs = jayl_token_str(right);
    buf = calloc(ls.len + rs.len + 1, sizeof(*buf));
    buf = strcpy(buf, str_raw(ls));
    buf = strcat(buf, str_raw(rs));
    res = jayl_tokenize(buf, &endptr);
    if (endptr != buf + ls.len + rs.len) {
        error("Invalid token resulting from pasting '%s' and '%s'.",
            str_raw(ls), str_raw(rs));
        exit(1);
    }

    res.leading_whitespace = left.leading_whitespace;
    free(buf);
    return res;
}

static jayl_token_type_t peek_token(const TokenArray *list, int i)
{
    if (i < array_len(list)) {
        return array_get(list, i).token;
    }

    return END;
}

/*
 * Replace content of list in segment [start, start + gaplength] with
 * contents of slice. The gap is from reading arguments from list, and
 * the slice is result of expanding it. Slice might be smaller or larger
 * than the gap.
 */ 
static void array_replace_slice(
    TokenArray *list,
    unsigned start,
    unsigned gaplength,
    TokenArray *slice)
{
    unsigned length;
    assert(start + gaplength <= array_len(list));

    length = array_len(list) - gaplength + array_len(slice);
    array_realloc(list, length);

    /*
     * Move trailing data out of the way, or move closer to prefix, to
     * align exactly where slice is inserted.
     */
    if (array_len(slice) != gaplength) {
        memmove(
            list->data + start + array_len(slice),
            list->data + start + gaplength,
            (array_len(list) - (start + gaplength)) * sizeof(*list->data));
    }

    /* Copy slice directly into now vacant space in list. */
    if (array_len(slice)) {
        memcpy(
            list->data + start,
            slice->data,
            array_len(slice) * sizeof(*list->data));
    }

    list->length = length;
}

/*
 * Replacing # <param> and <a> ## <b> is done in an initial scan of
 * the replacement list. This pass requires the parameters to not be
 * expanded.
 *
 * Return an array which still can contain PARAM tokens that needs
 * further expansion.
 */
static TokenArray expand_stringify_and_paste(
    const struct macro *def,
    TokenArray *args)
{
    int len, d, i;
    jayl_token_t t, s;
    TokenArray list = get_token_array();

    len = array_len(&def->replacement);
    if (len && array_get(&def->replacement, 0).token == TOKEN_PASTE) {
        error("Unexpected '##' operator at beginning of line.");
        exit(1);
    } else if (len > 2) {
        if (array_get(&def->replacement, len - 1).token == TOKEN_PASTE) {
            error("Unexpected '##' operator at end of line.");
            exit(1);
        }
    }

    for (i = 0; i < len; ++i) {
        t = array_get(&def->replacement, i);
        switch (t.token) {
        case TOKEN_PASTE:
            i += 1;
            t = array_get(&list, array_len(&list) - 1);
            if (t.token == PARAM) {
                (void) array_pop_back(&list);
                array_concat(&list, &args[t.d.val.i]);
                t = array_get(&list, array_len(&list) - 1);
            }
            s = array_get(&def->replacement, i);
            if (s.token == PARAM) {
                d = s.d.val.i;
                s = array_get(&args[d], 0);
                t = paste(t, s);
                (void) array_pop_back(&list);
                if (t.token != EMPTY_ARG) {
                    array_concat(&list, &args[d]);
                    d = array_len(&list) - array_len(&args[d]);
                    array_get(&list, d) = t;
                }
            } else {
                t = paste(t, s);
                list.data[array_len(&list) - 1] = t;
            }
            break;
        case '#':
            i += 1;
            if (peek_token(&def->replacement, i) == PARAM) {
                d = array_get(&def->replacement, i).d.val.i;
                t = stringify(&args[d]);
                array_push_back(&list, t);
            } else {
                error("Stray '#' in replacement list.");
                exit(1);
            }
            break;
        default:
            array_push_back(&list, t);
            break;
        }
    }

    return list;
}

static int expand_line(ExpandStack *scope, TokenArray *list);

static TokenArray expand_macro(
    ExpandStack *scope,
    const struct macro *def,
    TokenArray *args)
{
    int i, j;
    jayl_token_t t;
    TokenArray list = expand_stringify_and_paste(def, args);

    if (def->params > 0) {
        for (i = 0; i < def->params; ++i) {
            expand(&args[i]);
            if (!args[i].data[0].leading_whitespace) {
                args[i].data[0].leading_whitespace = 1;
            }
            for (j = 0; j < array_len(&args[i]); ++j) {
                t = array_get(&args[i], j);
                if (t.is_expandable) {
                    args[i].data[j].disable_expand = 1;
                }
            }
        }

        for (i = 0; i < array_len(&list); ++i) {
            t = array_get(&list, i);
            if (t.token == PARAM) {
                j = t.d.val.i;
                if (array_get(&args[j], 0).token == EMPTY_ARG) {
                    array_erase(&list, i);
                } else {
                    array_replace_slice(&list, i, 1, &args[j]);
                }
            }
        }

        for (i = 0; i < def->params; ++i)
            release_token_array(args[i]);
        free(args);
    }

    expand_line(scope, &list);
    return list;
}

static const jayl_token_t *skip(const jayl_token_t *list, jayl_token_type_t token)
{
    String a, b;
    if (list->token != token) {
        a = jayl_token_str(basic_token[token]);
        b = jayl_token_str(*list);
        error("Expected '%s', but got '%s'.", str_raw(a), str_raw(b));
        exit(1);
    }

    list++;
    return list;
}

/*
 * Read tokens forming next macro argument. Missing arguments are
 * represented by a single EMPTY_ARG element.
 */
static TokenArray read_arg(
    ExpandStack *scope,
    const jayl_token_t *list,
    const jayl_token_t **endptr)
{
    int nesting = 0;
    jayl_token_t t;
    TokenArray arg = get_token_array();

    while (nesting || (list->token != ',' && list->token != ')')) {
        if (list->token == NEWLINE) {
            error("Unexpected end of input in expansion.");
            exit(1);
        }
        if (list->token == '(') {
            nesting++;
        } else if (list->token == ')') {
            nesting--;
            if (nesting < 0) {
                error("Negative nesting depth in expansion.");
                exit(1);
            }
        }
        t = *list++;
        if (t.is_expandable && is_expanded(scope, t.d.string)) {
            t.disable_expand = 1;
        }
        array_push_back(&arg, t);
    }

    if (!array_len(&arg)) {
        array_push_back(&arg, basic_token[EMPTY_ARG]);
    }
    *endptr = list;
    return arg;
}

static TokenArray *read_args(
    ExpandStack *scope,
    const struct macro *def,
    const jayl_token_t *list,
    const jayl_token_t **endptr)
{
    int i;
    TokenArray *args = NULL;

    if (def->type == FUNCTION_LIKE) {
        list = skip(list, '(');
        if (def->params) {
            args = malloc(def->params * sizeof(*args));
            for (i = 0; i < def->params; ++i) {
                args[i] = read_arg(scope, list, &list);
                if (i < def->params - 1) {
                    list = skip(list, ',');
                }
            }
        }
        list = skip(list, ')');
    }

    *endptr = list;
    return args;
}

static int expand_line(ExpandStack *scope, TokenArray *list)
{
    int size, i, n;
    jayl_token_t t;
    const struct macro *def;
    const jayl_token_t *endptr;
    TokenArray *args, expn;

    for (n = 0, i = 0; i < array_len(list); ++i) {
        t = array_get(list, i);
        if (!t.is_expandable || t.disable_expand) {
            continue;
        }

        def = definition(t.d.string);
        if (!def)
            continue;

        if (is_expanded(scope, t.d.string)) {
            array_get(list, i).disable_expand = 1;
            continue;
        }

        if (def->type == FUNCTION_LIKE && array_get(list, i + 1).token != '(')
            continue;

        args = read_args(scope, def, list->data + i + 1, &endptr);
        array_push_back(scope, def->name);
        expn = expand_macro(scope, def, args);
        size = (endptr - list->data) - i;
        (void) array_pop_back(scope);

        /* Fix leading whitespace after expansion. */
        if (array_len(&expn)) {
            expn.data[0].leading_whitespace = t.leading_whitespace;
        }

        /* Squeeze in expansion in list. */
        array_replace_slice(list, i, size, &expn);
        i += array_len(&expn) - 1;
        release_token_array(expn);
        n += 1;
    }

    return n;
}

int expand(TokenArray *list)
{
    int n;
    ExpandStack stack = get_expand_stack();

    n = expand_line(&stack, list);
    release_expand_stack(stack);
    return n;
}

int tok_cmp(jayl_token_t a, jayl_token_t b)
{
    if (a.token != b.token)
        return 1;

    if (a.token == PARAM) {
        return a.d.val.i != b.d.val.i;
    } else if (a.token == NUMBER) {
        if (!type_equal(a.type, b.type))
            return 1;
        return
            (is_unsigned(a.type)) ?
                a.d.val.u != b.d.val.u :
                a.d.val.i != b.d.val.i;
    } else {
        return str_cmp(a.d.string, b.d.string);
    }
}

/*
 * Convert list of tokens to a single STRING token.
 *
 * - All leading and trailing whitespace in text being stringified is
 *   ignored.
 * - Any sequence of whitespace in the middle of the text is converted
 *   to a single space in the stringified result.
 * - Quotes and special characters in STRING tokens are escaped.
 */
jayl_token_t stringify(const TokenArray *list)
{
    int i;
    jayl_token_t str = {0}, tok;
    String strval;
    char *buf;
    size_t cap, len, ptr;

    if (array_len(list) == 0 || array_get(list, 0).token == EMPTY_ARG) {
        str.d.string = str_init("");
    } else if (array_len(list) == 1) {
        tok = array_get(list, 0);
        str.d.string = jayl_token_str(tok);
    } else {
        /* Estimate 7 characters per token. */
        cap = array_len(list) * 7 + 1;
        buf = malloc(cap);
        len = ptr = 0;
        buf[0] = '\0';

        for (i = 0; i < array_len(list); ++i) {
            tok = array_get(list, i);
            assert(tok.token != END);
            /*
             * Do not include trailing space of line. This case hits
             * when producing message for #error directives.
             */
            if (tok.token == NEWLINE) {
                assert(i == array_len(list) - 1);
                break;
            }
            /*
             * Reduce to a single space, and only insert between other
             * tokens in the list.
             */
            strval = jayl_token_str(tok);
            len += strval.len + (tok.leading_whitespace && i);
            if (len >= cap) {
                cap = len + array_len(list) + 1;
                buf = realloc(buf, cap);
            }
            if (tok.leading_whitespace && i) {
                buf[ptr++] = ' ';
            }
            memcpy(buf + ptr, str_raw(strval), strval.len);
            ptr += strval.len;
        }

        str.d.string = str_register(buf, len);
        free(buf);
    }

    str.token = STRING;
    return str;
}

static TokenArray parse(char *str)
{
    char *endptr;
    jayl_token_t param = {PARAM};
    TokenArray arr = get_token_array();

    while (*str) {
        if (*str == '@') {
            array_push_back(&arr, param);
            str++;
        } else {
            array_push_back(&arr, jayl_tokenize(str, &endptr));
            assert(str != endptr);
            str = endptr;
        }
    }

    return arr;
}

static void register_macro(const char *key, char *value)
{
    struct macro macro = {{{0}}, OBJECT_LIKE};

    macro.name = str_init(key);
    macro.replacement = parse(value);
    define(macro);
}

void register_builtin_definitions(void)
{
    register_macro("__STDC__", "1");
    register_macro("__STDC_HOSTED__", "1");
    register_macro("__FILE__", "0");
    register_macro("__LINE__", "0");
    register_macro("__x86_64__", "1");
    register_macro("__SIZE_TYPE__", "unsigned long");
    register_macro("__WCHAR_TYPE__", "signed int");
    register_macro("__PTRDIFF_TYPE__", "signed long");
    register_macro("__CHAR_BIT__", "8");
    register_macro("__SIZEOF_LONG__", "8");
    register_macro("__SIZEOF_POINTER__", "8");

#ifdef __linux__
    register_macro("__linux__", "1");
#endif
#ifdef __unix__
    register_macro("__unix__", "1");
#endif

    switch (context.standard) {
    case STD_C89:
        register_macro("__STDC_VERSION__", "199409L");
        register_macro("__STRICT_ANSI__", "");
        break;
    case STD_C99:
        register_macro("__STDC_VERSION__", "199901L");
        break;
    }
}
