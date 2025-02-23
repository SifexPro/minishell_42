#!/bin/bash

# Vérifier que le fichier CSV est passé en argument
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 input.csv output.txt"
    exit 1
fi

input_file="$1"
output_file="$2"

# Vérifier que le fichier CSV existe
if [ ! -f "$input_file" ]; then
    echo "Le fichier $input_file n'existe pas."
    exit 1
fi

# Vider le fichier de sortie avant d'écrire
> "$output_file"

# Lire chaque ligne du fichier CSV
while IFS=, read -r command; do
    # Exécuter Valgrind pour chaque commande
    echo "Exécution de Valgrind pour: $command" >> "$output_file"

    # Utiliser un here document pour passer la commande à minishell
    valgrind --suppressions=readline.supp \
             --leak-check=full \
             --show-leak-kinds=all \
             --trace-children=yes \
             --track-fds=yes \
             ../../minishell << EOF >> "$output_file" 2>&1
$command
EOF

    echo "----------------------------------------" >> "$output_file"
done < "$input_file"

echo "Exécution terminée. Les résultats sont enregistrés dans $output_file."
