# Mark Shell (marksh) 🐚
![Shell](https://img.shields.io/badge/status-active-brightgreen)

A fully functional custom Unix-like shell implemented in C. Mark Shell (`marksh`) demonstrates command execution, I/O redirection, command chaining, signal handling, and user-friendly features like command history and tab completion.

---

## Table of Contents
1. [Introduction](#introduction)
2. [Key Features](#key-features)
3. [Installation](#installation)
4. [How Mark Shell Works](#technical-deep-dive)
5. [Usage Examples](#usage-examples)
6. [Future Enhancements](#future-roadmap)


---

<a name="introduction"></a>
## 1. Introduction 📖

Mark Shell (`marksh`) is a custom command-line shell built from scratch in C.  
It provides a realistic terminal experience, supporting core shell functionality and advanced features like command history, tab completion, and I/O redirection.  

This project was created to explore system programming, terminal control, and process management in Unix-like systems.

---

<a name="key-features"></a>
## 2. Key Features 🚀

| Feature Category       | Commands / Operations                             | Status |
|------------------------|--------------------------------------------------|--------|
| Basic Execution        | External commands (ls, vim, etc.)               | ✅     |
| Built-in Commands      | cd, pwd, mkdir, exit, help                       | ✅     |
| Command History        | Up/Down arrow navigation                         | ✅     |
| Backspace Support      | Proper deletion of characters while typing      | ✅     |
| Tab Completion         | Commands & file names                            | ✅     |
| I/O Redirection        | `>`, `<`, `>>`                                   | ✅     |
| Command Chaining       | `&&`, `||`, `;`                                 | ✅     |
| Signal Handling        | Ctrl+C (SIGINT), Ctrl+D (EOF)                   | ✅     |
| Advanced Features      | Pipes (`|`), background jobs (`&`)              | 🔜     |

---

<a name="installation"></a>
## 3. Installation 💻

### Prerequisites:
- GCC compiler
- GNU Readline library
- Linux/macOS environment

### Build Instructions:

```bash
# Clone repository
git clone https://github.com/yourusername/marksh.git
cd marksh

# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential libreadline-dev

# Compile project
gcc src/*.c -Iinclude -o marksh -lreadline

# Run shell
./marksh
```
<img width="3516" height="702" alt="image" src="https://github.com/user-attachments/assets/0f0cfebf-9233-44a3-a472-aa6c34452f0f" />

<img width="2090" height="3457" alt="image" src="https://github.com/user-attachments/assets/08dfe72f-3002-40fb-bd4b-ae5176713916" />



<a name="technical-deep-dive"></a>

4. How Mark Shell Works 🧠
4.1 Core REPL Loop
Mark Shell operates as a REPL (Read-Evaluate-Print Loop):
```bash
int main() {
    char *line;
    char **args;
    while (1) {
        printf("marksh> ");
        line = mysh_read_line();   // read input
        args = mysh_split_line(line); // parse into tokens
        mysh_execute(args);        // execute command
        free(line);
        free(args);
    }
}
```

4.2 Command Parsing
Input is tokenized into commands, arguments, and operators (>, <, &&, ||, ;):
```bash
tokens = {
  {type: COMMAND, value: "ls"},
  {type: ARG, value: "-l"},
  {type: REDIR_OUT, value: ">"},
  {type: FILE, value: "out.txt"},
  {type: AND, value: "&&"},
  {type: COMMAND, value: "cat"},
  {type: ARG, value: "out.txt"}
};
```
Command chaining logic:
&& → Execute next command only if previous succeeds
|| → Execute next command only if previous fails
; → Always execute next command

4.3 Process Management
Child processes handle command execution using fork() and execvp():
```bash
pid_t pid = fork();
if (pid == 0) {  // Child
    execvp(args[0], args);
    perror("marksh");  // if exec fails
    exit(EXIT_FAILURE);
} else if (pid > 0) {  // Parent
    int status;
    waitpid(pid, &status, 0);  // Wait for child
}
```
Parent shell remains alive while child processes execute.
Kernel manages process isolation and resources.
4.4 I/O Redirection
Example: ls > out.txt
```bash
int fd = open("out.txt", O_WRONLY|O_CREAT, 0644);
dup2(fd, STDOUT_FILENO);  // Redirect stdout
close(fd);
execvp("ls", args);       // Output goes to file
```
4.5 Signal Handling (Ctrl+C)
Stops the child process but keeps shell running:
```bash
void sigint_handler(int sig) {
    if (child_pid > 0)
        kill(child_pid, SIGINT);  // Forward to child
    else
        write(STDOUT_FILENO, "\nmarksh> ", 9); // New prompt
}
signal(SIGINT, sigint_handler);
4.6 Built-in Commands
cd Implementation:
int mysh_cd(char **args) {
    char *path = args[1] ? args[1] : getenv("HOME");
    if (chdir(path) != 0) perror("marksh");
    char cwd[1024]; getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);
    return 1;
}
Custom mark help command:
int mysh_help(char **args) {
    printf("Mark Shell Help:\n");
    printf("  cd [dir], mkdir [name], pwd, ls, exit, help\n");
    printf("Features: History, Tab completion, Redirection, Command chaining, Ctrl+C\n");
    return 1;
}
```
4.7 Command History & Editing
Up/Down arrows: Traverse history[] buffer
Backspace: Delete characters while typing
Tab: Complete command or filename dynamically
<a name="usage-examples"></a>
5. Usage Examples 🖥️
```bash
# Built-in commands
marksh> pwd
/home/user/projects

marksh> mkdir new_folder
marksh> cd new_folder

# Redirection
marksh> ls > files.txt
marksh> cat < files.txt

# Command chaining
marksh> mkdir test && cd test || echo "Failed"

# Custom help command
marksh> help
Signal handling:
marksh> sleep 30
^C  # Ctrl+C stops sleep but shell continues
<a name="future-roadmap"></a>
```
6. Future Enhancements 🔮
Pipes (|) and background jobs (&)
Persistent command history (save/load from file)
Syntax highlighting and smarter auto-completion
Environment variable expansion ($HOME, $PATH)
<a name="contributing"></a>

7. Contributing 🤝
Fork the repository
Create a new branch for features/fixes
Commit your changes with clear messages
Submit a Pull Request
