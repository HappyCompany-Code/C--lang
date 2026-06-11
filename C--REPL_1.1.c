#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

#define MAX_INPUT 1024
#define MAX_VARS 100
#define MAX_FUNCTIONS 50
#define MAX_BODY_LINES 1000
#define MAX_LINE_LEN 512

typedef struct {
    char name[64];
    char value[512];
    char type[16];
    int isNull;
} Variable;

typedef struct {
    char name[64];
    char params[10][64];
    int paramCount;
    char body[MAX_BODY_LINES][MAX_LINE_LEN];
    int bodyLines;
} Function;

Variable vars[MAX_VARS];
int varCount = 0;

Function functions[MAX_FUNCTIONS];
int funcCount = 0;

int isExecuting = 0;

void trim(char *s) {
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

void addVar(char *name, char *value, char *type) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            strcpy(vars[i].value, value);
            strcpy(vars[i].type, type);
            vars[i].isNull = (strcmp(value, "null") == 0);
            return;
        }
    }
    if (varCount < MAX_VARS) {
        strcpy(vars[varCount].name, name);
        strcpy(vars[varCount].value, value);
        strcpy(vars[varCount].type, type);
        vars[varCount].isNull = (strcmp(value, "null") == 0);
        varCount++;
    }
}

char* getVar(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            if (vars[i].isNull) return NULL;
            return vars[i].value;
        }
    }
    return NULL;
}

void clearVariable(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].isNull = 1;
            strcpy(vars[i].value, "null");
            return;
        }
    }
}

void removeVariable(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            for (int j = i; j < varCount - 1; j++) {
                vars[j] = vars[j + 1];
            }
            varCount--;
            return;
        }
    }
}

int evaluateCondition(char *expr) {
    char var1[64], op[8], var2[64];
    char val1[512], val2[512];
    
    sscanf(expr, "%s %s %s", var1, op, var2);
    
    char *v1 = getVar(var1);
    if (v1) strcpy(val1, v1);
    else strcpy(val1, var1);
    
    char *v2 = getVar(var2);
    if (v2) strcpy(val2, v2);
    else strcpy(val2, var2);
    
    int i1, i2;
    float f1, f2;
    
    if (sscanf(val1, "%d", &i1) == 1 && sscanf(val2, "%d", &i2) == 1) {
        if (strcmp(op, "==") == 0) return i1 == i2;
        if (strcmp(op, "!=") == 0) return i1 != i2;
        if (strcmp(op, ">") == 0) return i1 > i2;
        if (strcmp(op, "<") == 0) return i1 < i2;
        if (strcmp(op, ">=") == 0) return i1 >= i2;
        if (strcmp(op, "<=") == 0) return i1 <= i2;
    }
    
    if (sscanf(val1, "%f", &f1) == 1 && sscanf(val2, "%f", &f2) == 1) {
        if (strcmp(op, "==") == 0) return f1 == f2;
        if (strcmp(op, "!=") == 0) return f1 != f2;
        if (strcmp(op, ">") == 0) return f1 > f2;
        if (strcmp(op, "<") == 0) return f1 < f2;
        if (strcmp(op, ">=") == 0) return f1 >= f2;
        if (strcmp(op, "<=") == 0) return f1 <= f2;
    }
    
    if (strcmp(op, "==") == 0) return strcmp(val1, val2) == 0;
    if (strcmp(op, "!=") == 0) return strcmp(val1, val2) != 0;
    
    return 0;
}

void executeLine(char *line, int *shouldBreak, int *shouldContinue);

void executeBlock(char block[MAX_BODY_LINES][MAX_LINE_LEN], int blockLines) {
    int shouldBreak = 0;
    int shouldContinue = 0;
    
    for (int i = 0; i < blockLines; i++) {
        if (shouldBreak) break;
        if (shouldContinue) {
            shouldContinue = 0;
            continue;
        }
        executeLine(block[i], &shouldBreak, &shouldContinue);
    }
}

