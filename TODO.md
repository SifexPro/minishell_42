# TODO

## Gerer les signaux

## Fix in exec

## Polar
- Signaux dans heredoc
- << stop cat | wc -l
ne s'arrête jamaiks avec les kill signaux, normal ?

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

### Check
- < Makefile cat | wc -l | cat
- cat Makefile | gzip > test.gz
- << file cat
-> cant exit