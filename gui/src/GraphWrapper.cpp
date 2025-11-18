#include "GraphWrapper.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

GraphWrapper::GraphWrapper(QObject *parent)
    : QObject(parent), m_graph(nullptr)
{
}

GraphWrapper::~GraphWrapper()
{
    cleanup();
}

void GraphWrapper::createGraph(int numVertices, bool isWeighted, bool isDirected)
{
    cleanup();

    m_graph = graph_create(numVertices, isWeighted, isDirected);

    if (!m_graph) {
        emit errorOccurred("Failed to create graph");
        return;
    }

    emit graphChanged();
}

void GraphWrapper::clearGraph()
{
    cleanup();
    emit graphChanged();
}

bool GraphWrapper::addEdge(int src, int dest, double weight)
{
    if (!m_graph) {
        emit errorOccurred("No graph created");
        return false;
    }

    if (src < 0 || src >= m_graph->num_vertices ||
        dest < 0 || dest >= m_graph->num_vertices) {
        emit errorOccurred(QString("Invalid vertices: %1 -> %2").arg(src).arg(dest));
        return false;
    }

    if (!graph_add_edge(m_graph, src, dest, weight)) {
        emit errorOccurred(QString("Failed to add edge: %1 -> %2").arg(src).arg(dest));
        return false;
    }

    emit graphChanged();
    return true;
}

bool GraphWrapper::removeEdge(int src, int dest)
{
    // TODO: Implement edge removal in C library if needed
    Q_UNUSED(src);
    Q_UNUSED(dest);
    emit errorOccurred("Edge removal not yet implemented");
    return false;
}

int GraphWrapper::getNumVertices() const
{
    return m_graph ? m_graph->num_vertices : 0;
}

int GraphWrapper::getNumEdges() const
{
    return m_graph ? m_graph->num_edges : 0;
}

bool GraphWrapper::isWeighted() const
{
    return m_graph ? m_graph->is_weighted : false;
}

bool GraphWrapper::isDirected() const
{
    return m_graph ? m_graph->is_directed : false;
}

bool GraphWrapper::hasGraph() const
{
    return m_graph != nullptr;
}

QVector<EdgeData> GraphWrapper::getEdges() const
{
    QVector<EdgeData> edges;

    if (!m_graph) {
        return edges;
    }

    // Iterate through adjacency list
    for (int i = 0; i < m_graph->num_vertices; i++) {
        Edge* edge = m_graph->adj_list[i];
        while (edge) {
            // For undirected graphs, only add edge once (i < dest to avoid duplicates)
            if (m_graph->is_directed || i <= edge->dest) {
                edges.append(EdgeData(i, edge->dest, edge->weight));
            }
            edge = edge->next;
        }
    }

    return edges;
}

QVector<PathResultData> GraphWrapper::findPaths(int start, int end)
{
    QVector<PathResultData> results;

    if (!m_graph) {
        emit errorOccurred("No graph created");
        return results;
    }

    if (start < 0 || start >= m_graph->num_vertices ||
        end < 0 || end >= m_graph->num_vertices) {
        emit errorOccurred(QString("Invalid vertices: start=%1, end=%2").arg(start).arg(end));
        return results;
    }

    // Select algorithms based on graph type (same logic as CLI)
    if (m_graph->is_weighted) {
        // Weighted graph: use Dijkstra
        PathResult* result = dijkstra_find_path(m_graph, start, end);
        if (result) {
            results.append(convertPathResult(result));
            path_result_destroy(result);
        }
    } else {
        // Unweighted graph: use BFS and DFS for comparison
        PathResult* bfs_result = bfs_find_path(m_graph, start, end);
        PathResult* dfs_result = dfs_find_path(m_graph, start, end);

        if (bfs_result) {
            results.append(convertPathResult(bfs_result));
            path_result_destroy(bfs_result);
        }

        if (dfs_result) {
            results.append(convertPathResult(dfs_result));
            path_result_destroy(dfs_result);
        }
    }

    return results;
}

bool GraphWrapper::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred(QString("Cannot open file: %1").arg(filename));
        return false;
    }

    QTextStream in(&file);

    // Read graph properties
    int numVertices, numEdges;
    bool isWeighted, isDirected;

    QString line = in.readLine();
    if (line.isNull()) {
        emit errorOccurred("Empty file");
        return false;
    }

    QStringList parts = line.split(' ');
    if (parts.size() < 4) {
        emit errorOccurred("Invalid file format");
        return false;
    }

    numVertices = parts[0].toInt();
    numEdges = parts[1].toInt();
    isWeighted = (parts[2].toInt() == 1);
    isDirected = (parts[3].toInt() == 1);

    createGraph(numVertices, isWeighted, isDirected);

    if (!m_graph) {
        return false;
    }

    // Read edges
    for (int i = 0; i < numEdges; i++) {
        line = in.readLine();
        if (line.isNull()) {
            emit errorOccurred("Unexpected end of file");
            return false;
        }

        parts = line.split(' ');
        if (parts.size() < 2) {
            emit errorOccurred(QString("Invalid edge format at line %1").arg(i + 2));
            return false;
        }

        int src = parts[0].toInt();
        int dest = parts[1].toInt();
        double weight = (parts.size() >= 3) ? parts[2].toDouble() : 1.0;

        if (!addEdge(src, dest, weight)) {
            return false;
        }
    }

    file.close();
    return true;
}

bool GraphWrapper::saveToFile(const QString& filename)
{
    if (!m_graph) {
        emit errorOccurred("No graph to save");
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred(QString("Cannot open file for writing: %1").arg(filename));
        return false;
    }

    QTextStream out(&file);

    // Write graph properties
    out << m_graph->num_vertices << " "
        << m_graph->num_edges << " "
        << (m_graph->is_weighted ? 1 : 0) << " "
        << (m_graph->is_directed ? 1 : 0) << "\n";

    // Write edges
    QVector<EdgeData> edges = getEdges();
    for (const EdgeData& edge : edges) {
        out << edge.src << " " << edge.dest;
        if (m_graph->is_weighted) {
            out << " " << edge.weight;
        }
        out << "\n";
    }

    file.close();
    return true;
}

PathResultData GraphWrapper::convertPathResult(const PathResult* result) const
{
    PathResultData data;

    if (!result) {
        return data;
    }

    data.algorithm = QString(result->algorithm);
    data.totalWeight = result->total_weight;
    data.timeMs = result->time_ms;
    data.found = result->found;

    if (result->found && result->path) {
        for (int i = 0; i < result->path_length; i++) {
            data.path.append(result->path[i]);
        }
    }

    return data;
}

void GraphWrapper::cleanup()
{
    if (m_graph) {
        graph_destroy(m_graph);
        m_graph = nullptr;
    }
}