void executeLine(char *line, int *shouldBreak, int *shouldContinue) {
    if (strlen(line) == 0) return;
    
    if (line[0] == '#' || (line[0] == '/' && line[1] == '/')) return;
    
    if (strncmp(line, "say!", 4) == 0) {
        char *start = strchr(line, '"');
        char *end = NULL;
        if (start) {
            end = strchr(start + 1, '"');
        }
        if (start && end) {
            *end = '\0';
            printf("%s\n", start + 1);
        } else {
            char var[64];
            sscanf(line, "say!(%s", var);
            char *p = strchr(var, ')');
            if (p) *p = '\0';
            char *val = getVar(var);
            if (val) printf("%s\n", val);
            else printf("❌ Переменная %s не найдена\n", var);
        }
        return;
    }
    
    if (strncmp(line, "input", 5) == 0) {
        char type[16], var[64];
        sscanf(line, "input(%s, %s", type, var);
        char *p = strchr(var, ')');
        if (p) *p = '\0';
        
        char buffer[512];
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(type, "int") == 0) {
            int val;
            if (sscanf(buffer, "%d", &val) == 1) sprintf(buffer, "%d", val);
            else {
                printf("❌ Ошибка: нужно ввести число!\n");
                strcpy(buffer, "0");
            }
        } else if (strcmp(type, "float") == 0) {
            float val;
            if (sscanf(buffer, "%f", &val) == 1) sprintf(buffer, "%f", val);
            else {
                printf("❌ Ошибка: нужно ввести число!\n");
                strcpy(buffer, "0");
            }
        }
        
        addVar(var, buffer, type);
        return;
    }
    
    if (strncmp(line, "sleep", 5) == 0) {
        int sec = 0;
        char *p = strchr(line, '(');
        if (p) sec = atoi(p + 1);
        SLEEP_MS(sec * 1000);
        return;
    }
    
    if (strcmp(line, "waitEnter()") == 0 || strcmp(line, "waitEnter") == 0) {
        printf("Нажми Enter...");
        getchar();
        getchar();
        return;
    }
    
    if (strcmp(line, "clearScreen()") == 0 || strcmp(line, "clearScreen") == 0) {
        system("clear");
        return;
    }
    
    if (strncmp(line, "ClearVariable", 13) == 0) {
        char name[64];
        sscanf(line, "ClearVariable(\"%s\"", name);
        char *p = strchr(name, '"');
        if (p) *p = '\0';
        clearVariable(name);
        return;
    }
    
    if (strncmp(line, "RemoveVariable", 14) == 0) {
        char name[64];
        sscanf(line, "RemoveVariable(\"%s\"", name);
        char *p = strchr(name, '"');
        if (p) *p = '\0';
        removeVariable(name);
        return;
    }
    
    if (strncmp(line, "if", 2) == 0) {
        char condition[256];
        char *brace = strchr(line, '{');
        if (brace) {
            int len = brace - line - 2;
            strncpy(condition, line + 3, len);
            condition[len] = '\0';
            trim(condition);
            
            char block[MAX_BODY_LINES][MAX_LINE_LEN];
            int blockLines = 0;
            int braceCount = 1;
            char tempLine[MAX_LINE_LEN];
            
            while (braceCount > 0) {
                if (!fgets(tempLine, sizeof(tempLine), stdin)) break;
                trim(tempLine);
                if (strchr(tempLine, '{')) braceCount++;
                if (strchr(tempLine, '}')) braceCount--;
                if (braceCount >= 0 && strlen(tempLine) > 0 && !strchr(tempLine, '}')) {
                    strcpy(block[blockLines++], tempLine);
                }
            }
            
            if (evaluateCondition(condition)) {
                executeBlock(block, blockLines);
            }
        }
        return;
    }
    
    if (strncmp(line, "while", 5) == 0) {
        char condition[256];
        char *brace = strchr(line, '{');
        if (brace) {
            int len = brace - line - 5;
            strncpy(condition, line + 6, len);
            condition[len] = '\0';
            trim(condition);
            
            char block[MAX_BODY_LINES][MAX_LINE_LEN];
            int blockLines = 0;
            int braceCount = 1;
            char tempLine[MAX_LINE_LEN];
            
            while (braceCount > 0) {
                if (!fgets(tempLine, sizeof(tempLine), stdin)) break;
                trim(tempLine);
                if (strchr(tempLine, '{')) braceCount++;
                if (strchr(tempLine, '}')) braceCount--;
                if (braceCount >= 0 && strlen(tempLine) > 0 && !strchr(tempLine, '}')) {
                    strcpy(block[blockLines++], tempLine);
                }
            }
            
            while (evaluateCondition(condition)) {
                executeBlock(block, blockLines);
                if (shouldBreak && *shouldBreak) {
                    *shouldBreak = 0;
                    break;
                }
            }
        }
        return;
    }
    
    if (strncmp(line, "for", 3) == 0 && strstr(line, "..")) {
        char var[64], start[16], end[16];
        sscanf(line, "for %s in %s .. %s {", var, start, end);
        
        int s = atoi(start);
        int e = atoi(end);
        
        char block[MAX_BODY_LINES][MAX_LINE_LEN];
        int blockLines = 0;
        int braceCount = 1;
        char tempLine[MAX_LINE_LEN];
        
        while (braceCount > 0) {
            if (!fgets(tempLine, sizeof(tempLine), stdin)) break;
            trim(tempLine);
            if (strchr(tempLine, '{')) braceCount++;
            if (strchr(tempLine, '}')) braceCount--;
            if (braceCount >= 0 && strlen(tempLine) > 0 && !strchr(tempLine, '}')) {
                strcpy(block[blockLines++], tempLine);
            }
        }
        
        char valStr[16];
        for (int i = s; i <= e; i++) {
            sprintf(valStr, "%d", i);
            addVar(var, valStr, "int");
            executeBlock(block, blockLines);
        }
        return;
    }
    
    if (strcmp(line, "break") == 0) {
        if (shouldBreak) *shouldBreak = 1;
        return;
    }
    
    if (strcmp(line, "continue") == 0) {
        if (shouldContinue) *shouldContinue = 1;
        return;
    }
    
    if (strncmp(line, "let ", 4) == 0 || strncmp(line, "var ", 4) == 0) {
        char name[64], value[256];
        if (strchr(line, '=')) {
            sscanf(line, "%*s %s = %[^\n]", name, value);
            trim(name);
            trim(value);
            addVar(name, value, "unknown");
        } else {
            sscanf(line, "%*s %s", name);
            addVar(name, "null", "unknown");
        }
        return;
    }
    
    if (strstr(line, "++") && !strstr(line, "==")) {
        char name[64];
        sscanf(line, "%s++", name);
        char *val = getVar(name);
        if (val) {
            int num = atoi(val);
            num++;
            char newVal[16];
            sprintf(newVal, "%d", num);
            addVar(name, newVal, "int");
        }
        return;
    }
    
    if (strstr(line, "--") && !strstr(line, "==")) {
        char name[64];
        sscanf(line, "%s--", name);
        char *val = getVar(name);
        if (val) {
            int num = atoi(val);
            num--;
            char newVal[16];
            sprintf(newVal, "%d", num);
            addVar(name, newVal, "int");
        }
        return;
    }
    
    if (strchr(line, '=') && !strstr(line, "==") && !strstr(line, "!=")) {
        char name[64], value[256];
        sscanf(line, "%s = %[^\n]", name, value);
        trim(name);
        trim(value);
        if (getVar(name) == NULL && !isExecuting) addVar(name, value, "unknown");
        else addVar(name, value, "unknown");
        return;
    }
    
    char funcName[64];
    sscanf(line, "%s", funcName);
    char *paren = strchr(funcName, '(');
    if (paren) *paren = '\0';
    
    for (int i = 0; i < funcCount; i++) {
        if (strcmp(functions[i].name, funcName) == 0) {
            Variable savedVars[MAX_VARS];
            int savedVarCount = varCount;
            memcpy(savedVars, vars, sizeof(Variable) * varCount);
            
            isExecuting = 1;
            for (int j = 0; j < functions[i].bodyLines; j++) {
                char lineCopy[MAX_LINE_LEN];
                strcpy(lineCopy, functions[i].body[j]);
                executeLine(lineCopy, shouldBreak, shouldContinue);
            }
            isExecuting = 0;
            
            varCount = savedVarCount;
            memcpy(vars, savedVars, sizeof(Variable) * savedVarCount);
            return;
        }
    }
}

