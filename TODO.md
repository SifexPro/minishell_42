# TODO

## Gerer les signaux

## Fix in exec

## Polar
- Signaux dans heredoc
- << stop cat | wc -l
ne s'arrête jamaiks avec les kill signaux, normal ?
- << test
== crash

- cat | | jkfe
== bash: syntax error near unexpected token `|'
== status 2
- token suivi d'un mot (pas pipe, pas void, <, >, <<, >>)
- < $non_existant
bash: $non_existant: ambiguous redirect
- expand seuleument alphanum+_, premier char != num
- echo -nnn -n i
== doit juste print i sans \n (actuellement met un espece)
- echo -nA
== print "-nA"



## CRASH
- echo te < cat

## Sifex 

### Conditionals mes couilles
- < Makefile cat | wc -l | cat > test

### Damn
- gérer "'command' < file 'suite command'"
- "exit exit_code"
- gérer "wc -w < Makefile > test | wc -l" - "wc -l < Makefile | ls | wc -l > test | wc -l" - "ls > test | wc < Makefile -w -l"
- gérer exit
- > test1 > test2 > test3 echo salut | echo cho >test4
- < Makefile > t1 < bruh > t2

- echo hello > test1 > test2 > test3
ouvrir tous les outfile et mettre dans le dernier
- < Makefile > 000chmod < test3 > t2 echo tes
== pas d'exec
rien
- < Makefile > t2 < test3 > t1 echo test
== pas d'exec
creation t2
- cat | cat | cat | cat | ls
ouvre X*cat
- rm -r ../test_dir
[1]    811449 segmentation fault (core dumped)  ./minishell
- pwd si dans non_existant
== pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
- print exit quand exit
- exit avec non num arg
== bash: exit: a: numeric argument required
- exit avec > 1 arg
== bash: exit: too many arguments
- exist sans arg
== dernier status

- Gerer multiple pipe & redirection

### Check
- cat Makefile | gzip > test.gz
- ls | echo t

- unset ne marche pas
- export append a la fin
- export b
== dans export
== pas dans env
- export b=
== dans export
== dans env
- export a+=example
== concatene dans a

