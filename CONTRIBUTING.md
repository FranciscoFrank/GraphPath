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

### 3. Розширити GUI
- Додати новий віджет або панель
- Покращити візуалізацію графів
- Додати нову функціональність інтерфейсу
- Покращити UX/UI

### 4. Покращити документацію
- README, TECHNICAL, EXAMPLES або інші MD файли
- Коментарі в коді
- Приклади використання

### 5. Додати тести
- Більше edge cases
- GUI тести
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

## Додавання GUI компонентів

### Структура GUI

GUI проєкт знаходиться в директорії `gui/`:
- `gui/include/` - заголовкові файли Qt
- `gui/src/` - реалізація Qt компонентів
- `gui/graphpath.pro` - Qt проектний файл

### Додавання нового віджету

**1. Створіть заголовковий файл (`gui/include/NewWidget.h`):**
```cpp
#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QWidget>

class NewWidget : public QWidget {
    Q_OBJECT

public:
    explicit NewWidget(QWidget *parent = nullptr);
    ~NewWidget();

signals:
    void somethingChanged();

public slots:
    void handleUpdate();

private:
    // Private members
};

#endif
```

**2. Створіть реалізацію (`gui/src/NewWidget.cpp`):**
```cpp
#include "NewWidget.h"
#include <QVBoxLayout>

NewWidget::NewWidget(QWidget *parent) : QWidget(parent) {
    // Initialize UI
    QVBoxLayout *layout = new QVBoxLayout(this);
    // Add widgets to layout
}

NewWidget::~NewWidget() {
    // Cleanup if needed
}

void NewWidget::handleUpdate() {
    // Implementation
    emit somethingChanged();
}
```

**3. Оновіть `gui/graphpath.pro`:**
```qmake
HEADERS += \
    include/MainWindow.h \
    include/NewWidget.h \
    ...

SOURCES += \
    src/main.cpp \
    src/NewWidget.cpp \
    ...
```

**4. Додайте віджет до MainWindow:**
```cpp
// In MainWindow.h
#include "NewWidget.h"

private:
    NewWidget *newWidget;

// In MainWindow.cpp
newWidget = new NewWidget(this);
layout->addWidget(newWidget);
connect(newWidget, &NewWidget::somethingChanged,
        this, &MainWindow::handleChange);
```

### Збірка GUI після змін

```bash
make clean-gui
make gui
make run-gui
```

### GUI Coding Style

- **Стандарт:** C++17
- **Qt версія:** Qt6 (або Qt5 для сумісності)
- **Відступи:** 4 spaces
- **Імена:**
  - Класи: `PascalCase` (наприклад, `GraphEditorWidget`)
  - Методи: `camelCase` (наприклад, `updateGraph()`)
  - Сигнали: `camelCase` (наприклад, `graphChanged()`)
  - Слоти: `handle*` або `on*` (наприклад, `handleClick()`)

### Тестування GUI

Додайте тести в `tests/test_gui.cpp`:
```cpp
#include <QtTest>
#include "NewWidget.h"

class TestNewWidget : public QObject {
    Q_OBJECT

private slots:
    void testInitialization();
    void testFunctionality();
};

void TestNewWidget::testInitialization() {
    NewWidget widget;
    QVERIFY(widget.isValid());
}

// ... more tests
```

### Інтеграція з C кодом

Для роботи з графом використовуйте `GraphWrapper`:

```cpp
#include "GraphWrapper.h"

// In your widget
void NewWidget::processGraph(GraphWrapper *wrapper) {
    if (!wrapper || !wrapper->isValid()) {
        return;
    }

    // Get graph data
    int vertices = wrapper->getVertexCount();
    // ... work with graph
}
```

### Корисні ресурси

> [!TIP]
>
> - **Докладна документація GUI:** [GUI.md](GUI.md)
> - **Make команди:** [MAKECOMMANDS.md](MAKECOMMANDS.md)
> - **Технічна документація:** [TECHNICAL.md](TECHNICAL.md)
> - **Qt документація:** https://doc.qt.io/

## 🔧 Інші корисні команди

Всі make команди описані в [MAKECOMMANDS.md](MAKECOMMANDS.md)

```bash
# CLI
make          # Зібрати CLI
make run      # Запустити CLI
make test     # Запустити тести

# GUI
make gui      # Зібрати GUI
make run-gui  # Запустити GUI

# Очищення
make clean      # Очистити CLI
make clean-gui  # Очистити GUI
make clean-all  # Очистити все
```

## 📋 Checklist перед Pull Request

> [!IMPORTANT]
> 
> - [ ] Код компілюється без warnings (`make` та `make gui`)
> - [ ] Всі існуючі тести проходять (`make test`)
> - [ ] Додано тести для нової функціональності
> - [ ] Оновлено документацію (README, TECHNICAL, GUI.md)
> - [ ] Код відповідає стилю проєкту
> - [ ] Commit messages зрозумілі та описові
> - [ ] GUI тести проходять (якщо застосовно)

## 🤔 Питання?

Створіть issue на GitHub з міткою `question` або `help wanted`.