void saveFunction(char *line) {
    char name[64];
    sscanf(line, "fn %s", name);
    char *paren = strchr(name, '(');
    if (paren) *paren = '\0';
    
    Function *f = &functions[funcCount];
    strcpy(f->name, name);
    f->paramCount = 0;
    f->bodyLines = 0;
    
    char *paramsStart = strchr(line, '(');
    char *paramsEnd = strchr(line, ')');
    if (paramsStart && paramsEnd) {
        *paramsEnd = '\0';
        char paramsStr[256];
        strcpy(paramsStr, paramsStart + 1);
        char *token = strtok(paramsStr, ",");
        while (token) {
            trim(token);
            strcpy(f->params[f->paramCount++], token);
            token = strtok(NULL, ",");
        }
    }
    
    char tempLine[MAX_LINE_LEN];
    int braceCount = 1;
    while (braceCount > 0 && fgets(tempLine, sizeof(tempLine), stdin)) {
        trim(tempLine);
        if (strchr(tempLine, '{')) braceCount++;
        if (strchr(tempLine, '}')) braceCount--;
        if (braceCount > 0 && strlen(tempLine) > 0 && !strchr(tempLine, '}')) {
            strcpy(f->body[f->bodyLines++], tempLine);
        }
    }
    
    funcCount++;
    printf("✅ Функция '%s' сохранена (%d строк)\n", name, f->bodyLines);
}

