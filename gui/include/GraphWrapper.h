#ifndef GRAPHWRAPPER_H
#define GRAPHWRAPPER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QPair>

extern "C" {
    #include "graph.h"
    #include "bfs.h"
    #include "dfs.h"
    #include "dijkstra.h"
}

/**
 * @brief Edge representation for Qt
 */
struct EdgeData {
    int src;
    int dest;
    double weight;

    EdgeData(int s, int d, double w) : src(s), dest(d), weight(w) {}
};

/**
 * @brief Path result wrapper for Qt
 */
struct PathResultData {
    QString algorithm;
    QVector<int> path;
    double totalWeight;
    double timeMs;
    bool found;

    PathResultData() : totalWeight(0), timeMs(0), found(false) {}
};

/**
 * @brief C-to-Qt bridge wrapper for graph operations
 *
 * This class wraps the C graph library and provides a Qt-friendly interface.
 * It manages memory and converts between C structures and Qt data types.
 */
class GraphWrapper : public QObject
{
    Q_OBJECT

public:
    explicit GraphWrapper(QObject *parent = nullptr);
    ~GraphWrapper();

    // Graph creation and configuration
    void createGraph(int numVertices, bool isWeighted, bool isDirected);
    void clearGraph();

    // Edge operations
    bool addEdge(int src, int dest, double weight = 1.0);
    bool removeEdge(int src, int dest);

    // Graph properties
    int getNumVertices() const;
    int getNumEdges() const;
    bool isWeighted() const;
    bool isDirected() const;
    bool hasGraph() const;

    // Get graph data for visualization
    QVector<EdgeData> getEdges() const;

    // Pathfinding
    QVector<PathResultData> findPaths(int start, int end);

    // File operations
    bool loadFromFile(const QString& filename);
    bool saveToFile(const QString& filename);

signals:
    void graphChanged();
    void errorOccurred(const QString& error);

private:
    Graph* m_graph;

    // Helper methods
    PathResultData convertPathResult(const PathResult* result) const;
    void cleanup();
};

#endif // GRAPHWRAPPER_H
