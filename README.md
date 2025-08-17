# VSH – A 42 KL Mini Shell  

![Language](https://img.shields.io/github/languages/top/vlow-dev/vsh)  
![Repo Size](https://img.shields.io/github/repo-size/vlow-dev/vsh)  
![Last Commit](https://img.shields.io/github/last-commit/vlow-dev/vsh)  
![License](https://img.shields.io/badge/license-MIT-blue)  

VSH is a **full-featured UNIX-like shell** implementation developed for the 42 curriculum’s m3 project track.  
It re-creates core shell behavior while providing a modular, well-documented C codebase for learning about **parsing, expansion, job control, and built-ins**.

---

## 📑 Table of Contents  
- [Features](#-features)  
- [Project Structure](#-project-structure)  
- [Build & Run](#️-build--run)  
- [Builtin Commands](#-builtin-commands)  
- [Shell Grammar Overview](#-shell-grammar-overview)  
- [Expansion Support](#-expansion-support)  
- [Signals & Job Control](#-signals--job-control)  
- [Debugging & Testing Helpers](#-debugging--testing-helpers)  
- [Credits](#-credits)  

---

## 🚀 Features  
- Interactive prompt with **colorized readline** support  
- Tokenizer & AST parser supporting:  
  - **Pipes** (`|`), **logical operators** (`&&`, `||`), **subshells**, **background jobs** (`&`), **sequences** (`;`), and **redirections** (`>`, `<`, `>>`, `<<`, `<&`, `>&`, `<>`, `<<<`)  
- Robust **expansion engine**: tilde, parameter, brace, glob, and quote removal  
- **Built-ins:** `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`  
- Dynamic environment management (`PWD`, `OLDPWD`, exported vars)  
- Signal handling for **SIGINT, SIGQUIT, SIGCHLD** (mimics real shell)  
- Process execution via an **AST interpreter** with pipelines, sequences, subshells  
- Detailed error handling for invalid syntax & execution failures  

---

## 📂 Project Structure  
```
Core/m3/vsh
├── Makefile           # Build targets
├── include/
│   └── minishell.h    # Core structs, enums, prototypes
├── libft/             # Custom C library (static link)
└── src/
    ├── minishell.c    # Entry point
    ├── builtins/      # Built-in command implementations
    ├── execution/     # AST interpreter & redirections
    ├── expansion/     # Expansion engine
    ├── inits/         # Env, prompt, signals, cleanup
    ├── parser/        # Recursive-descent AST parser
    └── tokenization/  # Lexical analyzer
```

---

## ⚙️ Build & Run  

### Requirements  
- POSIX-compliant environment (Linux / macOS)  
- `make`, `cc` (or compatible compiler)  
- `readline` development library  

### Build  
```bash
cd Core/m3/vsh
make
```

### Run  
```bash
./minishell
```

### Cleanup  
```bash
make clean     # remove object files
make fclean    # remove objects + binary
make re        # rebuild from scratch
```

---

## 🔧 Builtin Commands  

| Command   | Description |
|-----------|-------------|
| `cd`      | Change directory (`cd -`, `cd --` supported) |
| `echo`    | Print arguments (`-n` flag supported) |
| `env`     | Display environment variables |
| `exit`    | Exit shell with optional status |
| `export`  | Add/modify environment variables |
| `pwd`     | Print working directory |
| `unset`   | Remove environment variables |

---

## 🧩 Shell Grammar Overview  
The parser builds an **AST** with nodes representing:  
- **Simple commands**: words + optional redirections  
- **Pipelines**: `cmd1 | cmd2 | ...`  
- **Logical ops**: `cmd1 && cmd2`, `cmd1 || cmd2`  
- **Sequences**: `cmd1 ; cmd2`  
- **Background jobs**: `cmd &`  
- **Subshells/groupings**: `(cmd)`, `{ cmd ; }`  

Tokenization happens in multiple passes:  
1. Lexical separation  
2. Operator normalization  
3. Recursive-descent AST parsing  

---

## ✨ Expansion Support  

| Expansion   | Implementation |
|-------------|----------------|
| **Tilde**   | `tilde_expansion.c`, `tilde_user.c` |
| **Brace**   | `braces_expansion.c` |
| **Parameter** | `sign/` (`$VAR`, `$?`, etc.) |
| **Globbing** | `glob/` – pattern matching |
| **Quote**   | `quotes/` – handles `'`, `"`, `\` |
| **Here-doc** | handled by tokenizer + parser (`<<`) |

---

## 🛠️ Signals & Job Control  
- **SIGINT/SIGQUIT** → redraw prompt (does not exit)  
- **SIGCHLD** → reaps background processes  
- Global `g_signal` (`volatile sig_atomic_t`) captures async signal status  

---

## 🐞 Debugging & Testing Helpers  

| Target           | Purpose |
|------------------|---------|
| `make debug`     | Build with `-g3 -fpic` (debug symbols) |
| `make valgrind`  | Run under Valgrind leak checker |
| `make fval`      | Verbose Valgrind (origin tracking) |
| `make fsanitize` | Build with Address/Leak sanitizer |

---

## 👤 Credits  
- **Author:** [vlow](https://github.com/vlow-dev)  
- Relies on:  
  - Custom **libft**  
  - GNU **readline**  
