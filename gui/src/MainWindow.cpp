#include "MainWindow.h"
#include "ThemeManager.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("GraphPath - Graph Pathfinding Visualizer");
    resize(1200, 700);

    m_graphWrapper = new GraphWrapper(this);

    setupUI();
    createMenuBar();
    createStatusBar();
    connectSignals();

    updateStatusBar("Ready. Create a graph to start.");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create widgets
    m_graphEditor = new GraphEditorWidget(this);
    m_graphEditor->setGraphWrapper(m_graphWrapper);

    m_controlPanel = new ControlPanel(this);
    m_resultsWidget = new ResultsWidget(this);

    // Create right splitter (vertical) for control panel and results
    m_rightSplitter = new QSplitter(Qt::Vertical, this);
    m_rightSplitter->addWidget(m_controlPanel);
    m_rightSplitter->addWidget(m_resultsWidget);
    m_rightSplitter->setStretchFactor(0, 1);
    m_rightSplitter->setStretchFactor(1, 1);

    // Create main splitter (horizontal) for editor and right panel
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(m_graphEditor);
    m_mainSplitter->addWidget(m_rightSplitter);

    // Set proportions: 50% editor, 50% controls
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_mainSplitter);
}

void MainWindow::createMenuBar()
{
    // File menu
    m_fileMenu = menuBar()->addMenu("&File");

    m_newAction = new QAction("&New Graph", this);
    m_newAction->setShortcut(QKeySequence::New);
    m_fileMenu->addAction(m_newAction);

    m_loadAction = new QAction("&Load Graph...", this);
    m_loadAction->setShortcut(QKeySequence::Open);
    m_fileMenu->addAction(m_loadAction);

    m_saveAction = new QAction("&Save Graph...", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_fileMenu->addAction(m_saveAction);

    m_fileMenu->addSeparator();

    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_fileMenu->addAction(m_exitAction);

    // Edit menu
    m_editMenu = menuBar()->addMenu("&Edit");

    m_clearAction = new QAction("&Clear Graph", this);
    m_editMenu->addAction(m_clearAction);

    // View menu
    m_viewMenu = menuBar()->addMenu("&View");

    m_autoLayoutAction = new QAction("&Auto Layout", this);
    m_autoLayoutAction->setShortcut(QKeySequence("Ctrl+L"));
    m_viewMenu->addAction(m_autoLayoutAction);

    m_viewMenu->addSeparator();

    m_toggleThemeAction = new QAction("Toggle &Theme", this);
    m_toggleThemeAction->setShortcut(QKeySequence("Ctrl+T"));
    m_viewMenu->addAction(m_toggleThemeAction);

    // Help menu
    m_helpMenu = menuBar()->addMenu("&Help");

    m_aboutAction = new QAction("&About", this);
    m_helpMenu->addAction(m_aboutAction);

    // Connect menu actions
    connect(m_newAction, &QAction::triggered, this, &MainWindow::onNewGraph);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::onLoadGraph);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveGraph);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::onClearGraph);
    connect(m_autoLayoutAction, &QAction::triggered, this, &MainWindow::onAutoLayout);
    connect(m_toggleThemeAction, &QAction::triggered, this, &MainWindow::onToggleTheme);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    // Control panel signals
    connect(m_controlPanel, &ControlPanel::createGraphRequested,
            this, &MainWindow::onCreateGraph);
    connect(m_controlPanel, &ControlPanel::clearGraphRequested,
            this, &MainWindow::onClearGraph);
    connect(m_controlPanel, &ControlPanel::addEdgeRequested,
            this, &MainWindow::onAddEdge);
    connect(m_controlPanel, &ControlPanel::findPathRequested,
            this, &MainWindow::onFindPath);
    connect(m_controlPanel, &ControlPanel::loadGraphRequested,
            this, &MainWindow::onLoadGraph);
    connect(m_controlPanel, &ControlPanel::saveGraphRequested,
            this, &MainWindow::onSaveGraph);
    connect(m_controlPanel, &ControlPanel::autoLayoutRequested,
            this, &MainWindow::onAutoLayout);

    // Graph editor signals
    connect(m_graphEditor, &GraphEditorWidget::edgeCreationRequested,
            this, &MainWindow::onAddEdgeInteractive);
    connect(m_graphEditor, &GraphEditorWidget::vertexAddRequested,
            this, &MainWindow::onAddVertex);
    connect(m_graphEditor, &GraphEditorWidget::vertexRemoveRequested,
            this, &MainWindow::onRemoveVertex);
    connect(m_graphEditor, &GraphEditorWidget::edgeRemoveRequested,
            this, &MainWindow::onRemoveEdge);
    connect(m_graphEditor, &GraphEditorWidget::edgeWeightChangeRequested,
            this, &MainWindow::onChangeEdgeWeight);

    // Graph wrapper signals
    connect(m_graphWrapper, &GraphWrapper::graphChanged,
            m_resultsWidget, &ResultsWidget::onGraphChanged);
    connect(m_graphWrapper, &GraphWrapper::errorOccurred,
            this, &MainWindow::onError);
}

