# Solunox Tokens

Every token produced by `Scanner` is defined by the `TokenType` enum in
`include/token.h` and created in `src/scanner.cpp`. A token carries a value in a union
(`Storage`): `number` for numeric/char tokens or `identifierKey` for function names.

## Literals and identifiers

| Source | TokenType | Value | Notes |
| --- | --- | --- | --- |
| `123` | `NUMBER` | `number` | Decimal integer |
| `'A'` | `NUMBER` | `number` = 65 | Character literal; expands to its ASCII code |
| `"Hi"` | `NUMBER`,`COMMA`,… | `number` per char | String expands to comma-separated char codes |
| `foo` | `FUNCTION` | `identifierKey` | Any non-reserved identifier; key is its index in the global `functions` map |
| `main` | `FUNCTION` | `identifierKey` | Also records `mainLocation` |

## Operators

| Source | TokenType | Notes |
| --- | --- | --- |
| `+` | `PLUS` | |
| `-` | `MINUS` | |
| `*` | `MULTIPLY` | |
| `/` | `DIVIDE` | `//` starts a comment instead |
| `=` | `EQUAL` | |
| `==` | `EQUALEQUAL` | |
| `!=` | `BANGEQUAL` | |
| `!` | `BANG` | |
| `<` | `LESSTHAN` | |
| `<=` | `LESSEQUAL` | |
| `>` | `GREATERTHAN` | |
| `>=` | `GREATEREQUAL` | |
| `<<` | `LEFTCOPY` | Non-destructive push |
| `<~` | `LEFTPUSH` | Push; moves the source if it is a container |
| `>>` | `RIGHTCOPY` | Parsed, but the walker ignores it |
| `~>` | `RIGHTPUSH` | Parsed; expects `trash`; currently a no-op |
| `^` | `EXPO` | Statement-level operand repetition (not exponentiation) |
| `,` | `COMMA` | Separates operands; also separates expanded string chars |
| `(` `)` | `BOPEN` `BCLOSE` | Expression grouping |
| `{` `}` | `COPEN` `CCLOSE` | Blocks / function bodies |
| `$` | `DOLLAR` | Function declaration |
| `@` | `CALLER` | Function call |
| `:` | — | Delimits `:xyz:` (emits `DECLARE`) |
| `;` | `SEMICOLON` | Lexed but never parsed — causes an error |

## Keywords

Loaded by `loadKeywords()` (`src/scanner.cpp`).

| Source | TokenType |
| --- | --- |
| `input` | `INPUT` |
| `print` | `PRINT` |
| `printc` | `PRINTC` |
| `sol` | `SOL` |
| `luna` | `LUNA` |
| `nox` | `NOX` |
| `constas` | `CONSTAS` |
| `omnis` | `OMNIS` |
| `trash` | `TRASH` |
| `if` | `IF` |
| `else` | `ELSE` |
| `loop` | `LOOP` |
| `break` | `BREAK` |
| `return` | `RETURN` |
| `and` | `AND` |
| `or` | `OR` |

## Container declaration

`:sqp:` produces a single `DECLARE` token. The three letters are folded into one
base-3 number (`s=0`, `q=1`, `p=2`) in the order `sol`, `luna`, `nox`, then decoded by
`solveFunction`.

## Internal (AST-only) token types

These are never produced by the scanner; they label parser-created nodes.

| TokenType | Meaning |
| --- | --- |
| `PROGRAM`, `FUNCTIONS` | Root nodes |
| `FUNCTION` | A function declaration node (also the type of name tokens) |
| `STATEMENT`, `STATEMENTS` | Statement list nodes |
| `CONDITION_BRANCH` | One condition + block inside an `if` |
| `STACK`, `QUEUE`, `PRIORITYQUEUE` | Decoded container kinds |
| `DEFAULT` | Reserved, unused |

## Unused / dead

The following enum values exist but are never generated or handled:
`SEMICOLON` (lexed, unparsed), `ELSE` (reserved, unparsed), `DEFAULT`,
`CONSTAS` (no evaluator case), and the `Token(TokenType, std::string)` constructor
(which sets the type to `DECLARE` and ignores its string argument).
