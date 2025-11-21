#include "ThemeManager.h"

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
