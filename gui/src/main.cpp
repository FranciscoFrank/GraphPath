#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("GraphPath GUI");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("GraphPath");

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
