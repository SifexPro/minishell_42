# TODO

### To-Fix
- cmd + token + void ("cat Makefile <") = Segmentation fault (core dumped)
- historique minishell != historique bash (fleche du haut pour ancienne commande) : faut faire un strcmp pour pas dupliquer les commandes identique dans l'historique

### Gerer les signaux

### Faire les builtin
- export with no options
- unset with no options
- env with no options or arguments

### Fix in exec
- "< file" = crash
- "<< heredoc" = crash
- Handle exit (in pipe)
- Signaux dans heredoc

0 [CUSTOM] minishell_42 > < sss ls 
error: sss: No such file or directory
0 [CUSTOM] minishell_42 > echo $?
0 should be 1

### Conditionals mes couilles
- < Makefile cat | wc -l | cat > test