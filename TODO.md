# TODO

## Gerer les signaux

## Fix in exec

## Polar
- Signaux dans heredoc
- << stop cat | wc -l
ne s'arrête jamaiks avec les kill signaux, normal ?
gerer export avec _

== fix cd on deleted folder
-x cat | | jkfe
=x= bash: syntax error near unexpected token `|'
=x= status 2
-x token suivi d'un mot (pas pipe, pas void, <, >, <<, >>)
-x < $non_existant
-x bash: $non_existant: ambiguous redirect
-x expand seuleument alphanum+_, premier char != num
-x echo -nnn -n i
=x= doit juste print i sans \n (actuellement met un espece)
-x echo -nA
=x= print "-nA"
-x cd test_dir
-x> rm -r ../test_dir 
-x-[1]    811449 segmentation fault (core dumped)  ./minishell
-x pwd si dans non_existant
=x= pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
-x unset ne marche pas
- export append a la fin (pas possible)
-x export b
=x= dans export
=x= pas dans env
-x export b=
=x= dans export
=x= dans env
-x export a+=example
=x= concatene dans a
-x echo a'b'
-x ab


## CRASH

## Sifex 


### Damn
- gérer "'command' < file 'suite command'"
- gérer "wc -w < Makefile > test | wc -l" - "wc -l < Makefile | ls | wc -l > test | wc -l" - "ls > test | wc < Makefile -w -l"
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
- cat | cat | cat | cat | ls(command qui n'attend pas de input)
ouvre X*cat

+ print exit quand exit
+ exit avec non num arg
== bash: exit: a: numeric argument required
+ exit avec > 1 arg
== bash: exit: too many arguments
+ exist sans arg
== dernier status
-> ls | exit 9223372036854775809 | ls

- Gerer multiple pipe & redirection

### Check
- cat Makefile | gzip > test.gz
- ls | echo t
- ls | exit 5 5

