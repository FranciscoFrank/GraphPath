#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

/**
 * @brief Control panel for graph creation and pathfinding
 *
 * This widget provides controls for:
 * - Creating new graphs with specified properties
 * - Adding edges to the graph
 * - Configuring pathfinding parameters
 * - Triggering pathfinding operations
 * - Loading and saving graphs
 */
class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    // Get current graph configuration
    int getNumVertices() const;
    bool isWeighted() const;
    bool isDirected() const;

    // Get pathfinding parameters
    int getStartVertex() const;
    int getEndVertex() const;

    // Get edge input
    int getEdgeSrc() const;
    int getEdgeDest() const;
    double getEdgeWeight() const;

    // Enable/disable controls based on state
    void setGraphCreated(bool created);
    void setMaxVertexValue(int maxValue);

signals:
    // Graph creation
    void createGraphRequested(int numVertices, bool isWeighted, bool isDirected);
    void clearGraphRequested();

    // Edge operations
    void addEdgeRequested(int src, int dest, double weight);

    // Pathfinding
    void findPathRequested(int start, int end);

    // File operations
    void loadGraphRequested();
    void saveGraphRequested();

    // Layout
    void autoLayoutRequested();

private slots:
    void onCreateGraphClicked();
    void onClearGraphClicked();
    void onAddEdgeClicked();
    void onFindPathClicked();
    void onLoadGraphClicked();
    void onSaveGraphClicked();
    void onAutoLayoutClicked();
    void onWeightedChanged(int state);

private:
    // Graph configuration group
    QGroupBox* m_graphConfigGroup;
    QSpinBox* m_verticesSpinBox;
    QCheckBox* m_weightedCheckBox;
    QCheckBox* m_directedCheckBox;
    QPushButton* m_createGraphButton;
    QPushButton* m_clearGraphButton;

    // Edge input group
    QGroupBox* m_edgeInputGroup;
    QSpinBox* m_edgeSrcSpinBox;
    QSpinBox* m_edgeDestSpinBox;
    QSpinBox* m_edgeWeightSpinBox;
    QLabel* m_edgeWeightLabel;
    QPushButton* m_addEdgeButton;

    // Pathfinding group
    QGroupBox* m_pathfindingGroup;
    QSpinBox* m_startVertexSpinBox;
    QSpinBox* m_endVertexSpinBox;
    QPushButton* m_findPathButton;

    // File operations group
    QGroupBox* m_fileOperationsGroup;
    QPushButton* m_loadGraphButton;
    QPushButton* m_saveGraphButton;

    // Layout group
    QGroupBox* m_layoutGroup;
    QPushButton* m_autoLayoutButton;

    // Helper methods
    void setupUI();
    void createGraphConfigGroup();
    void createEdgeInputGroup();
    void createPathfindingGroup();
    void createFileOperationsGroup();
    void createLayoutGroup();
};

#endif // CONTROLPANEL_H
