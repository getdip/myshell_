# myshell_
# MiniShell – A Simple POSIX-like Shell in C

`MiniShell` is a lightweight shell written in C that emulates key features of common POSIX shells like `bash` or `sh`. It supports job control, built-in commands, background execution, and more — all implemented from scratch using low-level system calls.

## Features

- 🔹 **Built-in commands**:
  - `cd` – Change directory
  - `pwd` – Print working directory
  - `echo` – Display messages or variables
  - `fg`, `bg` – Foreground/background job control
  - `jobs` – List active background/stopped jobs

- 🔹 **External command execution** using `execvp()`

- 🔹 **Background execution** using `&`

- 🔹 **Signal handling**:
  - `Ctrl+C` (SIGINT) — Interrupt foreground process
  - `Ctrl+Z` (SIGTSTP) — Suspend foreground process
  - Background job reaping using `SIGCHLD`

- 🔹 **Command prompt customization** via `PS1` environment variable

- 🔹 **Accurate exit status** reporting with `$?` like in standard shells

##  Getting Started

### Requirements
  🔹C compiler (GCC)
  🔹Make

### Build & Run




```sh
make           # Build the shell
./minishell    # Launch it
