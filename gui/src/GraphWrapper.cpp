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

bool GraphWrapper::addVertex()
{
    if (!m_graph) {
        emit errorOccurred("No graph created");
        return false;
    }

    if (!graph_add_vertex(m_graph)) {
        emit errorOccurred("Failed to add vertex");
        return false;
    }

    emit graphChanged();
    return true;
}

bool GraphWrapper::removeVertex(int vertex)
{
    if (!m_graph) {
        emit errorOccurred("No graph created");
        return false;
    }

    if (vertex < 0 || vertex >= m_graph->num_vertices) {
        emit errorOccurred(QString("Invalid vertex: %1").arg(vertex));
        return false;
    }

    if (!graph_remove_vertex(m_graph, vertex)) {
        emit errorOccurred(QString("Failed to remove vertex: %1").arg(vertex));
        return false;
    }

    emit graphChanged();
    return true;
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
    if (!m_graph) {
        emit errorOccurred("No graph created");
        return false;
    }

    if (src < 0 || src >= m_graph->num_vertices ||
        dest < 0 || dest >= m_graph->num_vertices) {
        emit errorOccurred(QString("Invalid vertices: %1 -> %2").arg(src).arg(dest));
        return false;
    }

    if (!graph_remove_edge(m_graph, src, dest)) {
        emit errorOccurred(QString("Failed to remove edge: %1 -> %2").arg(src).arg(dest));
        return false;
    }

    emit graphChanged();
    return true;
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

bool GraphWrapper::setVertexCoordinates(int vertex, double x, double y)
{
    if (!m_graph) {
        emit errorOccurred("No graph created");
        return false;
    }

    if (vertex < 0 || vertex >= m_graph->num_vertices) {
        emit errorOccurred(QString("Invalid vertex: %1").arg(vertex));
        return false;
    }

    return graph_set_coordinates(m_graph, vertex, x, y);
}

bool GraphWrapper::hasCoordinates() const
{
    return m_graph ? graph_has_coordinates(m_graph) : false;
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
        // Weighted graph: use all weighted algorithms
        PathResult* dijkstra_result = dijkstra_find_path(m_graph, start, end);
        PathResult* bellman_result = bellman_ford_find_path(m_graph, start, end);
        PathResult* astar_zero_result = astar_zero(m_graph, start, end);

        if (dijkstra_result) {
            results.append(convertPathResult(dijkstra_result));
            path_result_destroy(dijkstra_result);
        }

        if (bellman_result) {
            results.append(convertPathResult(bellman_result));
            path_result_destroy(bellman_result);
        }

        // Add A* variants with coordinates if available
        if (graph_has_coordinates(m_graph)) {
            PathResult* astar_euclidean_result = astar_euclidean(m_graph, start, end);
            PathResult* astar_manhattan_result = astar_manhattan(m_graph, start, end);

            if (astar_euclidean_result) {
                results.append(convertPathResult(astar_euclidean_result));
                path_result_destroy(astar_euclidean_result);
            }

            if (astar_manhattan_result) {
                results.append(convertPathResult(astar_manhattan_result));
                path_result_destroy(astar_manhattan_result);
            }
        }

        // Always include A* with zero heuristic
        if (astar_zero_result) {
            results.append(convertPathResult(astar_zero_result));
            path_result_destroy(astar_zero_result);
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

    // Read graph properties (CLI format: 4 lines)
    int numVertices, numEdges;
    bool isWeighted, isDirected;

    // Line 1: number of vertices
    QString line = in.readLine();
    if (line.isNull()) {
        emit errorOccurred("Empty file");
        return false;
    }
    numVertices = line.trimmed().toInt();

    // Line 2: is weighted (y/n)
    line = in.readLine();
    if (line.isNull()) {
        emit errorOccurred("Invalid file format: missing weighted flag");
        return false;
    }
    QString weightedFlag = line.trimmed().toLower();
    isWeighted = (weightedFlag == "y" || weightedFlag == "yes" || weightedFlag == "1");

    // Line 3: is directed (y/n)
    line = in.readLine();
    if (line.isNull()) {
        emit errorOccurred("Invalid file format: missing directed flag");
        return false;
    }
    QString directedFlag = line.trimmed().toLower();
    isDirected = (directedFlag == "y" || directedFlag == "yes" || directedFlag == "1");

    // Line 4: number of edges
    line = in.readLine();
    if (line.isNull()) {
        emit errorOccurred("Invalid file format: missing edge count");
        return false;
    }
    numEdges = line.trimmed().toInt();

    createGraph(numVertices, isWeighted, isDirected);

    if (!m_graph) {
        return false;
    }

    // Read edges
    QStringList parts;
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
