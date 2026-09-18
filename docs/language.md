# The Solunox Language Guide

> This guide is derived from the **actual implementation** in `src/` (scanner, parser,
> walker), not from the original design ambitions. Where the implementation and the
> README disagree, this document describes what the code really does. See
> [Known Bugs & Unimplemented Features](#11-known-bugs--unimplemented-features) for the
> parts that are declared but broken or missing.

Solunox is a procedure-oriented language with **no variables**. All state lives in a
small set of containers, and every program is a set of functions that operate on those
containers.

---

## 1. Program shape

A Solunox source file is a sequence of **function declarations**:

```
$main{
    :sss:
    printc << "Hello, Solunox!"
}
```

Rules:

- A program is one or more functions. Nothing runs at the top level.
- Exactly one function must be named `main`. Execution starts there.
- Statement boundaries are implicit; **newlines are not significant** and there is no
  statement terminator. Do not use `;` (see [bugs](#11-known-bugs--unimplemented-features)).
- Each function body must begin with a container declaration (`:xyz:`).

The tree-walk pipeline is: `Scanner` → `Parser` (AST + semantic checks) → `mainWalker`.
`mainWalker` looks up `mainLocation` (recorded when the scanner sees the identifier
`main`) and starts walking that function.

---

## 2. Lexical elements

### 2.1 Comments

```
// a line comment
```

`//` skips to the end of the line.

### 2.2 Numbers

Decimal integer literals only: `0`, `42`, `1234`. There are no floating-point literals
and no negative literals — use unary `-`.

### 2.3 Characters

A single-quoted character becomes its ASCII code (a number):

```
'A'      // 65
printc << 'A'    // prints A
```

### 2.4 Strings

A double-quoted string is expanded by the scanner into comma-separated character codes,
which is why it works with the comma/multi-operand rule:

```
"Hi"     // equivalent to 72, 105
```

Because of this expansion, strings are most useful with `printc`:

```
printc << "Hello"
```

> A string is **not** a first-class value. It is only usable where a comma-separated
> list of operands is allowed.

### 2.5 Identifiers and keywords

Identifiers match `[A-Za-z_][A-Za-z0-9_]*`. The reserved words are loaded in
`scanner.cpp` (`loadKeywords`):

| Reserved | Purpose |
| --- | --- |
| `sol`, `luna`, `nox` | The three containers |
| `omnis` | Shared stack across all functions |
| `constas` | Reserved for the immutable array (not implemented) |
| `input` | Read an integer from stdin |
| `print` | Print an integer + newline |
| `printc` | Print a character |
| `if`, `loop` | Control flow |
| `break`, `return` | Control flow |
| `and`, `or` | Logical operators |
| `trash` | Placeholder operand for `~>` |
| `else` | Reserved but **unused** |

Any other identifier is a function name.

### 2.6 Punctuation and operators

These are the only meaningful symbols. Anything not listed here (for example `?`, `[`,
`]`, `%`, `&`, `|`) is **silently ignored** by the scanner.

| Symbol | Token | Meaning |
| --- | --- | --- |
| `$` | `DOLLAR` | Start a function declaration |
| `@` | `CALLER` | Call a function |
| `:` | — | Delimit a container declaration `:xyz:` |
| `{` `}` | `COPEN` `CCLOSE` | Block / function body |
| `(` `)` | `BOPEN` `BCLOSE` | Group an expression |
| `,` | `COMMA` | Separate multiple operands |
| `+` `-` `*` `/` | arithmetic | |
| `=` | `EQUAL` | Replace the current element |
| `==` `!=` `<` `<=` `>` `>=` | comparison | |
| `!` | `BANG` | Logical negation |
| `and` `or` | `AND` `OR` | Logical operators (there is no `&&` / `||`) |
| `<<` | `LEFTCOPY` | Non-destructive push/copy |
| `<~` | `LEFTPUSH` | Push (and, if the source is a container, move/pop it) |
| `>>` | `RIGHTCOPY` | Parsed but does nothing |
| `~>` | `RIGHTPUSH` | Parsed but does nothing (expects `trash`) |
| `^` | `EXPO` | Repeat the previous operand N times |
| `'` | — | Character literal |
| `"` | — | String literal |

> **Important:** the docs/README historically use `->` and `<<` for push. The actual
> push operators are `<~` (moving push) and `<<` (copying push). `->` is not a token;
> it lexes as `-` followed by `>`.

---

## 3. Container declaration

Every function body starts with a **container declaration** telling the runtime what
each container should behave as:

```
:sqp:
```

The three characters map, **in order**, to `sol`, `luna`, `nox`:

| Char | Type | `add` | `remove` | `peek` |
| --- | --- | --- | --- | --- |
| `s` | Stack | push front | pop front | front |
| `q` | Queue | enqueue back | dequeue front | front |
| `p` | Priority queue | insert in sorted order | remove smallest | smallest |

So `:sqs:` means `sol`=stack, `luna`=queue, `nox`=stack. All nine combinations are
valid, and the containers start empty on every function call.

---

## 4. Containers and the scope stack

At runtime there is a stack of `containers` (one per active function call). Each
`containers` holds `sol`, `luna`, and `nox` built with the types from that function's
declaration. Reading `sol` / `luna` / `nox` in an expression returns the `peek()` of
the container.

- Peeking an empty container returns **`-1`** (it does not error).
- Containers are **private to a function call**. A called function cannot see the
  caller's containers.
- `omnis` is the one exception: a single global stack shared by every function. It is
  the only way to pass data between functions.

---

## 5. Expressions

Expressions produce an integer. Grammar and precedence, from loosest to tightest:

| Level | Operators | Associativity |
| --- | --- | --- |
| logic | `and`, `or` | left |
| equality | `==`, `!=` | left |
| comparison | `<`, `<=`, `>`, `>=` | left |
| additive | `+`, `-` | left |
| multiplicative | `*`, `/` | left |
| unary | `!`, `-` | right |
| primary | number, char, `( expr )`, `sol`, `luna`, `nox`, `omnis`, `input`, `trash` | — |

Notes:

- `and` and `or` share one precedence level (unlike C).
- `^` is **not** an operator inside expressions. It is a statement-level repetition
  marker (see below).
- There is no `&&`, `||`, or `%`.
- Division is integer division; there is no divide-by-zero guard.

---

## 6. Statements

### 6.1 Operations (push / assign / print)

```
<target> <operator> <operand> [ , <operand> ... ]
```

`<target>` is one of `sol`, `luna`, `nox`, `constas`, `omnis`, `print`, `printc`.

Each `<operand>` is `expression [ ^ count ]`, where `^ count` repeats that operand
`count` times. Commas add more operands.

```
sol <~ 1^3          // same as: sol <~ 1, 1, 1   → pushes three 1s
printc << "Hi"      // "Hi" expands to 72, 105
```

> `^` is **repetition**, not exponentiation: `2^3` produces `2, 2, 2`, not `8`.

The operator determines what happens:

| Operator | On a container target (`sol`/`luna`/`nox`) | On `print` / `printc` | On `omnis` |
| --- | --- | --- | --- |
| `=` | Remove the current element, then add the operand (replaces the top) | Print, then pop the operand's container if it is one | Pushes to `omnis`, **but also pops `sol`** (bug) |
| `<~` | Add the operand, then pop the operand's container if it is one (so `a <~ b` **moves** `b`'s top into `a`) | Print, then pop if the operand is a container | Push, then pop the operand's container if it is one |
| `<<` | Add the operand (non-destructive) | Print, no pop | Push, no pop |
| `>>` | No-op | No-op | No-op |
| `~>` | No-op (expects `trash`) | No-op | No-op |

Practical consequences:

- **Push a literal:** `sol <~ 5` or `sol << 5`.
- **Copy a container's top:** `sol << luna` pushes `luna`'s top onto `sol`, leaving
  `luna` intact.
- **Move a container's top:** `sol <~ luna` pushes `luna`'s top onto `sol` and pops
  `luna`.
- **Replace the top:** `sol = 9` pops the current top and pushes `9`.
- **Destructive print (pop and print):** `print <~ sol` prints the top and pops it.
- **Non-destructive print:** `print << sol` prints the top and leaves it.
- **Discard a value / transfer to shared storage:** `omnis <~ sol`.

> `print` without an operator and operand is a syntax error (`print` alone).

### 6.2 `if`

```
if {
    <condition> { <statements> }
    <condition> { <statements> }
    ...
}
```

- Conditions are evaluated in order; the **first true condition's block runs**, then the
  whole `if` ends. This is how you write `else if` chains.
- There is **no `else`**. The keyword is reserved but the parser rejects it.
- An empty `if { }` is legal and does nothing.

```
if {
    sol > 10 { print << 100 }
    sol > 5  { print << 200 }
}
```

### 6.3 `loop`

```
loop {
    <statements>
}
```

Loops forever until a `break` (exits the innermost loop) or a `return` (exits the
function).

### 6.4 `break` and `return`

- `break` — leave the innermost loop. Using it outside a loop is a runtime error
  (`"No loops for break statement"`).
- `return` — stop the current function immediately.

`break` / `return` are plain statements with no value.

### 6.5 Function calls

```
@name
```

Calls a function as a statement. Calls take **no arguments** and produce **no return
value**. A call may appear anywhere a statement may (inside `if`, `loop`, etc.).

---

## 7. Functions and scope

### 7.1 Declaration

```
$name{
    :xyz:
    <statements>
}
```

Every function's first body element is its container declaration. Function order in the
file does not matter — you can call a function declared later.

### 7.2 Calling convention

- Each call pushes a **fresh container scope** whose types come from that function's
  declaration.
- The scope is popped when the function returns.
- Because scopes are private and there are no parameters or return values, **functions
  communicate only through `omnis`**.

### 7.3 Recursion

A function may call itself or other functions (including mutually). Recursion works,
but note:

- Each call gets empty containers, so you cannot pass a value down the recursion in
  `sol`/`luna`/`nox`. Use `omnis`.
- There is **no recursion-depth guard**. Infinite recursion will crash.

A complete recursive example is [`examples/factorial.slx`](../examples/factorial.slx):
it pushes `n` onto `omnis`, recurses on `n-1`, and leaves `n!` on `omnis` for the caller.

### 7.4 `main`

A function named `main` is required. If it is missing you get
`"No Main Function found"`. `mainLocation` is remembered by the scanner in declaration
order, so `main` does not have to be the first function.

---

## 8. Input and output

| Form | Effect |
| --- | --- |
| `input` | Reads one integer from stdin and evaluates to it |
| `print` | Prints an integer followed by a newline |
| `printc` | Prints `char(value % 128)` |

```
$main{
    :sss:
    sol <~ input
    print <~ sol + 20
}
```

```
$ echo 5 | ./solunox examples/input.slx
25
```

---

## 9. Worked examples

All of the examples below are verified to run against the current build. A larger
end-to-end program lives in
[`examples/toolkit.slx`](../examples/toolkit.slx) (recursion, all three container
kinds, `omnis`, and I/O).

### Hello

```
$main{
    :sss:
    printc << "Hello, Solunox!"
}
```

### Stack vs. queue

```
$main{
    :sqs:
    sol <~ 1, 2, 3       // stack: top is 3
    luna <~ 10, 20, 30   // queue: front is 10
    print << sol         // 3
    print << luna        // 10
}
```

### Conditionals

```
$main{
    :sss:
    sol <~ 7
    if {
        sol > 10 { print << 100 }
        sol > 5  { print << 200 }
    }
}
```

### Countdown with `loop`

There is no decrement operator, so the old value is staged in another container:

```
$main{
    :sss:
    sol <~ 3
    loop {
        print << sol
        luna <~ sol          // move top of sol into luna
        sol <~ luna - 1      // compute next value
        if {
            sol == 0 { break }
        }
    }
}
```

Output:

```
3
2
1
```

### Sharing data between functions with `omnis`

```
$produce{
    :sss:
    omnis <~ 42
    omnis <~ 7
}
$main{
    :sss:
    @produce
    print <~ omnis    // 7  (print and pop)
    print <~ omnis    // 42
}
```

### Characters

```
$main{
    :sss:
    sol <~ 'A'
    printc << sol        // A
    printc << sol + 1    // B
}
```

---

## 10. Grammar (EBNF)

See [`bnf.txt`](bnf.txt) for the full grammar. Summary:

```
program        = function, { function } ;
function       = "$", identifier, "{", declaration, statements, "}" ;
declaration    = ":", kind, kind, kind, ":" ;          (* sol, luna, nox *)
kind           = "s" | "q" | "p" ;

statement      = if | loop | call | "break" | "return" | operation ;
operation      = target, operator, operand, { ",", operand } ;
target         = "sol" | "luna" | "nox" | "constas" | "omnis" | "print" | "printc" ;
operator       = "=" | "<~" | "<<" | ">>" | "~>" ;
operand        = expression, [ "^", number ] ;
if             = "if", "{", { expression, block }, "}" ;
loop           = "loop", block ;
block          = "{", statements, "}" ;
call           = "@", identifier ;
```

---

## 11. Known Bugs & Unimplemented Features

This is the honest state of the interpreter. Anything here should be treated as
"documented intent, not working behaviour".

### Parsed but does nothing

- `a >> b` (`RIGHTCOPY`) — the walker has no `RIGHTCOPY` case.
- `a ~> trash` (`RIGHTPUSH`) — "pops" the `trash` operand, which is a no-op. There is
  currently **no clean way to discard a container's top** except by moving it to `omnis`
  or using a destructive `print <~`.

### Wrong runtime behaviour

- `omnis = v` pops `sol` before pushing to `omnis` (walker.cpp: `case OMNIS`).
- `nox = v` pops `luna` instead of `nox` (walker.cpp: `case NOX` under `EQUAL`).
- `popper()`'s `NOX` case has no `break`, so it falls through to `omnis.pop()`; if
  `omnis` is empty this aborts (`std::stack::pop` assertion).
- The `EQUAL` case removes the current element *before* evaluating the right-hand side,
  so `sol = sol - 1` does not decrement — it evaluates the already-popped container.

### Reserved but not implemented

- `else` — reserved by the scanner, never parsed.
- `constas` — accepted as a target but any read produces `"Error in evaluation"`;
  there is no array indexing (`constas[3]` is not lexable) and no `?` length check.
- Container type conversion `cont~s;` / `~q` / `~p` — no tokens.
- `new (SLL/DLL/CLL/TREE/HEAP/GRAPH)`, `*sol`, `sol ->`, `sol <-`, `sol ->[0]`,
  `a<->b` — none of these tokens exist.
- `?` (empty check), `[` `]` (indexing) — not lexed; silently ignored.

### Lexer / parser edge cases

- `;` is lexed as `SEMICOLON` but the parser never consumes it, producing
  `"Unexpected '}'..."` or `"Unexpected token in expression."`.
- `?`, `[`, `]`, `%`, `&`, `|` and other unknown characters are **silently dropped**,
  so typos may go unnoticed.
- Calling an undeclared function (`@ghost`) indexes past the end of the AST and crashes.
- Bare function names in expressions parse but fail at evaluation.
- `Token(TokenType, std::string)` sets the type to `DECLARE` and ignores the string; it
  appears to be dead code.

### Design gaps

- No parameters, no return values, no arrays, no user-defined data structures.
- Only `omnis` is shared; there is no global scope.
- No recursion-depth limit and no stack-overflow protection.
