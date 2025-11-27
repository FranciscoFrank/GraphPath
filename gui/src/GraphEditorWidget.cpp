#include "GraphEditorWidget.h"
#include "ThemeManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>
#include <QMenu>
#include <QAction>

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

    // Connect to theme changes
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this]() { update(); });
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

void GraphEditorWidget::addVertex(const QPointF& position)
{
    int newId = m_nodes.size();
    m_nodes.append(NodePosition(newId, position));
    update();
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

    syncCoordinatesToGraph();
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

    syncCoordinatesToGraph();
    update();
}

void GraphEditorWidget::syncCoordinatesToGraph()
{
    if (!m_graphWrapper || !m_graphWrapper->hasGraph()) {
        return;
    }

    // Normalize coordinates to a 0-1000 range for better heuristic calculations
    // This makes the coordinates independent of the actual widget size
    double minX = width();
    double maxX = 0;
    double minY = height();
    double maxY = 0;

    // Find bounds
    for (const NodePosition& node : m_nodes) {
        minX = qMin(minX, node.position.x());
        maxX = qMax(maxX, node.position.x());
        minY = qMin(minY, node.position.y());
        maxY = qMax(maxY, node.position.y());
    }

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    // Avoid division by zero
    if (rangeX < 1.0) rangeX = 1.0;
    if (rangeY < 1.0) rangeY = 1.0;

    // Set normalized coordinates for each vertex
    for (int i = 0; i < m_nodes.size(); i++) {
        double normalizedX = ((m_nodes[i].position.x() - minX) / rangeX) * 1000.0;
        double normalizedY = ((m_nodes[i].position.y() - minY) / rangeY) * 1000.0;
        m_graphWrapper->setVertexCoordinates(i, normalizedX, normalizedY);
    }
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

    // Automatically sync coordinates when graph changes
    syncCoordinatesToGraph();

    clearHighlight();
    update();
}

void GraphEditorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background with theme colors
    ThemeManager& theme = ThemeManager::instance();
    painter.fillRect(rect(), theme.canvasBackground());

    if (!m_graphWrapper || !m_graphWrapper->hasGraph()) {
        // Draw placeholder text
        painter.setPen(theme.textSecondary());
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
        ThemeManager& theme = ThemeManager::instance();
        painter.setPen(QPen(theme.nodeSelectedBorder(), 2, Qt::DashLine));
        QPointF start = getNodeCenter(m_edgeCreationStart);
        QPointF end = mapFromGlobal(QCursor::pos());
        painter.drawLine(start, end);
    }
}

void GraphEditorWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        handleRightClick(event->pos());
        return;
    }

    if (event->button() != Qt::LeftButton) {
        return;
    }

    QPointF pos = event->pos();
    int nodeId = findNodeAt(pos);

    // If in edge creation mode, complete the edge
    if (m_edgeCreationMode && nodeId >= 0) {
        // Allow self-loops: removed the nodeId != m_edgeCreationStart check
        emit edgeCreationRequested(m_edgeCreationStart, nodeId);
        m_edgeCreationMode = false;
        m_edgeCreationStart = -1;
        setCursor(Qt::ArrowCursor);
        update();
        return;
    }

    // Ctrl+Click on empty space creates a new vertex
    if (nodeId < 0 && (event->modifiers() & Qt::ControlModifier)) {
        emit vertexAddRequested(pos);
        return;
    }

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
    QPointF pos = event->pos();

    // If in edge creation mode, update visual feedback
    if (m_edgeCreationMode) {
        update();
        return;
    }

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

    // Sync coordinates after dragging
    if (m_isDragging) {
        syncCoordinatesToGraph();
    }

    m_isDragging = false;
}

void GraphEditorWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QPointF pos = event->pos();
    int nodeId = findNodeAt(pos);

    if (nodeId >= 0) {
        // Start edge creation mode
        m_edgeCreationMode = true;
        m_edgeCreationStart = nodeId;
        m_isDragging = false;  // Prevent dragging during edge creation
        setCursor(Qt::CrossCursor);
        update();
    }
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
    ThemeManager& theme = ThemeManager::instance();

    // Determine colors based on theme
    QColor fillColor = theme.nodeFill();
    QColor borderColor = theme.nodeBorder();
    int borderWidth = NODE_BORDER_WIDTH;

    if (isHighlighted) {
        fillColor = theme.nodeHighlightFill();
        borderColor = theme.nodeHighlightBorder();
        borderWidth = HIGHLIGHT_WIDTH;
    } else if (node.id == m_selectedNode) {
        fillColor = theme.nodeSelectedFill();
        borderColor = theme.nodeSelectedBorder();
    } else if (node.id == m_hoveredNode) {
        fillColor = theme.nodeHoverFill();
    }

    // Draw node circle
    painter.setPen(QPen(borderColor, borderWidth));
    painter.setBrush(fillColor);
    painter.drawEllipse(center, NODE_RADIUS, NODE_RADIUS);

    // Draw node label
    painter.setPen(theme.nodeText());
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

    ThemeManager& theme = ThemeManager::instance();
    QPointF start = m_nodes[edge.src].position;
    QPointF end = m_nodes[edge.dest].position;

    // Determine edge style based on theme
    QColor edgeColor = isHighlighted ? theme.edgeHighlight() : theme.edgeNormal();
    int edgeWidth = isHighlighted ? HIGHLIGHT_WIDTH : EDGE_WIDTH;
    painter.setPen(QPen(edgeColor, edgeWidth));

    // Handle self-loops (edge from node to itself)
    if (edge.src == edge.dest) {
        // Draw a circular loop above the node
        const double loopRadius = NODE_RADIUS * 0.8;
        const double loopOffset = NODE_RADIUS + loopRadius;

        QPointF loopCenter = start - QPointF(0, loopOffset);
        QRectF loopRect(loopCenter.x() - loopRadius, loopCenter.y() - loopRadius,
                        loopRadius * 2, loopRadius * 2);

        // Draw the loop arc (270 degrees, leaving gap for arrow)
        int startAngle = 45 * 16;  // Qt uses 1/16th degree units
        int spanAngle = 270 * 16;
        painter.drawArc(loopRect, startAngle, spanAngle);

        // Draw arrow at the end of the loop for directed graphs
        if (m_graphWrapper && m_graphWrapper->isDirected()) {
            double arrowAngle = qDegreesToRadians(45.0 + 270.0);
            QPointF arrowPos = loopCenter + QPointF(loopRadius * qCos(arrowAngle),
                                                     loopRadius * qSin(arrowAngle));
            QPointF arrowDir = arrowPos - loopCenter;
            arrowDir = QPointF(-arrowDir.y(), arrowDir.x()); // Perpendicular (tangent)
            double dirLen = qSqrt(arrowDir.x() * arrowDir.x() + arrowDir.y() * arrowDir.y());
            arrowDir /= dirLen;

            QPointF arrowStart = arrowPos - arrowDir * ARROW_SIZE;
            drawArrow(painter, arrowStart, arrowPos);
        }

        // Draw weight label for weighted graphs
        if (m_graphWrapper && m_graphWrapper->isWeighted()) {
            QPointF labelPos = loopCenter - QPointF(0, loopRadius + 5);

            painter.setPen(theme.edgeText());
            painter.setBrush(theme.canvasBackground());

            QString weightText = QString::number(edge.weight, 'g', 3);
            QFont font = painter.font();
            font.setPointSize(9);
            painter.setFont(font);

            QFontMetrics fm(font);
            QRectF textRect = fm.boundingRect(weightText);
            textRect.moveCenter(labelPos);
            textRect.adjust(-3, -3, 3, 3);

            painter.drawRect(textRect);
            painter.drawText(textRect, Qt::AlignCenter, weightText);
        }
        return;
    }

    // Normal edge (not a self-loop)
    // Calculate edge endpoints (offset by node radius)
    double angle = qAtan2(end.y() - start.y(), end.x() - start.x());
    QPointF startOffset = start + QPointF(NODE_RADIUS * qCos(angle),
                                          NODE_RADIUS * qSin(angle));
    QPointF endOffset = end - QPointF(NODE_RADIUS * qCos(angle),
                                      NODE_RADIUS * qSin(angle));

    painter.drawLine(startOffset, endOffset);

    // Draw arrow for directed graphs
    if (m_graphWrapper && m_graphWrapper->isDirected()) {
        drawArrow(painter, startOffset, endOffset);
    }

    // Draw weight label for weighted graphs
    if (m_graphWrapper && m_graphWrapper->isWeighted()) {
        QPointF midPoint = (start + end) / 2.0;

        painter.setPen(theme.edgeText());
        painter.setBrush(theme.canvasBackground());

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
    if (m_highlightedPath.size() < 1) {
        return false;
    }

    bool isDirected = m_graphWrapper && m_graphWrapper->isDirected();

    // Handle self-loops: check if the path has consecutive identical vertices
    if (src == dest) {
        for (int i = 0; i < m_highlightedPath.size() - 1; i++) {
            if (m_highlightedPath[i] == src && m_highlightedPath[i + 1] == src) {
                return true;
            }
        }
        return false;
    }

    // Handle normal edges
    for (int i = 0; i < m_highlightedPath.size() - 1; i++) {
        // Check forward direction
        if (m_highlightedPath[i] == src && m_highlightedPath[i + 1] == dest) {
            return true;
        }

        // For undirected graphs, also check reverse direction
        if (!isDirected && m_highlightedPath[i] == dest && m_highlightedPath[i + 1] == src) {
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

QPair<int, int> GraphEditorWidget::findEdgeAt(const QPointF& pos) const
{
    if (!m_graphWrapper || !m_graphWrapper->hasGraph()) {
        return QPair<int, int>(-1, -1);
    }

    QVector<EdgeData> edges = m_graphWrapper->getEdges();
    const double EDGE_HIT_TOLERANCE = 8.0; // pixels

    for (const EdgeData& edge : edges) {
        if (edge.src < 0 || edge.src >= m_nodes.size() ||
            edge.dest < 0 || edge.dest >= m_nodes.size()) {
            continue;
        }

        QPointF start = m_nodes[edge.src].position;
        QPointF end = m_nodes[edge.dest].position;

        // Handle self-loops
        if (edge.src == edge.dest) {
            const double loopRadius = NODE_RADIUS * 0.8;
            const double loopOffset = NODE_RADIUS + loopRadius;
            QPointF loopCenter = start - QPointF(0, loopOffset);

            // Check if click is near the loop circle
            double distToLoopCenter = qSqrt(QPointF::dotProduct(pos - loopCenter, pos - loopCenter));
            double distToLoopArc = qAbs(distToLoopCenter - loopRadius);

            if (distToLoopArc <= EDGE_HIT_TOLERANCE) {
                return QPair<int, int>(edge.src, edge.dest);
            }
            continue;
        }

        // Handle normal edges
        // Calculate distance from point to line segment
        QPointF lineVec = end - start;
        QPointF pointVec = pos - start;

        double lineLenSq = QPointF::dotProduct(lineVec, lineVec);
        if (lineLenSq < 0.0001) continue; // Too short

        double t = qBound(0.0, QPointF::dotProduct(pointVec, lineVec) / lineLenSq, 1.0);
        QPointF projection = start + t * lineVec;

        double distance = qSqrt(QPointF::dotProduct(pos - projection, pos - projection));

        if (distance <= EDGE_HIT_TOLERANCE) {
            return QPair<int, int>(edge.src, edge.dest);
        }
    }

    return QPair<int, int>(-1, -1);
}

void GraphEditorWidget::handleRightClick(const QPointF& pos)
{
    // First check if clicking on a node
    int nodeId = findNodeAt(pos);
    if (nodeId >= 0) {
        showNodeContextMenu(pos, nodeId);
        return;
    }

    // Then check if clicking on an edge
    QPair<int, int> edge = findEdgeAt(pos);
    if (edge.first >= 0 && edge.second >= 0) {
        showEdgeContextMenu(pos, edge.first, edge.second);
        return;
    }
}

void GraphEditorWidget::showNodeContextMenu(const QPointF& pos, int nodeId)
{
    QMenu menu(this);
    QAction* deleteAction = menu.addAction(QString("Delete Vertex %1").arg(nodeId));

    QAction* selected = menu.exec(mapToGlobal(pos.toPoint()));
    if (selected == deleteAction) {
        emit vertexRemoveRequested(nodeId);
    }
}

void GraphEditorWidget::showEdgeContextMenu(const QPointF& pos, int src, int dest)
{
    if (!m_graphWrapper) return;

    QMenu menu(this);
    QAction* deleteAction = menu.addAction(QString("Delete Edge %1 → %2").arg(src).arg(dest));

    QAction* changeWeightAction = nullptr;
    if (m_graphWrapper->isWeighted()) {
        // Find current weight
        double currentWeight = 1.0;
        QVector<EdgeData> edges = m_graphWrapper->getEdges();
        for (const EdgeData& edge : edges) {
            if (edge.src == src && edge.dest == dest) {
                currentWeight = edge.weight;
                break;
            }
        }

        changeWeightAction = menu.addAction(QString("Change Weight (current: %1)").arg(currentWeight));
    }

    QAction* selected = menu.exec(mapToGlobal(pos.toPoint()));
    if (selected == deleteAction) {
        emit edgeRemoveRequested(src, dest);
    } else if (changeWeightAction && selected == changeWeightAction) {
        // Find current weight again
        double currentWeight = 1.0;
        QVector<EdgeData> edges = m_graphWrapper->getEdges();
        for (const EdgeData& edge : edges) {
            if (edge.src == src && edge.dest == dest) {
                currentWeight = edge.weight;
                break;
            }
        }
        emit edgeWeightChangeRequested(src, dest, currentWeight);
    }
}
