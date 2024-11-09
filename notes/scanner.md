# The Scanner
- Since solunox does not have variables, we can optimize the scanner accordingly such that it does well in each case.
## Meet the Vars
- All the local storage comprises of 3 stacks
- Each function will have its own local storage
- So we end up with a weird looking stack of stacks
  - There will be a usual stack of numbers
  - Every stack will house 3 containers and the one array.
## The Keywords
- Since there are many weird representations, we will twist the meaning of "Tokens" here for easy work

### Operators (Other than Arithmetic)
- <<
- ->
- -n-> (To apply this, we will check if it exists, and have a diff name for it)

| How is the structure of a Token Looking
- Each Token will have
  - TokenType assigned to it
  - UINION, to specify what does it contain
    - void*
    - number
    - string
  - But here, not only the constants, but also the transfer symbols will contain the number, so that way, we can use the scanner to relieve a lot of burden from the Parser and Walker.
