# Solunox

**Solunox** is a procedure-oriented, purpose-based programming language implemented in
C++ as a **tree-walk interpreter**. It is an educational tool for practising data
structures and algorithms: the language has **no variables**, so all computation is
expressed through a small set of containers that can act as stacks, queues, or priority
queues.

## How it works

The interpreter has three stages, all under `src/`:

- **Lexing** (`scanner.cpp`) — turns the source script into a sequence of tokens.
- **Parsing** (`parser.cpp`) — builds an Abstract Syntax Tree (AST) and performs
  semantic checks. Every function becomes one child of the root `FUNCTIONS` node, in
  declaration order.
- **Walking** (`walker.cpp`) — executes the program by traversing the AST. Execution
  begins at the function named `main`.

## A taste of the language

```
$main{
    :sqs:
    sol <~ 1, 2, 3       // sol is a stack,  luna is a queue
    luna <~ 10, 20, 30
    print << sol         // 3
    print << luna        // 10
}

$greet{
    :sss:
    printc << "Hello"
}
```

Key ideas:

- A program is a set of `$name{ ... }` functions; one must be `main`.
- Each function declares its container types in its first line, e.g. `:sqs:`
  (`s`tack / `q`ueue / `p`riority queue for `sol`, `luna`, `nox`).
- There are no variables, parameters, or return values. Each function call gets a fresh
  container scope, and `omnis` is the only storage shared between functions.
- `@name` calls a function; `<~` pushes (moving a container's top), `<<` pushes a copy,
  `print` / `printc` produce output, and `if` / `loop` / `break` / `return` control flow.

> The old README examples (e.g. `$f() {}`, `->`, `if/else`, `constas[3]`) do **not**
> match the implementation. The current language is documented in
> [`docs/language.md`](docs/language.md).

## Documentation

- [`docs/language.md`](docs/language.md) — the complete language guide, with verified
  examples and the current list of known bugs and unimplemented features.
- [`docs/bnf.txt`](docs/bnf.txt) — the grammar (EBNF) as implemented.
- [`docs/tokens.md`](docs/tokens.md) — token and keyword reference.
- [`notes/scanner.md`](notes/scanner.md) — design notes on the scanner.

## Layout

```
Solunox/
├── src/            # C++ source files (scanner, parser, walker, ...)
├── include/        # C++ header files
├── examples/       # Sample .slx scripts
├── docs/           # Language docs (guide, grammar, tokens)
├── notes/          # Design notes
├── Makefile
└── README.md
```

## Build & Run

```
make build              # compile into ./solunox
make run                # build and run examples/hello.slx
make clean              # remove object files and the binary
```

Run any script with:

```
./solunox <filename.slx>
```

## Project status

Implemented and working: functions and calls (including recursion), per-call container
scopes, stack / queue / priority-queue containers, integer/char/string literals,
arithmetic and logical expressions, `if` chains, `loop` / `break` / `return`, `print` /
`printc` / `input`, and the shared `omnis` stack.

Not implemented (despite older docs): `else`, `constas` arrays and indexing,
container type conversion, linked lists / trees / graphs, and the `>>` / `~>` pop
operators. See
[`docs/language.md` §11](docs/language.md#11-known-bugs--unimplemented-features) for
the authoritative list.
