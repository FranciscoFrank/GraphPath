# 🤝 Contributing to GraphPath

## 🎯 Як допомогти

### 1. Додати новий алгоритм
- Створіть `include/newalgo.h` та `src/newalgo.c`
- Використайте `PathResult` як інтерфейс
- Додайте тести в `tests/test_graph.c`
- Оновіть документацію

### 2. Виправити баг
- Створіть issue з описом
- Додайте тест, що відтворює баг
- Виправте та перевірте, що тест проходить

### 3. Покращити документацію
- README, TECHNICAL, QUICKSTART, EXAMPLES або інші MD файли
- Коментарі в коді
- Приклади використання

### 4. Додати тести
- Більше edge cases
- Benchmarks
- Performance tests

## 📝 Coding Style

- **Стандарт:** C11
- **Відступи:** 4 spaces (not tabs)
- **Імена:** snake_case для функцій/змінних
- **Warnings:** Код має компілюватись без warnings

## 🧪 Testing

Перед commit:
```bash
make clean
make
make test
```

> [!IMPORTANT]
> 
> Всі тести мають проходити ✓

## 📚 Приклад додавання алгоритму

**1. Header (`include/astar.h`):**
```c
#ifndef ASTAR_H
#define ASTAR_H
#include "graph.h"

PathResult* astar_find_path(const Graph* graph, int start, int end);

#endif
```

**2. Implementation (`src/astar.c`):**
```c
#include "astar.h"
#include <time.h>

PathResult* astar_find_path(const Graph* graph, int start, int end) {
    PathResult* result = path_result_create();
    result->algorithm = "A*";
    
    clock_t start_time = clock();
    // ... your algorithm ...
    result->time_ms = ((double)(clock() - start_time) / CLOCKS_PER_SEC) * 1000.0;
    
    return result;
}
```

**3. Add to selector (`main.c`):**
```c
if (has_heuristic) {
    algorithms[count++] = astar_find_path;
}
```

**4. Update Makefile:**
```makefile
SOURCES = ... src/astar.c
```

**5. Add test (`tests/test_graph.c`):**
```c
void test_astar(void) {
    // ... test code ...
}
```