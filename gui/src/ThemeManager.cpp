#include "ThemeManager.h"
#include <QApplication>

ThemeManager::ThemeManager()
    : m_currentTheme(ThemeType::Light)
    , m_settings(new QSettings("GraphPath", "GUI"))
{
    loadTheme();
    updatePalette();
}

ThemeManager::~ThemeManager()
{
    delete m_settings;
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setTheme(ThemeType theme)
{
    if (m_currentTheme == theme) {
        return;
    }

    m_currentTheme = theme;
    updatePalette();
    saveTheme();
    emit themeChanged(theme);
}

void ThemeManager::toggleTheme()
{
    ThemeType newTheme = (m_currentTheme == ThemeType::Light)
                         ? ThemeType::Dark
                         : ThemeType::Light;
    setTheme(newTheme);
}

void ThemeManager::loadTheme()
{
    QString themeStr = m_settings->value("theme", "light").toString();
    m_currentTheme = (themeStr == "dark") ? ThemeType::Dark : ThemeType::Light;
}

void ThemeManager::saveTheme()
{
    QString themeStr = (m_currentTheme == ThemeType::Dark) ? "dark" : "light";
    m_settings->setValue("theme", themeStr);
}

void ThemeManager::updatePalette()
{
    m_palette = (m_currentTheme == ThemeType::Dark)
                ? createDarkPalette()
                : createLightPalette();

    // Apply theme to entire application
    applyThemeToApplication();
}

ColorPalette ThemeManager::createLightPalette()
{
    ColorPalette palette;

    // Background colors
    palette.canvasBackground = QColor(255, 255, 255);      // White
    palette.panelBackground = QColor(245, 245, 245);       // Light gray

    // Node colors
    palette.nodeFill = QColor(255, 255, 255);              // White
    palette.nodeBorder = QColor(0, 0, 0);                  // Black
    palette.nodeHoverFill = QColor(240, 240, 240);         // Light gray
    palette.nodeSelectedFill = QColor(255, 255, 200);      // Light yellow
    palette.nodeSelectedBorder = QColor(0, 0, 139);        // Dark blue
    palette.nodeHighlightFill = QColor(100, 200, 255);     // Cyan
    palette.nodeHighlightBorder = QColor(0, 100, 200);     // Blue
    palette.nodeText = QColor(0, 0, 0);                    // Black

    // Edge colors
    palette.edgeNormal = QColor(0, 0, 0);                  // Black
    palette.edgeHighlight = QColor(255, 100, 100);         // Red
    palette.edgeText = QColor(0, 0, 0);                    // Black

    // UI colors
    palette.text = QColor(0, 0, 0);                        // Black
    palette.textSecondary = QColor(128, 128, 128);         // Gray
    palette.border = QColor(200, 200, 200);                // Light gray
    palette.buttonBackground = QColor(76, 175, 80);        // Green
    palette.buttonText = QColor(255, 255, 255);            // White

    return palette;
}

ColorPalette ThemeManager::createDarkPalette()
{
    ColorPalette palette;

    // Background colors
    palette.canvasBackground = QColor(30, 30, 30);         // Dark gray
    palette.panelBackground = QColor(45, 45, 45);          // Medium dark gray

    // Node colors
    palette.nodeFill = QColor(60, 60, 60);                 // Dark gray
    palette.nodeBorder = QColor(200, 200, 200);            // Light gray
    palette.nodeHoverFill = QColor(80, 80, 80);            // Lighter dark gray
    palette.nodeSelectedFill = QColor(100, 100, 50);       // Dark yellow
    palette.nodeSelectedBorder = QColor(135, 206, 250);    // Sky blue
    palette.nodeHighlightFill = QColor(70, 130, 180);      // Steel blue
    palette.nodeHighlightBorder = QColor(100, 180, 255);   // Light blue
    palette.nodeText = QColor(255, 255, 255);              // White

    // Edge colors
    palette.edgeNormal = QColor(180, 180, 180);            // Light gray
    palette.edgeHighlight = QColor(255, 120, 120);         // Light red
    palette.edgeText = QColor(220, 220, 220);              // Off-white

    // UI colors
    palette.text = QColor(220, 220, 220);                  // Off-white
    palette.textSecondary = QColor(150, 150, 150);         // Medium gray
    palette.border = QColor(80, 80, 80);                   // Dark gray
    palette.buttonBackground = QColor(56, 142, 60);        // Dark green
    palette.buttonText = QColor(255, 255, 255);            // White

    return palette;
}

QPalette ThemeManager::createLightQPalette()
{
    QPalette palette;

    // Window and base colors
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, QColor(0, 0, 0));
    palette.setColor(QPalette::Base, QColor(255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
    palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
    palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));

    // Text colors
    palette.setColor(QPalette::Text, QColor(0, 0, 0));
    palette.setColor(QPalette::PlaceholderText, QColor(128, 128, 128));

    // Button colors
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));

    // Highlight colors
    palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));

    // Link colors
    palette.setColor(QPalette::Link, QColor(0, 100, 200));
    palette.setColor(QPalette::LinkVisited, QColor(100, 0, 200));

    // Other colors
    palette.setColor(QPalette::BrightText, QColor(255, 0, 0));
    palette.setColor(QPalette::Light, QColor(255, 255, 255));
    palette.setColor(QPalette::Midlight, QColor(247, 247, 247));
    palette.setColor(QPalette::Dark, QColor(160, 160, 160));
    palette.setColor(QPalette::Mid, QColor(200, 200, 200));
    palette.setColor(QPalette::Shadow, QColor(105, 105, 105));

    return palette;
}

