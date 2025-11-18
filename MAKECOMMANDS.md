# Make Команди

Цей документ містить опис всіх доступних make команд для збірки та запуску проєкту GraphPath.

## Швидкий старт

```bash
# CLI версія
make        # Зібрати CLI програму
make run    # Зібрати та запустити CLI

# GUI версія
make gui        # Зібрати GUI програму
make run-gui    # Зібрати та запустити GUI
```

## Команди CLI

### `make` (або `make all`)
**Призначення:** Компіляція CLI версії програми

**Що робить:**
- Компілює всі вихідні файли з `src/` та `include/`
- Створює виконуваний файл `build/graphpath`
- Використовує gcc з прапорцями: `-Wall -Wextra -std=c11 -O2`

**Приклад:**
```bash
make
```

### `make run`
**Призначення:** Збірка та запуск CLI програми

**Що робить:**
- Спочатку виконує `make` для компіляції
- Потім запускає `build/graphpath`

**Приклад:**
```bash
make run
```

**Використання програми:**
```bash
# Після збірки можна запускати напряму:
./build/graphpath < example_input.txt
./build/graphpath < examples/dijkstra_classic.txt
```

### `make test`
**Призначення:** Збірка та запуск модульних тестів

**Що робить:**
- Компілює тестовий файл `tests/test_graph.c`
- Створює виконуваний файл `build/test_graph`
- Автоматично запускає тести
- Виводить результати перевірок

**Приклад:**
```bash
make test
```

**Тести включають:**
- Створення графа
- Додавання ребер
- BFS алгоритм
- DFS алгоритм
- Dijkstra алгоритм
- Випадки відсутності шляху
- Обробка петель

### `make clean`
**Призначення:** Видалення файлів збірки CLI

**Що робить:**
- Видаляє всі `.o` об'єктні файли з `build/`
- Видаляє виконуваний файл `build/graphpath`
- Видаляє тестовий файл `build/test_graph`
- НЕ видаляє GUI файли збірки

**Приклад:**
```bash
make clean
```

---

## Команди GUI

### `make gui`
**Призначення:** Компіляція GUI версії програми

**Що робить:**
- Автоматично визначає доступну версію qmake (`qmake6` або `qmake`)
- Генерує Makefile з Qt конфігурації (`gui/graphpath.pro`)
- Компілює всі GUI компоненти з `gui/src/` та `gui/include/`
- Створює виконуваний файл `build/graphpath-gui`

**Вимоги:**
- Qt5 або Qt6 development packages
- qmake або qmake6

**Приклад:**
```bash
make gui
```

**Встановлення залежностей (якщо потрібно):**

Ubuntu/Debian:
```bash
sudo apt-get install qt6-base-dev qt6-tools-dev
# або для Qt5:
sudo apt-get install qtbase5-dev qttools5-dev
```

Fedora:
```bash
sudo dnf install qt6-qtbase-devel
# або для Qt5:
sudo dnf install qt5-qtbase-devel
```

Arch Linux:
```bash
sudo pacman -S qt6-base
# або для Qt5:
sudo pacman -S qt5-base
```

### `make run-gui`
**Призначення:** Збірка та запуск GUI програми

**Що робить:**
- Спочатку виконує `make gui` для компіляції
- Потім запускає `build/graphpath-gui`

**Приклад:**
```bash
make run-gui
```

**Використання програми:**
```bash
# Після збірки можна запускати напряму:
./build/graphpath-gui
```

### `make clean-gui`
**Призначення:** Видалення файлів збірки GUI

**Що робить:**
- Видаляє всі згенеровані Qt файли (`moc_*.cpp`, `*.o`)
- Видаляє виконуваний файл `build/graphpath-gui`
- Очищає директорії `gui/release/` та `gui/debug/`
- Видаляє згенеровані Makefile файли Qt
- НЕ видаляє CLI файли збірки

**Приклад:**
```bash
make clean-gui
```

---

## Загальні команди

### `make clean-all`
**Призначення:** Повне очищення всіх файлів збірки

**Що робить:**
- Виконує `make clean` (очищення CLI)
- Виконує `make clean-gui` (очищення GUI)
- Видаляє всю директорію `build/`

