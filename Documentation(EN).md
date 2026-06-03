# 🐺 C-- — Official Language Documentation

**Version:** 1.0  
**Author:** Andrey
**Philosophy:** C on steroids, but without strong steroids

---

## 📖 1. VARIABLES

| Command | Description | Example |
|---------|-------------|---------|
| `let name` | Empty variable (Null Variable) | `let name` |
| `let name = value` | Immutable variable | `let max = 100` |
| `var name` | Empty mutable variable | `var counter` |
| `var name = value` | Mutable variable | `var score = 0` |
| `name = value` | Assignment | `name = "Andrey"` |

### VariableEdit

| Command | Description | Example |
|---------|-------------|---------|
| `ClearVariable("name")` | Clears variable (to Null Variable) | `ClearVariable("result")` |
| `RemoveVariable("name")` | Completely removes variable | `RemoveVariable("temp")` |

---

## 📊 2. DATA TYPES

| Type | Stores | Example |
|------|--------|---------|
| `int` | Integer numbers | `let age = 10` |
| `float` | Floating point numbers | `let pi = 3.14` |
| `char` | String (text) | `let name = "Wolf"` |
| `boolean` | True / False | `let isReady = true` |

---

## 📥📤 3. INPUT / OUTPUT

| Command | Description | Example |
|---------|-------------|---------|
| `say!(...)` | Print to screen | `say!("Hello ", name)` |
| `input("type", var)` | Type-safe input | `input("int", age)` |
| `waitEnter()` | Wait for Enter key | `waitEnter()` |
| `clearScreen()` | Clear screen | `clearScreen()` |

### Input types:
- `"char"` — string (text)
- `"int"` — integer
- `"float"` — floating point
- `"boolean"` — `true` or `false`

### Type checking after input (optional):
```c--
var age
input("int", age)

if age != "int" {
    say!("Error: please enter a number!")
} else {
    say!("You are ", age, " years old")
}
```

---

⏱️ 4. TIME & PAUSES

Command Description Example
sleep(sec) Pause in seconds sleep(3)
Sleep(ms) Pause in milliseconds Sleep(500)

---

📁 5. FILE OPERATIONS

Command Description Example
createFile("file.txt", data) Create file with content createFile("log.txt", "start")
writeFile("file.txt", data) Overwrite file writeFile("log.txt", "new text")
updateFile("file.txt", data) Append to file updateFile("log.txt", "new line")
readFile("file.txt") Read file let data = readFile("config.txt")
removeFile("file.txt") Delete file removeFile("old.txt")
fileExists("file.txt") Check if file exists if fileExists("data.txt") == true

⚠️ FILE ERROR HANDLING

After each file operation, check if an error occurred.

Syntax:

```c--
if filename == error {
    // error handling
}
```

Example:

```c--
createFile("backup.txt", "data")
if backup.txt == error {
    say!("Error: cannot create file!")
}
```

---

🧮 6. ARITHMETIC & COMPARISON

6.1. Operators

Operator Description Example
+ Addition x + y
- Subtraction x - y
* Multiplication x * y
/ Division x / y
= Assignment x = 5
== Equal if name == "Andrey"
!= Not equal if x != y
> Greater than if age > 18
< Less than if age < 18
>= Greater or equal if age >= 18
<= Less or equal if age <= 18
and / && Logical AND if age >= 18 and hasLicense == true
or / \|\| Logical OR if isStudent or isWeekend
! Logical NOT if !isReady

6.2. Increment and Decrement

Operator Description Example
++ Increases variable by 1 counter++
-- Decreases variable by 1 counter--

Example:

```c--
var counter = 0
counter++
say!(counter)   // 1
```

Important: Work only with var (mutable variables).

---

🔁 7. CONDITIONALS

Command Description Example
if condition { } If condition is true if age >= 18 { say!("Adult") }
else if condition { } Else if else if age >= 16 { say!("Almost") }
else { } Else else { say!("Young") }

---

🔄 8. LOOPS

Command Description Example
while condition { } Repeat while true while counter < 10 { }
while true { } Infinite loop while true { }
break Exit loop if x == 5 { break }
continue Skip to next iteration if x % 2 == 0 { continue }
for i in start..end { } Range loop for i in 1..5 { say!(i) }
for item in array { } Array loop for fruit in fruits { say!(fruit) }

---

📦 9. ARRAYS

Command Description Example
let arr = [a, b, c] Immutable array let nums = [1, 2, 3]
var arr = [a, b] Mutable array var items = ["code", "wolf"]
arr[index] Access element (0‑based) say!(nums[0])
arr.add(value) Add element items.add("moon")
arr.remove[index] Remove element items.remove[1]

Indices start at 0: first element is index 0, second is index 1, etc.

---

⚙️ 10. FUNCTIONS

Command Description Example
fn name(params) { } Function declaration fn add(a, b) { return a + b }
return value Return value return x + y
name(args) Function call add(4, 4)

Lazy Call (unique C-- feature)

```c--
fn add(x, y) {
    return x + y
}

fn main() {
    lazy sum = add(4, 4)   // not executed yet
    say!(sum)              // executed here (prints 8)
}
```

---

💬 11. COMMENTS

Type Syntax Example
Single-line // text // this is a comment
Single-line # text # also a comment

---

🔗 12. C INJECTION

Write raw C code directly inside C--.

The three pillars are already included:

· stdio.h
· stdlib.h
· string.h

Syntax:

```c--
C {
    // any C code
}
```

Example:

```c--
fn main() {
    let name = "Andrey"
    let age = 10
    
    C {
        printf("Name: %s\n", name);
        printf("Age: %d\n", age);
    }
}
```

⚠️ Warning: If the entire program is C injection, the Wolf gets offended.

---

🏆 UNIQUE C-- FEATURES

1. Null Variable

A variable can exist without a value.

```c--
let name      // Null Variable
input("char", name)   // now name = "Andrey"
```

2. Lazy Call

A function call can be stored in a variable and executed only when used.

```c--
lazy result = add(4, 4)
say!(result)   // executed here
```

3. VariableEdit

```c--
ClearVariable("name")   // reset to Null Variable
RemoveVariable("temp")  // completely delete
```

---

✅ COMPLETE EXAMPLE

```c--
// Todo list program
# author: Andrey

fn main() {
    var task
    var choice
    
    while true {
        say!("\n===== TODO LIST =====")
        say!("1. Show tasks")
        say!("2. Add task")
        say!("3. Clear all")
        say!("4. Exit")
        input("int", choice)
        
        if choice == 1 {
            if fileExists("tasks.txt") == true {
                let tasks = readFile("tasks.txt")
                if tasks.txt == error {
                    say!("Error reading file!")
                } else {
                    say!("Tasks:\n", tasks)
                }
            } else {
                say!("No tasks")
            }
            waitEnter()
            clearScreen()
            
        } else if choice == 2 {
            say!("Enter task")
            input("char", task)
            updateFile("tasks.txt", task + "\n")
            if tasks.txt == error {
                say!("Error saving task!")
            } else {
                say!("Added!")
            }
            sleep(1)
            clearScreen()
            
        } else if choice == 3 {
            writeFile("tasks.txt", "")
            if tasks.txt == error {
                say!("Error clearing file!")
            } else {
                say!("All cleared!")
            }
            sleep(1)
            clearScreen()
            
        } else if choice == 4 {
            say!("Goodbye, commander!")
            break
            
        } else {
            say!("Unknown command")
            sleep(1)
            clearScreen()
        }
    }
}
‘‘‘
