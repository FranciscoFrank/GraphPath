#ifndef GRAPHEDITORWIDGET_H
#define GRAPHEDITORWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QMap>
#include <QPair>
#include "GraphWrapper.h"

/**
 * @brief Node position data for visualization
 */
struct NodePosition {
    int id;
    QPointF position;
    bool isSelected;

    NodePosition() : id(-1), isSelected(false) {}
    NodePosition(int nodeId, const QPointF& pos)
        : id(nodeId), position(pos), isSelected(false) {}
};

/**
 * @brief Interactive graph editor widget with visualization
 *
 * This widget provides:
 * - Visual representation of the graph
 * - Interactive node creation and positioning
 * - Edge creation by clicking nodes
 * - Path highlighting after pathfinding
 * - Automatic layout support
 */
class GraphEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphEditorWidget(QWidget *parent = nullptr);
    ~GraphEditorWidget();

    // Set the graph wrapper to visualize
    void setGraphWrapper(GraphWrapper* wrapper);

    // Visualization control
    void updateVisualization();
    void clearVisualization();
    void highlightPath(const QVector<int>& path);
    void clearHighlight();

    // Layout algorithms
    void autoLayout();
    void circularLayout();
    void gridLayout();

    // Node operations
    void setNodeCount(int count);
    int getNodeCount() const { return m_nodes.size(); }
    void addVertex(const QPointF& position);

    // Coordinate synchronization (for A* algorithm)
    void syncCoordinatesToGraph();

public slots:
    void onGraphChanged();

signals:
    void nodeClicked(int nodeId);
    void edgeCreationRequested(int src, int dest);
    void selectionChanged(int nodeId);
    void vertexAddRequested(const QPointF& position);
    void vertexRemoveRequested(int vertex);
    void edgeRemoveRequested(int src, int dest);
    void edgeWeightChangeRequested(int src, int dest, double currentWeight);

protected:
    // Qt event handlers
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    // Graph data
    GraphWrapper* m_graphWrapper;
    QVector<NodePosition> m_nodes;
    QVector<int> m_highlightedPath;

    // Interaction state
    int m_selectedNode;
    int m_hoveredNode;
    bool m_isDragging;
    QPointF m_dragOffset;

    // Edge creation mode
    bool m_edgeCreationMode;
    int m_edgeCreationStart;

    // Visual constants
    static constexpr int NODE_RADIUS = 25;
    static constexpr int NODE_BORDER_WIDTH = 2;
    static constexpr int EDGE_WIDTH = 2;
    static constexpr int ARROW_SIZE = 10;
    static constexpr int HIGHLIGHT_WIDTH = 4;

    // Helper methods
    int findNodeAt(const QPointF& pos) const;
    QPair<int, int> findEdgeAt(const QPointF& pos) const;
    void handleRightClick(const QPointF& pos);
    void showNodeContextMenu(const QPointF& pos, int nodeId);
    void showEdgeContextMenu(const QPointF& pos, int src, int dest);
    void drawNode(QPainter& painter, const NodePosition& node, bool isHighlighted);
    void drawEdge(QPainter& painter, const EdgeData& edge, bool isHighlighted);
    void drawArrow(QPainter& painter, const QPointF& start, const QPointF& end);
    bool isEdgeHighlighted(int src, int dest) const;
    QPointF getNodeCenter(int nodeId) const;
};

#endif // GRAPHEDITORWIDGET_H
