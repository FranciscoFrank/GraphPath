#include "GraphEditorWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>

GraphEditorWidget::GraphEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_graphWrapper(nullptr),
      m_selectedNode(-1),
      m_hoveredNode(-1),
      m_isDragging(false),
      m_edgeCreationMode(false),
      m_edgeCreationStart(-1)
{
    setMinimumSize(400, 400);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

GraphEditorWidget::~GraphEditorWidget()
{
}

void GraphEditorWidget::setGraphWrapper(GraphWrapper* wrapper)
{
    m_graphWrapper = wrapper;
    if (m_graphWrapper) {
        connect(m_graphWrapper, &GraphWrapper::graphChanged,
                this, &GraphEditorWidget::onGraphChanged);
    }
}

void GraphEditorWidget::updateVisualization()
{
    update();
}

void GraphEditorWidget::clearVisualization()
{
    m_nodes.clear();
    m_highlightedPath.clear();
    m_selectedNode = -1;
    m_hoveredNode = -1;
    update();
}

void GraphEditorWidget::highlightPath(const QVector<int>& path)
{
    m_highlightedPath = path;
    update();
}

void GraphEditorWidget::clearHighlight()
{
    m_highlightedPath.clear();
    update();
}

void GraphEditorWidget::setNodeCount(int count)
{
    m_nodes.clear();
    m_nodes.reserve(count);

    for (int i = 0; i < count; i++) {
        m_nodes.append(NodePosition(i, QPointF(0, 0)));
    }

    // Apply default layout
    circularLayout();
}

void GraphEditorWidget::autoLayout()
{
    circularLayout(); // Use circular layout as default auto layout
}

void GraphEditorWidget::circularLayout()
{
    if (m_nodes.isEmpty()) {
        return;
    }

    QPointF center(width() / 2.0, height() / 2.0);
    double radius = qMin(width(), height()) * 0.35;

    int count = m_nodes.size();
    for (int i = 0; i < count; i++) {
        double angle = 2.0 * M_PI * i / count - M_PI / 2.0; // Start from top
        m_nodes[i].position = center + QPointF(
            radius * qCos(angle),
            radius * qSin(angle)
        );
    }

    update();
}

void GraphEditorWidget::gridLayout()
{
    if (m_nodes.isEmpty()) {
        return;
    }

    int count = m_nodes.size();
    int cols = qCeil(qSqrt(count));
    int rows = qCeil(count / (double)cols);

    double marginX = width() * 0.1;
    double marginY = height() * 0.1;
    double spacingX = (width() - 2 * marginX) / qMax(cols - 1, 1);
    double spacingY = (height() - 2 * marginY) / qMax(rows - 1, 1);

    for (int i = 0; i < count; i++) {
        int row = i / cols;
        int col = i % cols;
        m_nodes[i].position = QPointF(
            marginX + col * spacingX,
            marginY + row * spacingY
        );
    }

    update();
}

void GraphEditorWidget::onGraphChanged()
{
    if (!m_graphWrapper) {
        return;
    }

    int newCount = m_graphWrapper->getNumVertices();
    int currentCount = m_nodes.size();

    if (newCount != currentCount) {
        setNodeCount(newCount);
    }

    clearHighlight();
    update();
}

void GraphEditorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), Qt::white);

    if (!m_graphWrapper || !m_graphWrapper->hasGraph()) {
        // Draw placeholder text
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter,
                        "Create a graph to start\n(Use controls on the right)");
        return;
    }

    // Draw edges first (so they appear behind nodes)
    QVector<EdgeData> edges = m_graphWrapper->getEdges();
    for (const EdgeData& edge : edges) {
        bool highlighted = isEdgeHighlighted(edge.src, edge.dest);
        drawEdge(painter, edge, highlighted);
    }

    // Draw nodes
    for (const NodePosition& node : m_nodes) {
        bool highlighted = m_highlightedPath.contains(node.id);
        drawNode(painter, node, highlighted);
    }

    // Draw edge creation indicator
    if (m_edgeCreationMode && m_edgeCreationStart >= 0) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        QPointF start = getNodeCenter(m_edgeCreationStart);
        QPointF end = mapFromGlobal(QCursor::pos());
        painter.drawLine(start, end);
    }
}

void GraphEditorWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QPointF pos = event->position();
    int nodeId = findNodeAt(pos);

    if (nodeId >= 0) {
        m_selectedNode = nodeId;
        m_isDragging = true;
        m_dragOffset = pos - m_nodes[nodeId].position;
        emit nodeClicked(nodeId);
        emit selectionChanged(nodeId);
        update();
    } else {
        m_selectedNode = -1;
        emit selectionChanged(-1);
        update();
    }
}

void GraphEditorWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPointF pos = event->position();

    if (m_isDragging && m_selectedNode >= 0) {
        // Drag node
        m_nodes[m_selectedNode].position = pos - m_dragOffset;
        update();
    } else {
        // Update hover state
        int nodeId = findNodeAt(pos);
        if (nodeId != m_hoveredNode) {
            m_hoveredNode = nodeId;
            setCursor(nodeId >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor);
            update();
        }
    }
}

void GraphEditorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}

void GraphEditorWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    // Optionally re-layout nodes on resize
    // circularLayout();
}

int GraphEditorWidget::findNodeAt(const QPointF& pos) const
{
    for (int i = 0; i < m_nodes.size(); i++) {
        QPointF nodePos = m_nodes[i].position;
        double dist = qSqrt(qPow(pos.x() - nodePos.x(), 2) +
                           qPow(pos.y() - nodePos.y(), 2));
        if (dist <= NODE_RADIUS) {
            return i;
        }
    }
    return -1;
}

void GraphEditorWidget::drawNode(QPainter& painter, const NodePosition& node, bool isHighlighted)
{
    QPointF center = node.position;

    // Determine colors
    QColor fillColor = Qt::white;
    QColor borderColor = Qt::black;
    int borderWidth = NODE_BORDER_WIDTH;

    if (isHighlighted) {
        fillColor = QColor(100, 200, 255);
        borderColor = QColor(0, 100, 200);
        borderWidth = HIGHLIGHT_WIDTH;
    } else if (node.id == m_selectedNode) {
        fillColor = QColor(255, 255, 200);
        borderColor = Qt::darkBlue;
    } else if (node.id == m_hoveredNode) {
        fillColor = QColor(240, 240, 240);
    }

    // Draw node circle
    painter.setPen(QPen(borderColor, borderWidth));
    painter.setBrush(fillColor);
    painter.drawEllipse(center, NODE_RADIUS, NODE_RADIUS);

    // Draw node label
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);

    QString label = QString::number(node.id);
    QRectF textRect(center.x() - NODE_RADIUS, center.y() - NODE_RADIUS,
                    NODE_RADIUS * 2, NODE_RADIUS * 2);
    painter.drawText(textRect, Qt::AlignCenter, label);
}

void GraphEditorWidget::drawEdge(QPainter& painter, const EdgeData& edge, bool isHighlighted)
{
    if (edge.src < 0 || edge.src >= m_nodes.size() ||
        edge.dest < 0 || edge.dest >= m_nodes.size()) {
        return;
    }

    QPointF start = m_nodes[edge.src].position;
    QPointF end = m_nodes[edge.dest].position;

    // Calculate edge endpoints (offset by node radius)
    double angle = qAtan2(end.y() - start.y(), end.x() - start.x());
    QPointF startOffset = start + QPointF(NODE_RADIUS * qCos(angle),
                                          NODE_RADIUS * qSin(angle));
    QPointF endOffset = end - QPointF(NODE_RADIUS * qCos(angle),
                                      NODE_RADIUS * qSin(angle));

    // Determine edge style
    QColor edgeColor = isHighlighted ? QColor(255, 100, 100) : Qt::black;
    int edgeWidth = isHighlighted ? HIGHLIGHT_WIDTH : EDGE_WIDTH;

    painter.setPen(QPen(edgeColor, edgeWidth));
    painter.drawLine(startOffset, endOffset);

    // Draw arrow for directed graphs
    if (m_graphWrapper && m_graphWrapper->isDirected()) {
        drawArrow(painter, startOffset, endOffset);
    }

    // Draw weight label for weighted graphs
    if (m_graphWrapper && m_graphWrapper->isWeighted()) {
        QPointF midPoint = (start + end) / 2.0;

        painter.setPen(Qt::black);
        painter.setBrush(Qt::white);

        QString weightText = QString::number(edge.weight, 'g', 3);
        QFont font = painter.font();
        font.setPointSize(9);
        painter.setFont(font);

        QFontMetrics fm(font);
        QRectF textRect = fm.boundingRect(weightText);
        textRect.moveCenter(midPoint);
        textRect.adjust(-3, -3, 3, 3);

        painter.drawRect(textRect);
        painter.drawText(textRect, Qt::AlignCenter, weightText);
    }
}

void GraphEditorWidget::drawArrow(QPainter& painter, const QPointF& start, const QPointF& end)
{
    double angle = qAtan2(end.y() - start.y(), end.x() - start.x());

    QPointF arrowP1 = end - QPointF(
        ARROW_SIZE * qCos(angle - M_PI / 6),
        ARROW_SIZE * qSin(angle - M_PI / 6)
    );

    QPointF arrowP2 = end - QPointF(
        ARROW_SIZE * qCos(angle + M_PI / 6),
        ARROW_SIZE * qSin(angle + M_PI / 6)
    );

    painter.setBrush(painter.pen().color());
    QPolygonF arrowHead;
    arrowHead << end << arrowP1 << arrowP2;
    painter.drawPolygon(arrowHead);
}

bool GraphEditorWidget::isEdgeHighlighted(int src, int dest) const
{
    if (m_highlightedPath.size() < 2) {
        return false;
    }

    for (int i = 0; i < m_highlightedPath.size() - 1; i++) {
        if (m_highlightedPath[i] == src && m_highlightedPath[i + 1] == dest) {
            return true;
        }
    }

    return false;
}

QPointF GraphEditorWidget::getNodeCenter(int nodeId) const
{
    if (nodeId >= 0 && nodeId < m_nodes.size()) {
        return m_nodes[nodeId].position;
    }
    return QPointF();
}
