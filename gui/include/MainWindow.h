#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include "GraphWrapper.h"
#include "GraphEditorWidget.h"
#include "ControlPanel.h"
#include "ResultsWidget.h"

/**
 * @brief Main application window
 *
 * This is the top-level window that brings together all GUI components:
 * - GraphEditorWidget (left): Visual graph editor
 * - ControlPanel (right top): Graph configuration and controls
 * - ResultsWidget (right bottom): Pathfinding results display
 *
 * Handles:
 * - Window layout and organization
 * - Menu bar (File, Edit, View, Help)
 * - Status bar for user feedback
 * - Coordination between widgets
 * - File operations (load/save)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Graph operations
    void onCreateGraph(int numVertices, bool isWeighted, bool isDirected);
    void onClearGraph();
    void onAddVertex(const QPointF& position);
    void onRemoveVertex(int vertex);
    void onAddEdge(int src, int dest, double weight);
    void onAddEdgeInteractive(int src, int dest);
    void onRemoveEdge(int src, int dest);
    void onChangeEdgeWeight(int src, int dest, double currentWeight);

    // Pathfinding
    void onFindPath(int start, int end);

    // File operations
    void onLoadGraph();
    void onSaveGraph();
    void onNewGraph();
    void onExit();

    // Layout operations
    void onAutoLayout();

    // Theme operations
    void onToggleTheme();

    // Error handling
    void onError(const QString& error);

    // Help
    void onAbout();

private:
    // Core components
    GraphWrapper* m_graphWrapper;
    GraphEditorWidget* m_graphEditor;
    ControlPanel* m_controlPanel;
    ResultsWidget* m_resultsWidget;

    // Layout
    QSplitter* m_mainSplitter;
    QSplitter* m_rightSplitter;

    // Menu bar
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;

    QAction* m_newAction;
    QAction* m_loadAction;
    QAction* m_saveAction;
    QAction* m_exitAction;
    QAction* m_clearAction;
    QAction* m_autoLayoutAction;
    QAction* m_toggleThemeAction;
    QAction* m_aboutAction;

    // Helper methods
    void setupUI();
    void createMenuBar();
    void createStatusBar();
    void connectSignals();
    void updateStatusBar(const QString& message);

    // Smart algorithm selection
    int selectBestAlgorithm(const QVector<PathResultData>& results, QString& reason);
    bool hasNegativeWeights() const;
};

#endif // MAINWINDOW_H
