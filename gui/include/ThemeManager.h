#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QString>
#include <QSettings>
#include <QPalette>

/**
 * @brief Theme type enumeration
 */
enum class ThemeType {
    Light,
    Dark
};

/**
 * @brief Color palette for theming
 */
struct ColorPalette {
    // Background colors
    QColor canvasBackground;
    QColor panelBackground;

    // Node colors
    QColor nodeFill;
    QColor nodeBorder;
    QColor nodeHoverFill;
    QColor nodeSelectedFill;
    QColor nodeSelectedBorder;
    QColor nodeHighlightFill;
    QColor nodeHighlightBorder;
    QColor nodeText;

    // Edge colors
    QColor edgeNormal;
    QColor edgeHighlight;
    QColor edgeText;

    // UI colors
    QColor text;
    QColor textSecondary;
    QColor border;
    QColor buttonBackground;
    QColor buttonText;
};

/**
 * @brief Manages application themes (light/dark mode)
 *
 * Singleton class that provides:
 * - Theme switching between light and dark modes
 * - Persistent theme storage
 * - Color palette access
 * - Theme change notifications via signals
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    // Singleton access
    static ThemeManager& instance();

    // Theme operations
    ThemeType currentTheme() const { return m_currentTheme; }
    void setTheme(ThemeType theme);
    void toggleTheme();

    // Color palette access
    const ColorPalette& palette() const { return m_palette; }

    // Individual color getters (convenience)
    QColor canvasBackground() const { return m_palette.canvasBackground; }
    QColor nodeFill() const { return m_palette.nodeFill; }
    QColor nodeBorder() const { return m_palette.nodeBorder; }
    QColor nodeHoverFill() const { return m_palette.nodeHoverFill; }
    QColor nodeSelectedFill() const { return m_palette.nodeSelectedFill; }
    QColor nodeSelectedBorder() const { return m_palette.nodeSelectedBorder; }
    QColor nodeHighlightFill() const { return m_palette.nodeHighlightFill; }
    QColor nodeHighlightBorder() const { return m_palette.nodeHighlightBorder; }
    QColor nodeText() const { return m_palette.nodeText; }
    QColor edgeNormal() const { return m_palette.edgeNormal; }
    QColor edgeHighlight() const { return m_palette.edgeHighlight; }
    QColor edgeText() const { return m_palette.edgeText; }
    QColor text() const { return m_palette.text; }
    QColor textSecondary() const { return m_palette.textSecondary; }

    // Apply theme to application
    void applyThemeToApplication();

signals:
    void themeChanged(ThemeType newTheme);

private:
    // Singleton pattern
    ThemeManager();
    ~ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    // Theme management
    void loadTheme();
    void saveTheme();
    void updatePalette();
    ColorPalette createLightPalette();
    ColorPalette createDarkPalette();

    // Qt palette creation
    QPalette createLightQPalette();
    QPalette createDarkQPalette();
    QString createStyleSheet();

    // Data members
    ThemeType m_currentTheme;
    ColorPalette m_palette;
    QSettings* m_settings;
};

#endif // THEMEMANAGER_H
