#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

ControlPanel::~ControlPanel()
{
}

int ControlPanel::getNumVertices() const
{
    return m_verticesSpinBox->value();
}

bool ControlPanel::isWeighted() const
{
    return m_weightedCheckBox->isChecked();
}

bool ControlPanel::isDirected() const
{
    return m_directedCheckBox->isChecked();
}

int ControlPanel::getStartVertex() const
{
    return m_startVertexSpinBox->value();
}

int ControlPanel::getEndVertex() const
{
    return m_endVertexSpinBox->value();
}

int ControlPanel::getEdgeSrc() const
{
    return m_edgeSrcSpinBox->value();
}

int ControlPanel::getEdgeDest() const
{
    return m_edgeDestSpinBox->value();
}

double ControlPanel::getEdgeWeight() const
{
    return m_edgeWeightSpinBox->value();
}

void ControlPanel::setGraphCreated(bool created)
{
    // Disable graph creation controls when graph exists
    m_verticesSpinBox->setEnabled(!created);
    m_weightedCheckBox->setEnabled(!created);
    m_directedCheckBox->setEnabled(!created);
    m_createGraphButton->setEnabled(!created);

    // Enable other controls when graph exists
    m_clearGraphButton->setEnabled(created);
    m_edgeInputGroup->setEnabled(created);
    m_pathfindingGroup->setEnabled(created);
    m_saveGraphButton->setEnabled(created);
    m_autoLayoutButton->setEnabled(created);
}

void ControlPanel::setMaxVertexValue(int maxValue)
{
    int max = maxValue - 1; // 0-indexed

    m_edgeSrcSpinBox->setMaximum(max);
    m_edgeDestSpinBox->setMaximum(max);
    m_startVertexSpinBox->setMaximum(max);
    m_endVertexSpinBox->setMaximum(max);

    // Reset values if they exceed new maximum
    if (m_edgeSrcSpinBox->value() > max) m_edgeSrcSpinBox->setValue(0);
    if (m_edgeDestSpinBox->value() > max) m_edgeDestSpinBox->setValue(0);
    if (m_startVertexSpinBox->value() > max) m_startVertexSpinBox->setValue(0);
    if (m_endVertexSpinBox->value() > max) m_endVertexSpinBox->setValue(0);
}

void ControlPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    createGraphConfigGroup();
    createEdgeInputGroup();
    createPathfindingGroup();
    createFileOperationsGroup();
    createLayoutGroup();

    mainLayout->addWidget(m_graphConfigGroup);
    mainLayout->addWidget(m_edgeInputGroup);
    mainLayout->addWidget(m_pathfindingGroup);
    mainLayout->addWidget(m_fileOperationsGroup);
    mainLayout->addWidget(m_layoutGroup);
    mainLayout->addStretch();

    // Initial state: no graph created
    setGraphCreated(false);
}

void ControlPanel::createGraphConfigGroup()
{
    m_graphConfigGroup = new QGroupBox("Graph Configuration", this);
    QVBoxLayout* layout = new QVBoxLayout(m_graphConfigGroup);

    QFormLayout* formLayout = new QFormLayout();

    // Number of vertices
    m_verticesSpinBox = new QSpinBox();
    m_verticesSpinBox->setRange(2, 100);
    m_verticesSpinBox->setValue(5);
    formLayout->addRow("Vertices:", m_verticesSpinBox);

    // Weighted checkbox
    m_weightedCheckBox = new QCheckBox();
    m_weightedCheckBox->setChecked(false);
    formLayout->addRow("Weighted:", m_weightedCheckBox);

    // Directed checkbox
    m_directedCheckBox = new QCheckBox();
    m_directedCheckBox->setChecked(false);
    formLayout->addRow("Directed:", m_directedCheckBox);

    layout->addLayout(formLayout);

    // Create graph button
    m_createGraphButton = new QPushButton("Create Graph");
    QFont boldFont = m_createGraphButton->font();
    boldFont.setBold(true);
    m_createGraphButton->setFont(boldFont);
    layout->addWidget(m_createGraphButton);

    // Clear graph button
    m_clearGraphButton = new QPushButton("Clear Graph");
    layout->addWidget(m_clearGraphButton);

    connect(m_createGraphButton, &QPushButton::clicked,
            this, &ControlPanel::onCreateGraphClicked);
    connect(m_clearGraphButton, &QPushButton::clicked,
            this, &ControlPanel::onClearGraphClicked);
    connect(m_weightedCheckBox, &QCheckBox::stateChanged,
            this, &ControlPanel::onWeightedChanged);
}