void MainWindow::onCreateGraph(int numVertices, bool isWeighted, bool isDirected)
{
    m_graphWrapper->createGraph(numVertices, isWeighted, isDirected);

    if (m_graphWrapper->hasGraph()) {
        m_controlPanel->setGraphCreated(true);
        m_controlPanel->setMaxVertexValue(numVertices);
        m_graphEditor->setNodeCount(numVertices);
        m_graphEditor->autoLayout();

        QString type = QString("%1, %2")
            .arg(isWeighted ? "Weighted" : "Unweighted")
            .arg(isDirected ? "Directed" : "Undirected");

        updateStatusBar(QString("Created %1 graph with %2 vertices")
                       .arg(type).arg(numVertices));
    }
}

void MainWindow::onClearGraph()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear Graph",
                                  "Are you sure you want to clear the graph?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_graphWrapper->clearGraph();
        m_graphEditor->clearVisualization();
        m_resultsWidget->clearResults();
        m_controlPanel->setGraphCreated(false);
        updateStatusBar("Graph cleared");
    }
}

void MainWindow::onAddVertex(const QPointF& position)
{
    if (!m_graphWrapper->hasGraph()) {
        QMessageBox::warning(this, "No Graph",
                           "Please create a graph first.");
        return;
    }

    if (m_graphWrapper->addVertex()) {
        m_graphEditor->addVertex(position);
        updateStatusBar(QString("Added vertex %1").arg(m_graphWrapper->getNumVertices() - 1));
    }
}

void MainWindow::onRemoveVertex(int vertex)
{
    if (!m_graphWrapper->hasGraph()) {
        return;
    }

    if (m_graphWrapper->removeVertex(vertex)) {
        m_graphEditor->onGraphChanged();
        updateStatusBar(QString("Removed vertex %1").arg(vertex));
    }
}

void MainWindow::onAddEdge(int src, int dest, double weight)
{
    if (m_graphWrapper->addEdge(src, dest, weight)) {
        m_graphEditor->updateVisualization();

        QString msg = QString("Added edge: %1 → %2").arg(src).arg(dest);
        if (m_graphWrapper->isWeighted()) {
            msg += QString(" (weight: %1)").arg(weight);
        }
        updateStatusBar(msg);
    }
}

void MainWindow::onAddEdgeInteractive(int src, int dest)
{
    double weight = 1.0;

    // If graph is weighted, prompt user for weight
    if (m_graphWrapper->isWeighted()) {
        bool ok;
        weight = QInputDialog::getDouble(this, "Edge Weight",
                                        QString("Enter weight for edge %1 → %2:")
                                            .arg(src).arg(dest),
                                        1.0, 0.0, 1000.0, 2, &ok);
        if (!ok) {
            // User cancelled, don't create edge
            updateStatusBar("Edge creation cancelled");
            return;
        }
    }

    onAddEdge(src, dest, weight);
}

void MainWindow::onRemoveEdge(int src, int dest)
{
    if (!m_graphWrapper->hasGraph()) {
        return;
    }

    if (m_graphWrapper->removeEdge(src, dest)) {
        m_graphEditor->updateVisualization();
        updateStatusBar(QString("Removed edge: %1 → %2").arg(src).arg(dest));
    }
}

