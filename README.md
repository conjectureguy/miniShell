# miniShell

A minimal Linux shell emulator written in C++. Demonstrates basic shell functionality by leveraging system calls for process management, I/O redirection, job control, and more.

## Features

- **Execute External Commands**  
  Forks and execs external programs.

- **Input/Output Redirection**  
  Supports `<`, `>`, and `>>` for redirecting stdin/stdout.

- **Command Piping**  
  Chains multiple commands using `|`.

- **Searchable Shell History**  
  Navigate through past commands with up/down arrows.

- **Tab Completion & Wildcards**  
  Auto-completes file names and supports `*`/`?` wildcards.

- **Job Control & Signal Handling**  
  Manage foreground/background jobs (`bg`/`fg`) and handle `Ctrl-C`, `Ctrl-Z`.
