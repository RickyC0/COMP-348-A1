# A1

## Overview

This project is designed to compile and run on a standard Linux system (e.g., Ubuntu via WSL or a Debian-based Docker container). It processes a directory structure by reading files, performing string replacements, and handling user interactions.

## File Structure

- **`kode.c`**  
  - Contains the `main()` function.
  - Handles command-line argument parsing and overall program control.

- **`fileread.c`**  
  - Reads files from the specified directory structure.
  - It implements logic for command processing and the type of replacements to be done.

- **`wreplace.c`**  
  - Replaces specific words.

- **`ui.c`**  
  - Manages all user interactions, including confirmation prompts, summary outputs, and error messages.

> **Note:** Each source file has a corresponding header (`.h`) file.


## System Requirements

- **Operating System:** Linux distribution (Ubuntu, Debian, etc.)
- **Compiler:** GCC
- **Windows Users:** Use WSL (Windows Subsystem for Linux) or a Debian-based Docker container.

## Compilation Instructions

### On Windows

Open your Windows terminal and run:

```bash
gcc -o kode -Wall kode.c fileread.c wreplace.c ui.c
```

### On Linux (or via Windows WSL)
```bash
gcc -Wall -o kode kode.c fileread.c wreplace.c ui.c
```

## Running the Application
After compilation, run the program using the following syntax:

./kode `<command>` `<word>` `<file>`

### Parameters
- `<command>`: Command of replacement type
- `<word>`: Target word for processing
- `<file>`: File or directory path to process

### Example
./kode RC Word file.txt

| Command | Description |
|---------|-------------|
| RC      | Replace Case Sensitive |
| RI      | Replace Ignore Case |
| UK      | Unique Keep Case |
| UM      | Unique Match Case |