void ControlPanel::createEdgeInputGroup()
{
    m_edgeInputGroup = new QGroupBox("Add Edge", this);
    QVBoxLayout* layout = new QVBoxLayout(m_edgeInputGroup);

    QFormLayout* formLayout = new QFormLayout();

    // Source vertex
    m_edgeSrcSpinBox = new QSpinBox();
    m_edgeSrcSpinBox->setRange(0, 0);
    formLayout->addRow("From:", m_edgeSrcSpinBox);

    // Destination vertex
    m_edgeDestSpinBox = new QSpinBox();
    m_edgeDestSpinBox->setRange(0, 0);
    formLayout->addRow("To:", m_edgeDestSpinBox);

    // Weight (only shown for weighted graphs)
    m_edgeWeightLabel = new QLabel("Weight:");
    m_edgeWeightSpinBox = new QSpinBox();
    m_edgeWeightSpinBox->setRange(1, 1000);
    m_edgeWeightSpinBox->setValue(1);
    formLayout->addRow(m_edgeWeightLabel, m_edgeWeightSpinBox);

    layout->addLayout(formLayout);

    // Add edge button
    m_addEdgeButton = new QPushButton("Add Edge");
    layout->addWidget(m_addEdgeButton);

    connect(m_addEdgeButton, &QPushButton::clicked,
            this, &ControlPanel::onAddEdgeClicked);

    // Initially hide weight controls
    m_edgeWeightLabel->setVisible(false);
    m_edgeWeightSpinBox->setVisible(false);
}

void ControlPanel::createPathfindingGroup()
{
    m_pathfindingGroup = new QGroupBox("Find Path", this);
    QVBoxLayout* layout = new QVBoxLayout(m_pathfindingGroup);

    QFormLayout* formLayout = new QFormLayout();

    // Start vertex
    m_startVertexSpinBox = new QSpinBox();
    m_startVertexSpinBox->setRange(0, 0);
    formLayout->addRow("Start:", m_startVertexSpinBox);

    // End vertex
    m_endVertexSpinBox = new QSpinBox();
    m_endVertexSpinBox->setRange(0, 0);
    formLayout->addRow("End:", m_endVertexSpinBox);

    layout->addLayout(formLayout);

    // Find path button
    m_findPathButton = new QPushButton("Find Path");
    QFont boldFont2 = m_findPathButton->font();
    boldFont2.setBold(true);
    m_findPathButton->setFont(boldFont2);
    m_findPathButton->setStyleSheet("background-color: #4CAF50; color: white;");
    layout->addWidget(m_findPathButton);

    connect(m_findPathButton, &QPushButton::clicked,
            this, &ControlPanel::onFindPathClicked);
}

void ControlPanel::createFileOperationsGroup()
{
    m_fileOperationsGroup = new QGroupBox("File Operations", this);
    QVBoxLayout* layout = new QVBoxLayout(m_fileOperationsGroup);

    // Load graph button
    m_loadGraphButton = new QPushButton("Load Graph");
    layout->addWidget(m_loadGraphButton);

    // Save graph button
    m_saveGraphButton = new QPushButton("Save Graph");
    layout->addWidget(m_saveGraphButton);

    connect(m_loadGraphButton, &QPushButton::clicked,
            this, &ControlPanel::onLoadGraphClicked);
    connect(m_saveGraphButton, &QPushButton::clicked,
            this, &ControlPanel::onSaveGraphClicked);
}

void ControlPanel::createLayoutGroup()
{
    m_layoutGroup = new QGroupBox("Layout", this);
    QVBoxLayout* layout = new QVBoxLayout(m_layoutGroup);

    // Auto layout button
    m_autoLayoutButton = new QPushButton("Auto Layout");
    layout->addWidget(m_autoLayoutButton);

    connect(m_autoLayoutButton, &QPushButton::clicked,
            this, &ControlPanel::onAutoLayoutClicked);
}

void ControlPanel::onCreateGraphClicked()
{
    emit createGraphRequested(getNumVertices(), isWeighted(), isDirected());
}

void ControlPanel::onClearGraphClicked()
{
    emit clearGraphRequested();
}

void ControlPanel::onAddEdgeClicked()
{
    double weight = isWeighted() ? getEdgeWeight() : 1.0;
    emit addEdgeRequested(getEdgeSrc(), getEdgeDest(), weight);
}

void ControlPanel::onFindPathClicked()
{
    emit findPathRequested(getStartVertex(), getEndVertex());
}

void ControlPanel::onLoadGraphClicked()
{
    emit loadGraphRequested();
}

void ControlPanel::onSaveGraphClicked()
{
    emit saveGraphRequested();
}

void ControlPanel::onAutoLayoutClicked()
{
    emit autoLayoutRequested();
}

void ControlPanel::onWeightedChanged(int state)
{
    bool isWeighted = (state == Qt::Checked);
    m_edgeWeightLabel->setVisible(isWeighted);
    m_edgeWeightSpinBox->setVisible(isWeighted);
}
