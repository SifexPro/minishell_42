# TODO

### To-Fix
- pipe = tokens: -1
- space after a command (ex: "cat TODO.md ", "ls -a ", ...)
- CTRL+C -> New line (has the native bash)
- cmd + token + void ("cat Makefile <") = Segmentation fault (core dumped)

### Faire les tokens
	< should redirect input.
	> should redirect output.
	<< should be given a delimiter, then read the input until a line containing the	delimiter is seen. However, it doesn’t have to update the history!
	>> should redirect output in append mode.
### Faire les multiples pipes

### Gerer les signaux

### Faire les builtin
- export with no options
- unset with no options
- env with no options or arguments