void executeMain() {
    for (int i = 0; i < funcCount; i++) {
        if (strcmp(functions[i].name, "main") == 0) {
            printf("\n🚀 Выполнение main():\n");
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            isExecuting = 1;
            int sb = 0, sc = 0;
            for (int j = 0; j < functions[i].bodyLines; j++) {
                char lineCopy[MAX_LINE_LEN];
                strcpy(lineCopy, functions[i].body[j]);
                executeLine(lineCopy, &sb, &sc);
            }
            isExecuting = 0;
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            return;
        }
    }
    printf("❌ Функция main() не найдена!\n");
}

void showHelp() {
    printf("\n📖 C-- ЯЗЫК ПРОГРАММИРОВАНИЯ v1.1\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("📝 ОСНОВНЫЕ КОМАНДЫ:\n");
    printf("  let x = 10              - неизменяемая переменная\n");
    printf("  var x = 10              - изменяемая переменная\n");
    printf("  say!(\"текст\")          - вывод\n");
    printf("  input(int, x)           - ввод числа\n");
    printf("  x++ / x--               - инкремент / декремент\n");
    printf("\n🎯 УСЛОВИЯ И ЦИКЛЫ:\n");
    printf("  if x > 10 { ... }       - условие\n");
    printf("  while x < 10 { ... }    - цикл\n");
    printf("  for i in 1..5 { ... }   - цикл for\n");
    printf("\n🔧 ФУНКЦИИ:\n");
    printf("  fn name() { ... }       - объявление функции\n");
    printf("  fn main() { ... }       - точка входа\n");
    printf("  lazy name = func()      - отложенный вызов\n");
    printf("\n💡 КОМАНДЫ REPL:\n");
    printf("  vars         - показать переменные\n");
    printf("  funcs        - показать функции\n");
    printf("  run          - выполнить main()\n");
    printf("  help         - справка\n");
    printf("  exit         - выход\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void showFuncs() {
    if (funcCount == 0) {
        printf("📭 Нет функций\n");
        return;
    }
    printf("\n🔧 ФУНКЦИИ:\n");
    for (int i = 0; i < funcCount; i++) {
        printf("  • %s(", functions[i].name);
        for (int j = 0; j < functions[i].paramCount; j++) {
            printf("%s", functions[i].params[j]);
            if (j < functions[i].paramCount - 1) printf(", ");
        }
        printf(") - %d строк\n", functions[i].bodyLines);
    }
    printf("\n");
}

void showVars() {
    if (varCount == 0) {
        printf("📭 Нет переменных\n");
        return;
    }
    printf("\n📦 ПЕРЕМЕННЫЕ:\n");
    for (int i = 0; i < varCount; i++) {
        if (vars[i].isNull) printf("  • %s = (null) [%s]\n", vars[i].name, vars[i].type);
        else printf("  • %s = \"%s\" [%s]\n", vars[i].name, vars[i].value, vars[i].type);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT];
    
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║     🐺 C-- FULL REPL v1.1 - Язык программирования           ║\n");
    printf("║     Автор: Андрей                                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\nВведите 'help' для списка команд\n");
    printf("Пример:\n");
    printf("  fn main() {\n");
    printf("      let age = 10\n");
    printf("      age++\n");
    printf("      say!(\"Возраст: \", age)\n");
    printf("  }\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
    
    while (1) {
        printf("C-- > ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) continue;
        
        if (strcmp(input, "help") == 0) {
            showHelp();
            continue;
        }
        if (strcmp(input, "vars") == 0) {
            showVars();
            continue;
        }
        if (strcmp(input, "funcs") == 0) {
            showFuncs();
            continue;
        }
        if (strcmp(input, "run") == 0) {
            executeMain();
            continue;
        }
        if (strcmp(input, "clear") == 0) {
            system("clear");
            continue;
        }
        if (strcmp(input, "exit") == 0) {
            printf("\n🐺 До свидания, командир!\n");
            break;
        }
        
        if (strncmp(input, "fn", 2) == 0) {
            saveFunction(input);
            continue;
        }
        
        int sb = 0, sc = 0;
        executeLine(input, &sb, &sc);
    }
    
    return 0;
}
