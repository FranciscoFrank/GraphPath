# GraphPath GUI Documentation

## Зміст
- [Вступ](#вступ)
- [Встановлення та Запуск](#встановлення-та-запуск)
- [Інтерфейс Користувача](#інтерфейс-користувача)
- [Архітектура GUI](#архітектура-gui)
- [Компоненти](#компоненти)
- [Робочий Процес](#робочий-процес)
- [Розширення GUI](#розширення-gui)

---

## Вступ

GraphPath GUI - це графічний інтерфейс користувача для проекту GraphPath, який дозволяє візуально створювати графи, додавати ребра та виконувати алгоритми пошуку найкоротшого шляху з порівнянням продуктивності.

### Основні можливості

- ✅ **Інтерактивний редактор графів** - візуальне створення та редагування графів
- ✅ **Автоматична візуалізація** - кругова та сіткова розкладка вузлів
- ✅ **Підтримка всіх типів графів** - зважені/незважені, орієнтовані/неорієнтовані
- ✅ **Порівняння алгоритмів** - BFS vs DFS для незважених графів
- ✅ **Підсвічування шляху** - візуалізація знайденого найкоротшого шляху
- ✅ **Завантаження/збереження** - робота з файлами графів
- ✅ **Статистика продуктивності** - час виконання кожного алгоритму

---

## Встановлення та Запуск

### Вимоги

- **Qt 5** або **Qt 6** (Qt5Widgets або Qt6Widgets)
- **qmake** (зазвичай постачається разом з Qt)
- **Компілятор C/C++** (gcc, g++, clang)
- **make**

### Встановлення Qt на Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install qtbase5-dev qt5-qmake qtbase5-dev-tools
```

### Встановлення Qt на Fedora/RHEL

```bash
sudo dnf install qt5-qtbase-devel
```

### Встановлення Qt на Arch Linux

```bash
sudo pacman -S qt5-base
```

### Компіляція GUI

```bash
# З кореневої директорії проекту
make gui
```

Або вручну:

```bash
cd gui
qmake
make
cd ..
```

### Запуск GUI

```bash
make run-gui
```

Або напряму:

```bash
./build/graphpath-gui
```

### Очищення збірки

```bash
# Очистити тільки GUI
make clean-gui

# Очистити CLI та GUI
make clean-all
```

---

## Інтерфейс Користувача

### Макет вікна

```
┌─────────────────────────────────────────────────────────────┐
│ File   Edit   View   Help                                   │
├───────────────────────────────┬─────────────────────────────┤
│                               │  Graph Configuration        │
│                               │  ┌─────────────────────────┐│
│                               │  │ Vertices: [5]           ││
│                               │  │ Weighted: [ ]           ││
│                               │  │ Directed: [ ]           ││
│      Graph Editor             │  │ [Create Graph]          ││
│      (Візуалізація)           │  │ [Clear Graph]           ││
│                               │  └─────────────────────────┘│
│                               │                             │
│                               │  Add Edge                   │
│                               │  ┌─────────────────────────┐│
│                               │  │ From: [0]               ││
│                               │  │ To:   [1]               ││
│       [Граф малюється тут]    │  │ Weight: [1] (якщо треба)││
│                               │  │ [Add Edge]              ││
│                               │  └─────────────────────────┘│
│                               │                             │
│                               │  Find Path                  │
│                               │  ┌─────────────────────────┐│
│                               │  │ Start: [0]              ││
│                               │  │ End:   [4]              ││
│                               │  │ [Find Path]             ││
│                               │  └─────────────────────────┘│
│                               │                             │
│                               │  File Operations            │
│                               │  [Load Graph] [Save Graph]  │
│                               │                             │
│                               │  Layout                     │
│                               │  [Auto Layout]              │
├───────────────────────────────┼─────────────────────────────┤
│                               │  Pathfinding Results        │
│                               │  ┌─────────────────────────┐│
│                               │  │ [Результати тут]        ││
│                               │  │                         ││
│                               │  └─────────────────────────┘│
└───────────────────────────────┴─────────────────────────────┘
│ Ready. Create a graph to start.                             │
└─────────────────────────────────────────────────────────────┘
```

### Основні елементи

#### 1. **Graph Editor (Ліва панель)**
- Візуальне представлення графу
- Інтерактивні вузли (можна перетягувати мишею)
- Автоматична розкладка вузлів
- Підсвічування знайденого шляху

#### 2. **Control Panel (Права верхня панель)**
- **Graph Configuration** - налаштування графу
- **Add Edge** - додавання ребер
- **Find Path** - запуск пошуку шляху
- **File Operations** - завантаження/збереження
- **Layout** - автоматична розкладка

#### 3. **Results Widget (Права нижня панель)**
- Інформація про граф
- Результати алгоритмів
- Порівняння продуктивності
- Знайдений шлях

#### 4. **Menu Bar (Верхнє меню)**
- **File** - New, Load, Save, Exit
- **Edit** - Clear Graph
- **View** - Auto Layout (Ctrl+L)
- **Help** - About

#### 5. **Status Bar (Нижня панель)**
- Повідомлення про поточні операції
- Підтвердження дій

---

## Архітектура GUI

### Діаграма компонентів

```
┌─────────────────────────────────────────────────────────────┐
│                         MainWindow                          │
│  (QMainWindow - головне вікно програми)                     │
│                                                             │
│  ┌─────────────────┬────────────────────────────────────┐  │
│  │                 │      QSplitter (vertical)          │  │
│  │  GraphEditor    │  ┌──────────────────────────────┐  │  │
│  │   Widget        │  │    ControlPanel              │  │  │
│  │                 │  │  (Graph controls)            │  │  │
│  │  (Canvas для    │  └──────────────────────────────┘  │  │
│  │   візуалізації) │  ┌──────────────────────────────┐  │  │
│  │                 │  │    ResultsWidget             │  │  │
│  │                 │  │  (Results display)           │  │  │
│  │                 │  └──────────────────────────────┘  │  │
│  └─────────────────┴────────────────────────────────────┘  │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │             GraphWrapper (C ↔ Qt Bridge)            │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
              ┌──────────────────────────┐
              │   C Library (graph.c)    │
              │   BFS / DFS / Dijkstra   │
              └──────────────────────────┘
```

### Потік даних

```
User Action (GUI)
    │
    ▼
MainWindow (координує компоненти)
    │
    ▼
GraphWrapper (конвертує Qt ↔ C)
    │
    ▼
C Library (виконує алгоритми)
    │
    ▼
GraphWrapper (конвертує результати назад)
    │
    ▼
ResultsWidget / GraphEditorWidget (відображення)
```

---

## Компоненти

### 1. MainWindow (`MainWindow.h/cpp`)

**Призначення:** Головне вікно програми, координує всі компоненти.

**Відповідальність:**
- Створення та розміщення віджетів
- Обробка меню та команд
- Координація між компонентами
- Діалоги файлів (Load/Save)
- Відображення повідомлень про помилки

**Ключові методи:**
```cpp
void onCreateGraph(int numVertices, bool isWeighted, bool isDirected);
void onAddEdge(int src, int dest, double weight);
void onFindPath(int start, int end);
void onLoadGraph();
void onSaveGraph();
```

**Сигнали:** Немає (MainWindow - координатор).

---

### 2. GraphWrapper (`GraphWrapper.h/cpp`)

**Призначення:** Міст між C кодом та Qt GUI.

**Відповідальність:**
- Управління життєвим циклом C структур (`Graph*`)
- Конвертація даних між C та Qt
- Виклик C функцій з безпечним управлінням пам'яттю
- Завантаження/збереження файлів

**Ключові методи:**
```cpp
void createGraph(int numVertices, bool isWeighted, bool isDirected);
bool addEdge(int src, int dest, double weight);
QVector<PathResultData> findPaths(int start, int end);
QVector<EdgeData> getEdges() const;
bool loadFromFile(const QString& filename);
bool saveToFile(const QString& filename);
```

**Сигнали:**
```cpp
void graphChanged();           // Граф змінився
void errorOccurred(QString);   // Виникла помилка
```

**Формат файлу:**
```
<vertices> <edges> <is_weighted> <is_directed>
<src> <dest> [weight]
<src> <dest> [weight]
...
```

Приклад:
```
5 6 0 0
0 1
1 2
2 3
3 4
1 4
0 4
```

---

### 3. GraphEditorWidget (`GraphEditorWidget.h/cpp`)

**Призначення:** Інтерактивний візуальний редактор графів.

**Відповідальність:**
- Візуалізація графу (вузли та ребра)
- Обробка взаємодії миші (перетягування вузлів)
- Підсвічування знайденого шляху
- Автоматична розкладка (кругова, сіткова)

**Ключові методи:**
```cpp
void setGraphWrapper(GraphWrapper* wrapper);
void updateVisualization();
void highlightPath(const QVector<int>& path);
void autoLayout();            // Кругова розкладка
void circularLayout();        // Кругова розкладка (явно)
void gridLayout();            // Сіткова розкладка
```

**Візуальні константи:**
```cpp
static constexpr int NODE_RADIUS = 25;
static constexpr int NODE_BORDER_WIDTH = 2;
static constexpr int EDGE_WIDTH = 2;
static constexpr int ARROW_SIZE = 10;
static constexpr int HIGHLIGHT_WIDTH = 4;
```

**Кольорова схема:**
- Звичайний вузол: білий з чорною рамкою
- Наведений курсор: світло-сірий
- Вибраний вузол: світло-жовтий з синьою рамкою
- Підсвічений вузол (в шляху): блакитний
- Підсвічене ребро (в шляху): червоне

**Сигнали:**
```cpp
void nodeClicked(int nodeId);
void edgeCreationRequested(int src, int dest);
void selectionChanged(int nodeId);
```

---

### 4. ControlPanel (`ControlPanel.h/cpp`)

**Призначення:** Панель управління для конфігурації графу.

**Відповідальність:**
- Введення параметрів графу
- Додавання ребер
- Налаштування пошуку шляху
- Кнопки файлових операцій

**Групи елементів:**

**Graph Configuration:**
- Кількість вершин (2-100)
- Зважений граф (checkbox)
- Орієнтований граф (checkbox)
- Кнопка "Create Graph"
- Кнопка "Clear Graph"

**Add Edge:**
- From (початкова вершина)
- To (кінцева вершина)
- Weight (тільки для зважених графів)
- Кнопка "Add Edge"

**Find Path:**
- Start (початок пошуку)
- End (кінець пошуку)
- Кнопка "Find Path" (зелена, виділена)

**File Operations:**
- Кнопка "Load Graph"
- Кнопка "Save Graph"

**Layout:**
- Кнопка "Auto Layout"

**Сигнали:**
```cpp
void createGraphRequested(int numVertices, bool isWeighted, bool isDirected);
void clearGraphRequested();
void addEdgeRequested(int src, int dest, double weight);
void findPathRequested(int start, int end);
void loadGraphRequested();
void saveGraphRequested();
void autoLayoutRequested();
```

---

### 5. ResultsWidget (`ResultsWidget.h/cpp`)

**Призначення:** Відображення результатів пошуку шляху.

**Відповідальність:**
- Форматування та відображення результатів
- Порівняння продуктивності алгоритмів
- Текстовий вивід у стилі CLI

**Формат виводу:**
```
═══════════════════════════════════════════════════════════
                 PATHFINDING STATISTICS
═══════════════════════════════════════════════════════════

--- Graph Information ---
Type: Unweighted, Undirected
Vertices: 5
Edges: 6
Search: from vertex 0 to vertex 4

--- Algorithm Results ---

[Result 1]
  Algorithm: BFS (Breadth-First Search)
  Time: 0.021 ms
  Path: 0 -> 1 -> 4
  Total weight: 2.0

[Result 2]
  Algorithm: DFS (Depth-First Search)
  Time: 0.018 ms
  Path: 0 -> 1 -> 4
  Total weight: 2.0

--- Performance Comparison ---
Fastest algorithm: DFS (Depth-First Search) (0.018 ms)
```

**Ключові методи:**
```cpp
void displayResults(const GraphWrapper* graph,
                   const QVector<PathResultData>& results,
                   int start, int end);
void clearResults();
```

---

## Робочий Процес

### Типовий сценарій використання

#### 1. Створення графу

```
Користувач → ControlPanel
  │
  ├─ Встановлює Vertices: 5
  ├─ Встановлює Weighted: unchecked
  ├─ Встановлює Directed: unchecked
  └─ Натискає "Create Graph"
      │
      ▼
MainWindow::onCreateGraph(5, false, false)
      │
      ▼
GraphWrapper::createGraph(5, false, false)
      │ (викликає graph_create з C)
      │
      ▼
GraphEditorWidget::setNodeCount(5)
GraphEditorWidget::autoLayout()
      │
      ▼
[Граф візуалізується]
```

#### 2. Додавання ребер

```
Користувач → ControlPanel
  │
  ├─ Встановлює From: 0
  ├─ Встановлює To: 1
  └─ Натискає "Add Edge"
      │
      ▼
MainWindow::onAddEdge(0, 1, 1.0)
      │
      ▼
GraphWrapper::addEdge(0, 1, 1.0)
      │ (викликає graph_add_edge з C)
      │
      ▼
GraphEditorWidget::updateVisualization()
      │
      ▼
[Ребро малюється на екрані]
```

#### 3. Пошук шляху

```
Користувач → ControlPanel
  │
  ├─ Встановлює Start: 0
  ├─ Встановлює End: 4
  └─ Натискає "Find Path"
      │
      ▼
MainWindow::onFindPath(0, 4)
      │
      ▼
GraphWrapper::findPaths(0, 4)
      │
      ├─ Незважений граф?
      │   ├─ Викликає bfs_find_path()
      │   └─ Викликає dfs_find_path()
      │
      └─ Зважений граф?
          └─ Викликає dijkstra_find_path()
      │
      ▼
ResultsWidget::displayResults(...)
GraphEditorWidget::highlightPath(...)
      │
      ▼
[Шлях підсвічується, результати відображаються]
```

#### 4. Завантаження графу з файлу

```
Користувач → "File" → "Load Graph"
      │
      ▼
QFileDialog (вибір файлу)
      │
      ▼
MainWindow::onLoadGraph()
      │
      ▼
GraphWrapper::loadFromFile(filename)
      │
      ├─ Читає перший рядок (vertices, edges, weighted, directed)
      ├─ Викликає createGraph(...)
      └─ Читає ребра та викликає addEdge(...) для кожного
      │
      ▼
GraphEditorWidget::setNodeCount(...)
GraphEditorWidget::autoLayout()
      │
      ▼
[Граф завантажений та візуалізований]
```

---

## Розширення GUI

GUI спроектований модульно для легкого розширення. Ось основні напрямки розвитку:

### 1. Додавання нових алгоритмів

**Приклад: A* алгоритм**

**Крок 1:** Додайте C реалізацію
```c
// include/astar.h
PathResult* astar_find_path(const Graph* graph, int start, int end);

// src/astar.c
PathResult* astar_find_path(const Graph* graph, int start, int end) {
    // Ваша реалізація
}
```

**Крок 2:** Оновіть GraphWrapper
```cpp
// GraphWrapper.cpp :: findPaths()
if (m_graph->is_weighted) {
    // Dijkstra
    PathResult* dijkstra_result = dijkstra_find_path(m_graph, start, end);
    results.append(convertPathResult(dijkstra_result));

    // A* (NEW)
    PathResult* astar_result = astar_find_path(m_graph, start, end);
    results.append(convertPathResult(astar_result));
}
```

**Крок 3:** Результати автоматично відобразяться!

---

### 2. Анімація виконання алгоритму

**Концепція:** Поступове виділення вузлів під час виконання алгоритму.

**Потрібні зміни:**

**a) Додайте callback механізм у C код:**
```c
typedef void (*VisitCallback)(int vertex, void* user_data);

PathResult* bfs_find_path_animated(const Graph* graph,
                                   int start, int end,
                                   VisitCallback callback,
                                   void* user_data);
```

**b) Додайте стан анімації у GraphEditorWidget:**
```cpp
class GraphEditorWidget : public QWidget {
    Q_OBJECT
public:
    void animateAlgorithm(const QVector<int>& visitOrder);

private:
    QTimer* m_animationTimer;
    QVector<int> m_visitOrder;
    int m_currentStep;
};
```

**c) Оновіть візуалізацію покроково:**
```cpp
void GraphEditorWidget::animateAlgorithm(const QVector<int>& visitOrder) {
    m_visitOrder = visitOrder;
    m_currentStep = 0;

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, [this]() {
        if (m_currentStep < m_visitOrder.size()) {
            highlightNode(m_visitOrder[m_currentStep]);
            m_currentStep++;
        } else {
            m_animationTimer->stop();
        }
    });

    m_animationTimer->start(500); // 500ms між кроками
}
```

---

### 3. Інтерактивне редагування графу

**Додайте можливість створення вузлів та ребер мишею.**

**Потрібні зміни:**

**a) Режим редагування у GraphEditorWidget:**
```cpp
enum EditMode {
    MODE_VIEW,        // Тільки перегляд
    MODE_ADD_NODE,    // Додавання вузлів
    MODE_ADD_EDGE,    // Додавання ребер
    MODE_DELETE       // Видалення
};
```

**b) Обробка кліків миші:**
```cpp
void GraphEditorWidget::mousePressEvent(QMouseEvent* event) {
    if (m_editMode == MODE_ADD_NODE) {
        emit nodeCreationRequested(event->pos());
    } else if (m_editMode == MODE_ADD_EDGE) {
        int nodeId = findNodeAt(event->pos());
        if (m_edgeCreationStart < 0) {
            m_edgeCreationStart = nodeId;
        } else {
            emit edgeCreationRequested(m_edgeCreationStart, nodeId);
            m_edgeCreationStart = -1;
        }
    }
}
```

**c) Додайте кнопки режимів у ControlPanel:**
```cpp
QPushButton* m_addNodeModeButton;
QPushButton* m_addEdgeModeButton;
QPushButton* m_deleteModeButton;
```

---

### 4. Експорт графу як зображення

**Збереження візуалізації у PNG/SVG.**

**Потрібні зміни:**

**a) Додайте метод експорту у GraphEditorWidget:**
```cpp
void GraphEditorWidget::exportToImage(const QString& filename) {
    QPixmap pixmap(size());
    render(&pixmap);
    pixmap.save(filename);
}
```

**b) Додайте пункт меню:**
```cpp
// MainWindow.cpp
QAction* m_exportImageAction = new QAction("Export as Image...", this);
m_fileMenu->addAction(m_exportImageAction);
connect(m_exportImageAction, &QAction::triggered, [this]() {
    QString filename = QFileDialog::getSaveFileName(
        this, "Export Image", "", "PNG Files (*.png);;All Files (*)"
    );
    if (!filename.isEmpty()) {
        m_graphEditor->exportToImage(filename);
    }
});
```

---

### 5. Графіки продуктивності

**Відображення графіків порівняння швидкості алгоритмів.**

**Потрібні зміни:**

**a) Використайте QCustomPlot або QtCharts:**
```cpp
// ResultsWidget.h
#include <QtCharts>

class ResultsWidget : public QWidget {
    // ...
private:
    QChartView* m_performanceChart;
};
```

**b) Створіть bar chart:**
```cpp
void ResultsWidget::displayPerformanceChart(const QVector<PathResultData>& results) {
    QBarSet* timeSet = new QBarSet("Execution Time (ms)");
    QStringList algorithms;

    for (const auto& result : results) {
        *timeSet << result.timeMs;
        algorithms << result.algorithm;
    }

    QBarSeries* series = new QBarSeries();
    series->append(timeSet);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Algorithm Performance Comparison");

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(algorithms);
    chart->addAxis(axisX, Qt::AlignBottom);

    m_performanceChart->setChart(chart);
}
```

---

### 6. Темна тема

**Додайте підтримку темної теми.**

**Потрібні зміни:**

**a) QSS (Qt Style Sheets):**
```cpp
// MainWindow.cpp
void MainWindow::applyDarkTheme() {
    QString darkStyle = R"(
        QMainWindow {
            background-color: #2b2b2b;
        }
        QTextEdit {
            background-color: #1e1e1e;
            color: #d4d4d4;
        }
        QPushButton {
            background-color: #3c3c3c;
            color: white;
            border: 1px solid #555555;
            padding: 5px;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
    )";

    qApp->setStyleSheet(darkStyle);
}
```

**b) Додайте перемикач у меню View:**
```cpp
QAction* m_darkThemeAction = new QAction("Dark Theme", this);
m_darkThemeAction->setCheckable(true);
m_viewMenu->addAction(m_darkThemeAction);
connect(m_darkThemeAction, &QAction::toggled, [this](bool checked) {
    if (checked) {
        applyDarkTheme();
    } else {
        qApp->setStyleSheet("");
    }
});
```

---

## Структура файлів

```
gui/
├── graphpath.pro              # Qt project file
├── include/
│   ├── MainWindow.h           # Головне вікно
│   ├── GraphWrapper.h         # C ↔ Qt bridge
│   ├── GraphEditorWidget.h    # Візуальний редактор
│   ├── ControlPanel.h         # Панель управління
│   └── ResultsWidget.h        # Відображення результатів
└── src/
    ├── main.cpp               # Entry point
    ├── MainWindow.cpp
    ├── GraphWrapper.cpp
    ├── GraphEditorWidget.cpp
    ├── ControlPanel.cpp
    └── ResultsWidget.cpp
```

---

## Відомі обмеження

### Поточні обмеження:

1. **Видалення ребер** - не реалізовано (потребує додавання у C бібліотеку)
2. **Редагування ваги ребра** - не реалізовано
3. **Множинний вибір вузлів** - не реалізовано
4. **Скасування/повтор** (Undo/Redo) - не реалізовано
5. **Масштабування графу** (zoom) - не реалізовано
6. **Анімація алгоритмів** - не реалізовано

### Планові покращення:

- [ ] Анімація виконання алгоритмів
- [ ] Інтерактивне створення вузлів мишею
- [ ] Режими редагування (додавання/видалення)
- [ ] Експорт графу як зображення (PNG/SVG)
- [ ] Графіки продуктивності (bar charts)
- [ ] Темна тема
- [ ] Множинні вікна графів
- [ ] Історія операцій (Undo/Redo)
- [ ] Масштабування (zoom in/out)
- [ ] A* алгоритм з евристиками

---

## Допомога та підтримка

### Поширені проблеми

**Q: GUI не компілується - "qmake: command not found"**

A: Встановіть Qt development пакети:
```bash
sudo apt-get install qt5-qmake qtbase5-dev
```

**Q: Помилка "Project ERROR: Unknown module(s) in QT: widgets"**

A: Встановіть qtbase5-dev:
```bash
sudo apt-get install qtbase5-dev
```

**Q: Граф не відображається після створення**

A: Переконайтеся що ви додали ребра. Граф без ребер відобразиться лише як вузли.

**Q: Шлях не знаходиться**

A: Перевірте, чи граф зв'язний. Для орієнтованих графів перевірте напрямок ребер.

---

## Контакти

- **Проект:** GraphPath
- **Документація:** README.md, TECHNICAL.md, QUICKSTART.md
- **GUI Документація:** GUI.md (цей файл)

---

**Версія документації:** 1.0
**Дата оновлення:** 2025-11-18
