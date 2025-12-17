#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include "GraphWrapper.h"

/**
 * @brief Results display widget for pathfinding statistics
 *
 * This widget displays:
 * - Graph information (type, vertices, edges)
 * - Algorithm results for each pathfinding algorithm
 * - Execution times and performance comparison
 * - Path details (vertices and total weight)
 *
 * Mimics the CLI output format but in a GUI-friendly way
 */
class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    ~ResultsWidget();

    // Display results
    void displayResults(const GraphWrapper* graph,
                       const QVector<PathResultData>& results,
                       int start, int end,
                       int highlightedIndex = -1,
                       const QString& highlightReason = QString());

    // Clear results
    void clearResults();

public slots:
    void onGraphChanged();

private:
    QTextEdit* m_resultsText;
    QGroupBox* m_resultsGroup;

    // Helper methods
    QString formatGraphInfo(const GraphWrapper* graph) const;
    QString formatAlgorithmResult(const PathResultData& result, int index) const;
    QString formatPerformanceComparison(const QVector<PathResultData>& results) const;
    QString formatPath(const QVector<int>& path) const;
};

#endif // RESULTSWIDGET_H
