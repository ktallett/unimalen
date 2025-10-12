#pragma once

#include <QSize>
#include <QString>
#include <QColor>

namespace Unimalen {

// Common constants
constexpr int DEFAULT_CANVAS_WIDTH = 576;
constexpr int DEFAULT_CANVAS_HEIGHT = 720;
constexpr int DEFAULT_DPI = 72;

// Paper color enumeration
enum class PaperColor {
    White,
    Grey,
    Black
};

// Page size enumeration
enum class PageSize {
    A6_Portrait,
    A6_Landscape,
    A5_Portrait,
    A5_Landscape,
    A4_Portrait,
    A4_Landscape,
    Custom
};

// Page size dimensions at 72 DPI
// A6: 105 x 148 mm = 4.13 x 5.83 inches
// A5: 148 x 210 mm = 5.83 x 8.27 inches
// A4: 210 x 297 mm = 8.27 x 11.69 inches
struct PageSizeInfo {
    PageSize size;
    QString name;
    int width;   // pixels at 72 DPI
    int height;  // pixels at 72 DPI
};

// Page size definitions
inline const PageSizeInfo PAGE_SIZES[] = {
    {PageSize::A6_Portrait, "A6 Portrait", 298, 420},
    {PageSize::A6_Landscape, "A6 Landscape", 420, 298},
    {PageSize::A5_Portrait, "A5 Portrait", 420, 595},
    {PageSize::A5_Landscape, "A5 Landscape", 595, 420},
    {PageSize::A4_Portrait, "A4 Portrait", 595, 842},
    {PageSize::A4_Landscape, "A4 Landscape", 842, 595},
    {PageSize::Custom, "Custom", DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT}
};

// Helper function to get page size info
inline const PageSizeInfo& getPageSizeInfo(PageSize size) {
    for (const auto& info : PAGE_SIZES) {
        if (info.size == size) {
            return info;
        }
    }
    return PAGE_SIZES[6]; // Custom
}

// Helper function to get QColor from PaperColor
inline QColor getPaperColorValue(PaperColor color) {
    switch (color) {
        case PaperColor::White:
            return QColor(255, 255, 255);
        case PaperColor::Grey:
            return QColor(180, 180, 180);
        case PaperColor::Black:
            return QColor(0, 0, 0);
        default:
            return QColor(255, 255, 255);
    }
}

} // namespace Unimalen
