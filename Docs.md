🐺 C-- — официальная документация языка

Версия: 1.0
Автор: Андрей 
Философия: C на стероидах, но без сильных стероидов

---

📖 1. ПЕРЕМЕННЫЕ

Команда Что делает Пример
let name Пустая переменная (Null Variable) let name
let name = value Неизменяемая переменная let max = 100
var name Пустая изменяемая переменная var counter
var name = value Изменяемая переменная var score = 0
name = value Присваивание значения name = "Андрей"

Управление переменными (VariableEdit)

Команда Что делает Пример
ClearVariable("name") Очищает переменную (Null Variable) ClearVariable("result")
RemoveVariable("name") Полностью удаляет переменную RemoveVariable("temp")

---

📊 2. ТИПЫ ДАННЫХ

Тип Что хранит Пример
int Целое число let age = 10
float Дробное число let pi = 3.14
char Строка (текст) let name = "Волк"
boolean Истина/Ложь let isReady = true

---

📥📤 3. ВВОД / ВЫВОД

Команда Что делает Пример
say!(...) Вывод на экран say!("Привет, ", name)
input("тип", var) Ввод с проверкой типа input("int", age)
waitEnter() Ждёт нажатие Enter waitEnter()
clearScreen() Очищает экран clearScreen()

Типы для input:

· "char" — строка (текст)
· "int" — целое число
· "float" — дробное число
· "boolean" — true или false

Проверка типа после input:

```c--
var age
input("int", age)

if age != "int" {
    say!("Ошибка: нужно ввести число!")
} else {
    say!("Тебе ", age, " лет")
}
```

---

⏱️ 4. ВРЕМЯ И ПАУЗЫ

Команда Что делает Пример
sleep(сек) Пауза в секундах sleep(3)
Sleep(мс) Пауза в миллисекундах Sleep(500)

---

📁 5. РАБОТА С ФАЙЛАМИ

Команда Что делает Пример
createFile("file.txt", data) Создать файл с содержимым createFile("log.txt", "start")
writeFile("file.txt", data) Перезаписать файл writeFile("log.txt", "новый текст")
updateFile("file.txt", data) Добавить в конец updateFile("log.txt", "новая строка")
readFile("file.txt") Прочитать файл let data = readFile("config.txt")
removeFile("file.txt") Удалить файл removeFile("old.txt")
fileExists("file.txt") Проверить существование if fileExists("data.txt") == true

---

🧮 6. АРИФМЕТИКА И СРАВНЕНИЕ

Оператор Что делает Пример
+ Сложение x + y
- Вычитание x - y
* Умножение x * y
/ Деление x / y
= Присваивание x = 5
== Равно if name == "Андрей"
!= Не равно if x != y
> Больше if age > 18
< Меньше if age < 18
>= Больше или равно if age >= 18
<= Меньше или равно if age <= 18
and / && Логическое И if age >= 18 and hasLicense == true
or / `  `
! Логическое НЕ if !isReady

---

🔁 7. УСЛОВИЯ

Команда Что делает Пример
if condition { } Если условие истинно if age >= 18 { say!("Взрослый") }
else if condition { } Иначе если else if age >= 16 { say!("Почти") }
else { } Иначе else { say!("Молодой") }

---

🔄 8. ЦИКЛЫ

Команда Что делает Пример
while condition { } Повторять, пока истинно while counter < 10 { }
while true { } Бесконечный цикл while true { }
break Выйти из цикла if x == 5 { break }
continue Перейти к следующей итерации if x % 2 == 0 { continue }
for i in start..end { } Цикл по диапазону for i in 1..5 { say!(i) }
for item in array { } Цикл по массиву for fruit in fruits { say!(fruit) }

---

📦 9. МАССИВЫ

Команда Что делает Пример
let arr = [a, b, c] Неизменяемый массив let nums = [1, 2, 3]
var arr = [a, b] Изменяемый массив var items = ["код", "волк"]
arr[index] Доступ к элементу (с 0) say!(nums[0])
arr.add(value) Добавить элемент items.add("луна")
arr.remove[index] Удалить элемент items.remove[1]

Индексы считаются с 0: первый элемент — индекс 0, второй — индекс 1 и т.д.

---

⚙️ 10. ФУНКЦИИ

Команда Что делает Пример
fn name(params) { } Объявление функции fn add(a, b) { return a + b }
return value Возврат значения return x + y
name(args) Вызов функции add(4, 4)

Lazy Call (уникальная фишка C--)

```c--
fn add(x, y) {
    return x + y
}

fn main() {
    lazy sum = add(4, 4)   // не выполняется сразу
    say!(sum)              // выполняется здесь (выводит 8)
}
```

---

💬 11. КОММЕНТАРИИ

Тип Синтаксис Пример
Однострочный // текст // это комментарий
Однострочный # текст # тоже комментарий

---

🏆 УНИКАЛЬНЫЕ ФИШКИ C--

1. Null Variable (пустая переменная)

Переменная может существовать без значения. Заполняется позже.

```c--
let name      // Null Variable
input("char", name)   // теперь name = "Андрей"
```

2. Lazy Call (отложенный вызов)

Вызов функции сохраняется в переменную и выполняется только при использовании.

```c--
lazy result = add(4, 4)
say!(result)   // здесь выполняется
```

3. VariableEdit (управление переменными)

```c--
ClearVariable("name")   // очистить до Null Variable
RemoveVariable("temp")  // полностью удалить
```

---

✅ ПОЛНЫЙ ПРИМЕР ПРОГРАММЫ НА C--

```c--
// Программа: список дел
# автор: Андрей

fn main() {
    var task
    var choice
    
    while true {
        say!("\n===== СПИСОК ДЕЛ =====")
        say!("1. Показать задачи")
        say!("2. Добавить задачу")
        say!("3. Очистить всё")
        say!("4. Выход")
        input("int", choice)
        
        if choice == 1 {
            if fileExists("tasks.txt") == true {
                let tasks = readFile("tasks.txt")
                say!("Задачи:\n", tasks)
            } else {
                say!("Задач нет")
            }
            waitEnter()
            clearScreen()
            
        } else if choice == 2 {
            say!("Введи задачу")
            input("char", task)
            updateFile("tasks.txt", task + "\n")
            say!("Добавлено!")
            sleep(1)
            clearScreen()
            
        } else if choice == 3 {
            writeFile("tasks.txt", "")
            say!("Всё очищено!")
            sleep(1)
            clearScreen()
            
        } else if choice == 4 {
            say!("До свидания, командир!")
            break
            
        } else {
            say!("Неизвестная команда")
            sleep(1)
            clearScreen()
        }
    }
}
```

---
