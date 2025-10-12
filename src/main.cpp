#include <QApplication>
#include <QFontDatabase>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "ui/StyleSheet.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("unimalen");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("unimalen");

    // Load custom fonts from fonts directory
    QStringList fontPaths;
    fontPaths << "../fonts";  // Build directory relative path
    fontPaths << "fonts";     // Current directory
    fontPaths << "../share/unimalen/fonts";  // Install directory relative
    fontPaths << "/usr/share/unimalen/fonts";  // System install directory
    fontPaths << "/usr/local/share/unimalen/fonts";  // Local install directory

    for (const QString &fontPath : fontPaths) {
        QDir fontsDir(fontPath);
        if (fontsDir.exists()) {
            QStringList fontFiles = fontsDir.entryList(QStringList() << "*.ttf" << "*.otf", QDir::Files);
            for (const QString &fontFile : fontFiles) {
                QString fullPath = fontsDir.absoluteFilePath(fontFile);
                int fontId = QFontDatabase::addApplicationFont(fullPath);
                if (fontId != -1) {
                    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
                    qDebug() << "Loaded font:" << fontFile << "Families:" << fontFamilies;
                }
            }
            break;  // Stop after finding first valid font directory
        }
    }

    // Apply design system stylesheet
    app.setStyleSheet(StyleSheet::getApplicationStyleSheet());

    MainWindow window;
    window.setMinimumSize(DesignTokens::Layout::WindowMinWidth,
                          DesignTokens::Layout::WindowMinHeight);
    window.show();

    return app.exec();
}