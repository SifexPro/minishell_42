# TODO

## Gerer les signaux

## Faire les builtin
- export with no options
- unset with no options
- env with no options or arguments

## Fix in exec


## Polar
- Signaux dans heredoc
- << stop cat | wc -l
ne s'arrête jamaiks avec les kill signaux, normal ?


- "< bruh ls" = crash
- "< test" = crash
(et essaye de exec une commande qui n'existe pas)
- "<< heredoc" = crash
(et essaye de exec une commande qui n'existe pas)
- "< test wc -l | cat > bruh/test"
= crash
(première commande, pas de crash si avant par ex : < test wc -l | cat > test1)
- "< test wc -l"
(première commande, pas de crash si avant par ex : < test wc -l)
= crash

### Conditionals mes couilles
- < Makefile cat | wc -l | cat > test