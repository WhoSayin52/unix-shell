# Unix Shell

## Table of Contents
- [Description](#description)
- [Features](#features)
- [Installation](#installation)

## Description
This is a custom Unix shell implemented in C, using POSIX APIs and system calls. The shell supports both interactive and batch modes, providing basic shell functionality like process management, built-in commands, and more.

## Features

- **Command-Line Interface (CLI)**: A simple, text-based interface for interacting with the shell.
- **Process Management**: 
  - Use of `fork()`, `execv()`, and `wait()` system calls for process creation and management.
  - Executes commands in the foreground and background.
- **Built-in Commands**: 
  - `cd`: Change the current working directory.
  - `exit`: Exit the shell.
  - `path`: Set or display the command search path.
- **Output Redirection**: 
  - Use of the `>` operator for redirecting command output to a file.
- **Parallel Execution**: 
  - Use of the `&` operator to execute commands in the background.

## Installation

1. Clone the repository: 
```bash
git clone <repo-url>
cd unix-shell
```
2. Build the project:
```bash
cmake --preset default
cmake --build build --config Release # or Debug if desired
```
### Running the Shell

To run the shell, you can either use **interactive mode** or **batch mode**.

- **Interactive Mode**: Run the shell directly from the terminal. This will start an interactive shell session where you can enter and execute commands.
  
  ```bash
  ./path/to/exe
  ```
   
- **Batch Mode**: To execute a series of commands from a batch file, pass the file path as an argument to the shell. The shell will execute the commands in the batch file sequentially.

  ```bash
  ./path/to/exe ./path/to/batch/file
  ```