**Приклад:**
```bash
make clean-all
```

**Коли використовувати:**
- Перед чистою пересборкою проєкту
- При зміні конфігурації компіляції
- Для вирішення проблем зі збіркою
- Перед комітом у git (для очищення артефактів)

### `make help`
**Призначення:** Показати довідку по всім командам

**Що робить:**
- Виводить список всіх доступних команд
- Показує короткий опис кожної команди

**Приклад:**
```bash
make help
```

---

## Структура збірки

### Директорія `build/`

Після компіляції в директорії `build/` будуть створені:

```
build/
├── graphpath           # CLI виконуваний файл
├── graphpath-gui       # GUI виконуваний файл
├── test_graph          # Тестовий виконуваний файл
├── graph.o            # Об'єктний файл graph.c
├── bfs.o              # Об'єктний файл bfs.c
├── dfs.o              # Об'єктний файл dfs.c
├── dijkstra.o         # Об'єктний файл dijkstra.c
└── main.o             # Об'єктний файл main.c
```

---

## Конфігурація компілятора

### CLI компіляція
- **Компілятор:** gcc
- **Стандарт:** C11 (`-std=c11`)
- **Оптимізація:** O2 (`-O2`)
- **Попередження:** `-Wall -Wextra`
- **Бібліотеки:** `-lm` (математична бібліотека)
- **Include шляхи:** `-Iinclude`

### GUI компіляція
- **Фреймворк:** Qt6 (або Qt5)
- **Модулі:** QtCore, QtGui, QtWidgets
- **Стандарт C++:** C++17
- **Конфігурація:** Визначається в `gui/graphpath.pro`

---

## Типові сценарії використання

### 1. Перша збірка проєкту
```bash
# Зібрати CLI версію
make

# Зібрати GUI версію
make gui

# Запустити тести
make test
```

### 2. Розробка CLI функціоналу
```bash
# Редагувати код...
nano src/graph.c

# Пересібрати та запустити
make run < example_input.txt
```

### 3. Розробка GUI функціоналу
```bash
# Редагувати код...
nano gui/src/MainWindow.cpp

# Пересібрати та запустити
make run-gui
```

### 4. Проблеми зі збіркою
```bash
# Повне очищення
make clean-all

# Чиста збірка
make
make gui
```

### 5. Тестування змін
```bash
# Запустити модульні тести
make test

# Перевірити CLI з прикладами
make run < examples/dijkstra_classic.txt

# Перевірити GUI інтерфейс
make run-gui
```

---

## Залежності

### Для CLI:
- gcc (GNU Compiler Collection)
- GNU Make
- glibc (стандартна бібліотека C)

### Для GUI:
- gcc/g++ (з підтримкою C++17)
- GNU Make
- Qt5 або Qt6 development files
- qmake або qmake6

### Перевірка наявності залежностей:
```bash
# Перевірка gcc
gcc --version

# Перевірка make
make --version

# Перевірка qmake
qmake --version
# або
qmake6 --version

# Перевірка Qt
pkg-config --modversion Qt6Core
# або
pkg-config --modversion Qt5Core
```

---

## Усунення проблем

### Помилка: `qmake: command not found`
**Рішення:** Встановіть Qt development packages (див. секцію `make gui`)

### Помилка: `fatal error: graph.h: No such file or directory`
**Рішення:** Переконайтеся, що ви в кореневій директорії проєкту

### Помилка при збірці після оновлення коду
**Рішення:**
```bash
make clean-all
make
make gui
```

### GUI не запускається: помилка бібліотек Qt
**Рішення:**
```bash
# Перевірте наявність бібліотек Qt
ldd build/graphpath-gui

# Встановіть відсутні бібліотеки
sudo apt-get install qt6-base-dev
```

---

## Додаткова інформація

- Докладніше про архітектуру: [TECHNICAL.md](TECHNICAL.md)
- Опис GUI компонентів: [GUI.md](GUI.md)
- Приклади використання: [EXAMPLES.md](EXAMPLES.md)
- Можливі помилки: [POSSIBLEERRORS.md](POSSIBLEERRORS.md)
- Внесок у проєкт: [CONTRIBUTING.md](CONTRIBUTING.md)
