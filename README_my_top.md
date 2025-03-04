
# My_Top

**My_Top** is a command-line tool that mimics the functionality of the `top` command in Unix-based systems. It allows users to monitor the system's processes and statistics in real-time using the `ncurses` library. This program shows system information, CPU usage, memory statistics, and detailed information about running processes.

## Compilation

The project is compiled using the provided Makefile. It includes the following rules:

- **`make`**: Compiles the project.
- **`make clean`**: Cleans up the compiled object files.
- **`make fclean`**: Removes all generated files, including the binary.

### Compilation Instructions

To compile the project, run the following command in your terminal:

```bash
make
```

This will compile the project and create the executable `my_top`.

To clean up object files, use the command:

```bash
make clean
```

To remove all generated files, including the binary:

```bash
make fclean
```

## Execution

After compiling the project, you can run the program with the following command:

```bash
./my_top
```

### Program Options

- **`-U <username>`**: Filter the processes by a specific username.
- **`-d <seconds[.cents]>`**: Modify the delay between refreshes in seconds (default: `3.0` seconds).
- **`-n <frames>`**: Define how many frames to show before the program exits (default: unlimited).

For example, to show processes owned by the user `clery` with a 1.5-second refresh rate for 10 frames:

```bash
./my_top -U clery -n 10 -d 1.5
```

To display processes owned by `clery` with a default 3-second interval:

```bash
./my_top -U clery
```

## Tests

### Basic Tests
- **System uptime**: Passed
- **Logged in users**: Failed (needs improvement in user count)
- **Load average**: Passed
- **Tasks total, running, sleeping, stopped, and zombie**: Mostly passed with a few discrepancies in stopped tasks.

### Process Tests
- **CPU usage**: Failed in some cases, needs improvement in reading accurate CPU usage.
- **Memory usage**: Passed in most cases but failed in memory used and memory buffer/cache values.

### Ncurses Tests
- **Multiple frame printing**: Passed