QPalette ThemeManager::createDarkQPalette()
{
    QPalette palette;

    // Window and base colors
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, QColor(220, 220, 220));
    palette.setColor(QPalette::Base, QColor(35, 35, 35));
    palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    palette.setColor(QPalette::ToolTipBase, QColor(60, 60, 60));
    palette.setColor(QPalette::ToolTipText, QColor(220, 220, 220));

    // Text colors
    palette.setColor(QPalette::Text, QColor(220, 220, 220));
    palette.setColor(QPalette::PlaceholderText, QColor(120, 120, 120));

    // Button colors
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, QColor(220, 220, 220));

    // Highlight colors
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));

    // Link colors
    palette.setColor(QPalette::Link, QColor(100, 180, 255));
    palette.setColor(QPalette::LinkVisited, QColor(180, 100, 255));

    // Other colors
    palette.setColor(QPalette::BrightText, QColor(255, 80, 80));
    palette.setColor(QPalette::Light, QColor(70, 70, 70));
    palette.setColor(QPalette::Midlight, QColor(60, 60, 60));
    palette.setColor(QPalette::Dark, QColor(25, 25, 25));
    palette.setColor(QPalette::Mid, QColor(40, 40, 40));
    palette.setColor(QPalette::Shadow, QColor(15, 15, 15));

    // Disabled colors
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::Base, QColor(45, 45, 45));
    palette.setColor(QPalette::Disabled, QPalette::Button, QColor(45, 45, 45));

    return palette;
}

QString ThemeManager::createStyleSheet()
{
    if (m_currentTheme == ThemeType::Dark) {
        return R"(
            QMenuBar {
                background-color: #353535;
                color: #dcdcdc;
            }
            QMenuBar::item:selected {
                background-color: #2a82da;
            }
            QMenu {
                background-color: #353535;
                color: #dcdcdc;
                border: 1px solid #505050;
            }
            QMenu::item:selected {
                background-color: #2a82da;
            }
            QToolTip {
                background-color: #3c3c3c;
                color: #dcdcdc;
                border: 1px solid #505050;
            }
            QScrollBar:vertical {
                background-color: #2d2d2d;
                width: 12px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background-color: #555555;
                min-height: 20px;
                border-radius: 6px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #666666;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
            QScrollBar:horizontal {
                background-color: #2d2d2d;
                height: 12px;
                margin: 0px;
            }
            QScrollBar::handle:horizontal {
                background-color: #555555;
                min-width: 20px;
                border-radius: 6px;
            }
            QScrollBar::handle:horizontal:hover {
                background-color: #666666;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                width: 0px;
            }
            QComboBox {
                border: 1px solid #505050;
                border-radius: 3px;
                padding: 3px;
                background-color: #353535;
                color: #dcdcdc;
            }
            QComboBox:hover {
                border: 1px solid #2a82da;
            }
            QComboBox::drop-down {
                border: none;
            }
            QComboBox QAbstractItemView {
                background-color: #353535;
                color: #dcdcdc;
                selection-background-color: #2a82da;
                border: 1px solid #505050;
            }
            QSpinBox, QDoubleSpinBox {
                border: 1px solid #505050;
                border-radius: 3px;
                padding: 2px;
                background-color: #232323;
                color: #dcdcdc;
            }
            QSpinBox:hover, QDoubleSpinBox:hover {
                border: 1px solid #2a82da;
            }
            QLineEdit {
                border: 1px solid #505050;
                border-radius: 3px;
                padding: 3px;
                background-color: #232323;
                color: #dcdcdc;
            }
            QLineEdit:focus {
                border: 1px solid #2a82da;
            }
            QCheckBox {
                color: #dcdcdc;
            }
            QCheckBox::indicator {
                width: 16px;
                height: 16px;
                border: 1px solid #505050;
                border-radius: 3px;
                background-color: #232323;
            }
            QCheckBox::indicator:checked {
                background-color: #2a82da;
                border: 1px solid #2a82da;
            }
            QGroupBox {
                color: #dcdcdc;
                border: 1px solid #505050;
                border-radius: 5px;
                margin-top: 8px;
                padding-top: 8px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                subcontrol-position: top left;
                padding: 0 3px;
            }
            QStatusBar {
                background-color: #2d2d2d;
                color: #dcdcdc;
            }
            QPushButton {
                border: 1px solid #505050;
                border-radius: 3px;
                padding: 5px 10px;
                background-color: #353535;
                color: #dcdcdc;
            }
            QPushButton:hover {
                background-color: #454545;
                border: 1px solid #2a82da;
            }
            QPushButton:pressed {
                background-color: #2d2d2d;
            }
            QPushButton:disabled {
                background-color: #2d2d2d;
                color: #646464;
            }
        )";
    } else {
        // Light theme - minimal stylesheet (Qt defaults are good for light theme)
        return R"(
            QScrollBar:vertical {
                background-color: #f0f0f0;
                width: 12px;
                margin: 0px;
            }
            QScrollBar::handle:vertical {
                background-color: #c0c0c0;
                min-height: 20px;
                border-radius: 6px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #a0a0a0;
            }
            QScrollBar:horizontal {
                background-color: #f0f0f0;
                height: 12px;
                margin: 0px;
            }
            QScrollBar::handle:horizontal {
                background-color: #c0c0c0;
                min-width: 20px;
                border-radius: 6px;
            }
            QScrollBar::handle:horizontal:hover {
                background-color: #a0a0a0;
            }
        )";
    }
}

void ThemeManager::applyThemeToApplication()
{
    // Create and apply Qt palette
    QPalette qPalette = (m_currentTheme == ThemeType::Dark)
                        ? createDarkQPalette()
                        : createLightQPalette();

    QApplication::setPalette(qPalette);

    // Apply stylesheet for additional styling
    qApp->setStyleSheet(createStyleSheet());
}