void MainWindow::onChangeEdgeWeight(int src, int dest, double currentWeight)
{
    if (!m_graphWrapper->hasGraph() || !m_graphWrapper->isWeighted()) {
        return;
    }

    bool ok;
    double newWeight = QInputDialog::getDouble(this, "Change Edge Weight",
                                              QString("Enter new weight for edge %1 → %2:")
                                                  .arg(src).arg(dest),
                                              currentWeight, 0.0, 1000.0, 2, &ok);
    if (!ok) {
        updateStatusBar("Weight change cancelled");
        return;
    }

    // Remove old edge and add new one with updated weight
    if (m_graphWrapper->removeEdge(src, dest)) {
        if (m_graphWrapper->addEdge(src, dest, newWeight)) {
            m_graphEditor->updateVisualization();
            updateStatusBar(QString("Changed weight of edge %1 → %2 to %3")
                           .arg(src).arg(dest).arg(newWeight));
        }
    }
}

void MainWindow::onFindPath(int start, int end)
{
    if (!m_graphWrapper->hasGraph()) {
        QMessageBox::warning(this, "No Graph",
                           "Please create a graph first.");
        return;
    }

    QVector<PathResultData> results = m_graphWrapper->findPaths(start, end);

    if (results.isEmpty()) {
        QMessageBox::warning(this, "Pathfinding Failed",
                           "No results returned. Check your graph.");
        updateStatusBar("Pathfinding failed");
        return;
    }

    // Display results
    m_resultsWidget->displayResults(m_graphWrapper, results, start, end);

    // Highlight path from first algorithm (usually the best one)
    if (results[0].found) {
        m_graphEditor->highlightPath(results[0].path);
        updateStatusBar(QString("Path found from %1 to %2 using %3")
                       .arg(start).arg(end).arg(results[0].algorithm));
    } else {
        m_graphEditor->clearHighlight();
        updateStatusBar(QString("No path found from %1 to %2")
                       .arg(start).arg(end));
    }
}

void MainWindow::onLoadGraph()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Load Graph",
        "../examples",
        "Graph Files (*.txt);;All Files (*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    if (m_graphWrapper->loadFromFile(filename)) {
        m_controlPanel->setGraphCreated(true);
        m_controlPanel->setMaxVertexValue(m_graphWrapper->getNumVertices());
        m_graphEditor->setNodeCount(m_graphWrapper->getNumVertices());
        m_graphEditor->autoLayout();

        updateStatusBar(QString("Loaded graph from %1").arg(filename));
    } else {
        QMessageBox::warning(this, "Load Failed",
                           "Failed to load graph from file.");
    }
}

void MainWindow::onSaveGraph()
{
    if (!m_graphWrapper->hasGraph()) {
        QMessageBox::warning(this, "No Graph",
                           "Please create a graph first.");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Graph",
        "",
        "Graph Files (*.txt);;All Files (*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    if (m_graphWrapper->saveToFile(filename)) {
        updateStatusBar(QString("Saved graph to %1").arg(filename));
    } else {
        QMessageBox::warning(this, "Save Failed",
                           "Failed to save graph to file.");
    }
}

void MainWindow::onNewGraph()
{
    if (m_graphWrapper->hasGraph()) {
        onClearGraph();
    }
}

void MainWindow::onExit()
{
    QApplication::quit();
}

void MainWindow::onAutoLayout()
{
    if (m_graphWrapper->hasGraph()) {
        m_graphEditor->autoLayout();
        updateStatusBar("Applied auto layout");
    }
}

void MainWindow::onToggleTheme()
{
    ThemeManager::instance().toggleTheme();

    ThemeType currentTheme = ThemeManager::instance().currentTheme();
    QString themeName = (currentTheme == ThemeType::Dark) ? "Dark" : "Light";
    updateStatusBar(QString("Switched to %1 theme").arg(themeName));
}

void MainWindow::onError(const QString& error)
{
    QMessageBox::critical(this, "Error", error);
    updateStatusBar(QString("Error: %1").arg(error));
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About GraphPath",
        "<h2>GraphPath GUI</h2>"
        "<p>Version 1.0</p>"
        "<p>A graph pathfinding visualization tool that implements "
        "multiple shortest-path algorithms:</p>"
        "<ul>"
        "<li>BFS (Breadth-First Search)</li>"
        "<li>DFS (Depth-First Search)</li>"
        "<li>Dijkstra's Algorithm</li>"
        "</ul>"
        "<p>Built with Qt and C</p>"
        "<p>For more information, see the README.md and GUI.md files.</p>"
    );
}

void MainWindow::updateStatusBar(const QString& message)
{
    statusBar()->showMessage(message, 5000); // Show for 5 seconds
}
