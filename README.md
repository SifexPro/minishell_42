# Minishell

A shell, but mini.

## About

Minishell is a project from the core curriculum at 42, where the goal is to implement a simple yet functional shell in C. This project focuses on understanding process creation, execution, and synchronization, as well as handling signals and user input efficiently.

The objective is to create a shell that can interpret and execute user commands while following the constraints imposed by the project (limited system functions, strict coding standards, etc.).

## Features

- Command execution with arguments (similar to bash)
- Handling of `|` (pipes) for command chaining
- Redirections (`<`, `>`, `>>`, `<<`)
- Environment variables (`$VAR` expansion)
- Built-in commands (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- Error handling and status codes

## Installation & Usage

### Requirements

Ensure you have the following installed:
- GNU make
- GCC

### Building the project

1. Clone this repository:
   ```sh
   git clone --recursive https://github.com/SifexPro/minishell_42
   ```
2. Navigate to the project directory:
   ```sh
   cd minishell
   ```
3. Compile the project:
   ```sh
   make
   ```

### Running the shell

Once compiled, you can launch Minishell by running:
   ```sh
   ./minishell
   ```
To check for memory leaks, you can use Valgrind:
```sh
valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes
```

## Notes

- This project follows the 42 norm, so expect some unconventional formatting.
- Certain standard library functions are restricted, making the implementation more challenging.
- While Minishell aims to be as close to Bash as possible, it is not a full-fledged shell replacement.
- For the full project subject [click here](https://cdn.intra.42.fr/pdf/pdf/136912/en.subject.pdf).
- You can also check the correction sheet [here](https://web.archive.org/web/20231228101331/https://rphlr.github.io/42-Evals/Cursus/Minishell/).


> [@SifexPro](https://github.com/SifexPro) & [@PolEpie](https://github.com/PolEpie)
