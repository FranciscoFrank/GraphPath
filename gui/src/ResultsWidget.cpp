#include "ResultsWidget.h"
#include <QFont>

ResultsWidget::ResultsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_resultsGroup = new QGroupBox("Pathfinding Results", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(m_resultsGroup);

    m_resultsText = new QTextEdit();
    m_resultsText->setReadOnly(true);
    m_resultsText->setFont(QFont("Monospace", 9));

    groupLayout->addWidget(m_resultsText);
    layout->addWidget(m_resultsGroup);

    clearResults();
}

ResultsWidget::~ResultsWidget()
{
}

void ResultsWidget::displayResults(const GraphWrapper* graph,
                                   const QVector<PathResultData>& results,
                                   int start, int end)
{
    if (!graph || results.isEmpty()) {
        clearResults();
        return;
    }

    QString output;

    // Header
    output += "═══════════════════════════════════════════════════════════\n";
    output += "                 PATHFINDING STATISTICS                    \n";
    output += "═══════════════════════════════════════════════════════════\n\n";

    // Graph information
    output += formatGraphInfo(graph);
    output += QString("Search: from vertex %1 to vertex %2\n\n").arg(start).arg(end);

    // Algorithm results
    output += "--- Algorithm Results ---\n\n";

    for (int i = 0; i < results.size(); i++) {
        output += formatAlgorithmResult(results[i], i + 1);
        if (i < results.size() - 1) {
            output += "\n";
        }
    }

    // Performance comparison
    if (results.size() > 1) {
        output += "\n" + formatPerformanceComparison(results);
    }

    m_resultsText->setPlainText(output);
}

void ResultsWidget::clearResults()
{
    m_resultsText->setPlainText(
        "No results yet.\n\n"
        "Create a graph, add edges, and click 'Find Path' to see results."
    );
}

void ResultsWidget::onGraphChanged()
{
    clearResults();
}

QString ResultsWidget::formatGraphInfo(const GraphWrapper* graph) const
{
    QString output;
    output += "--- Graph Information ---\n";

    QString type;
    if (graph->isWeighted()) {
        type = graph->isDirected() ? "Weighted, Directed" : "Weighted, Undirected";
    } else {
        type = graph->isDirected() ? "Unweighted, Directed" : "Unweighted, Undirected";
    }

    output += QString("Type: %1\n").arg(type);
    output += QString("Vertices: %1\n").arg(graph->getNumVertices());
    output += QString("Edges: %1\n").arg(graph->getNumEdges());

    return output;
}

QString ResultsWidget::formatAlgorithmResult(const PathResultData& result, int index) const
{
    QString output;

    output += QString("[Result %1]\n").arg(index);
    output += QString("  Algorithm: %1\n").arg(result.algorithm);
    output += QString("  Time: %1 ms\n").arg(result.timeMs, 0, 'f', 3);

    if (result.found) {
        output += QString("  Path: %1\n").arg(formatPath(result.path));
        output += QString("  Total weight: %1\n").arg(result.totalWeight, 0, 'f', 1);
    } else {
        output += "  Path: Not found\n";
    }

    return output;
}

QString ResultsWidget::formatPerformanceComparison(const QVector<PathResultData>& results) const
{
    QString output;
    output += "--- Performance Comparison ---\n";

    // Find fastest algorithm
    double minTime = results[0].timeMs;
    QString fastestAlgorithm = results[0].algorithm;

    for (const PathResultData& result : results) {
        if (result.timeMs < minTime) {
            minTime = result.timeMs;
            fastestAlgorithm = result.algorithm;
        }
    }

    output += QString("Fastest algorithm: %1 (%2 ms)\n")
              .arg(fastestAlgorithm)
              .arg(minTime, 0, 'f', 3);

    return output;
}

QString ResultsWidget::formatPath(const QVector<int>& path) const
{
    if (path.isEmpty()) {
        return "Empty";
    }

    QStringList pathStrings;
    for (int vertex : path) {
        pathStrings.append(QString::number(vertex));
    }

    return pathStrings.join(" -> ");
}
