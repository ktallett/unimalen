#pragma once

#include <QString>
#include <QColor>

namespace DesignTokens {

// ===== Color System =====
namespace Colors {
    // Neutrals - Base (NumWorks-inspired)
    inline const QColor BgPrimary("#FFFFFF");       // Pure white background
    inline const QColor BgSecondary("#FAFAFA");     // Light gray surfaces
    inline const QColor BgTertiary("#F0F0F0");      // Subtle backgrounds

    inline const QColor FgPrimary("#000000");       // Primary text/icons
    inline const QColor FgSecondary("#696D79");     // Secondary text
    inline const QColor FgTertiary("#A0A0A0");      // Disabled/placeholder

    inline const QColor BorderLight("#E8E8E8");     // Subtle dividers
    inline const QColor BorderMedium("#E0E0E0");    // Standard borders
    inline const QColor BorderDark("#D0D0D0");      // Emphasized borders

    // Accent Colors (Torso T-1-inspired)
    inline const QColor AccentPrimary("#4482B4");   // Primary actions (blue)
    inline const QColor AccentSecondary("#FF9C5C"); // Secondary actions (orange)
    inline const QColor AccentTertiary("#FFB380");  // Hover states

    // Semantic Colors
    inline const QColor Success("#4CAF50");         // Connected, valid
    inline const QColor Warning("#FFA726");         // Warnings, alerts
    inline const QColor Error("#EF5350");           // Errors, critical
    inline const QColor Info("#29B6F6");            // Information

    // Data Visualization
    inline const QColor DataPrimary("#2196F3");     // Graph line 1
    inline const QColor DataSecondary("#FF9800");   // Graph line 2
    inline const QColor DataTertiary("#9C27B0");    // Graph line 3
    inline const QColor DataQuaternary("#4CAF50");  // Graph line 4

    // Dark Mode Palette
    inline const QColor BgPrimaryDark("#1A1A1A");
    inline const QColor BgSecondaryDark("#2D2D2D");
    inline const QColor BgTertiaryDark("#3A3A3A");

    inline const QColor FgPrimaryDark("#FAFAFA");
    inline const QColor FgSecondaryDark("#B8B8B8");
    inline const QColor FgTertiaryDark("#707070");

    inline const QColor BorderLightDark("#404040");
    inline const QColor BorderMediumDark("#505050");
    inline const QColor BorderDarkDark("#606060");
}

// ===== Typography =====
namespace Typography {
    // Font Families
    inline const QString PrimaryFont = "Inter, Segoe UI, Roboto, Oxygen, Ubuntu, Cantarell, Helvetica Neue, Arial, sans-serif";
    inline const QString MonoFont = "JetBrains Mono, SF Mono, Consolas, Monaco, Liberation Mono, Courier New, monospace";
    inline const QString DisplayFont = "Inter, SF Pro Display, Segoe UI, sans-serif";

    // Font Sizes (in points for Qt)
    inline constexpr int FontSizeXS = 8;      // 10.24px - Captions
    inline constexpr int FontSizeSM = 10;     // 12.80px - Small text
    inline constexpr int FontSizeBase = 12;   // 16.00px - Body text
    inline constexpr int FontSizeMD = 15;     // 20.00px - Subheadings
    inline constexpr int FontSizeLG = 19;     // 25.00px - Headings
    inline constexpr int FontSizeXL = 23;     // 31.25px - Large headings
    inline constexpr int FontSize2XL = 29;    // 39.06px - Display
    inline constexpr int FontSize3XL = 37;    // 48.83px - Hero
    inline constexpr int FontSizeLCD = 42;    // 56.00px - Main readings

    // Font Weights
    inline constexpr int WeightLight = 300;
    inline constexpr int WeightRegular = 400;
    inline constexpr int WeightMedium = 500;
    inline constexpr int WeightSemibold = 600;
    inline constexpr int WeightBold = 700;
}

// ===== Spacing & Layout =====
namespace Layout {
    // Spacing Scale (base unit: 6px)
    inline constexpr int Spacing0 = 0;
    inline constexpr int Spacing1 = 6;      // Tight spacing
    inline constexpr int Spacing2 = 12;     // Standard control spacing
    inline constexpr int Spacing3 = 18;     // Group padding, margins
    inline constexpr int Spacing4 = 24;     // Section spacing
    inline constexpr int Spacing5 = 30;     // Large sections
    inline constexpr int Spacing6 = 36;     // Extra large spacing
    inline constexpr int Spacing8 = 48;     // Major sections
    inline constexpr int Spacing10 = 60;    // Page-level spacing

    // Panel & Container Sizing
    inline constexpr int PanelMinWidth = 300;
    inline constexpr int PanelMaxWidth = 400;
    inline constexpr int WindowMinWidth = 600;
    inline constexpr int WindowMinHeight = 450;
    inline constexpr int SidebarWidth = 280;
    inline constexpr int HeaderHeight = 56;
    inline constexpr int FooterHeight = 48;

    // Content Constraints
    inline constexpr int ContentMaxWidth = 1200;
    inline constexpr int ReadingWidth = 680;

    // Border Radius
    inline constexpr int RadiusNone = 0;
    inline constexpr int RadiusSM = 4;      // Small elements
    inline constexpr int RadiusBase = 6;    // Standard buttons, inputs
    inline constexpr int RadiusMD = 8;      // Cards, panels
    inline constexpr int RadiusLG = 12;     // Large containers
    inline constexpr int RadiusXL = 16;     // Modals, dialogs
    inline constexpr int RadiusFull = 9999; // Circular elements

    // Button & Control Sizing
    inline constexpr int ButtonMinHeight = 36;
    inline constexpr int ButtonPaddingH = 16;
    inline constexpr int ButtonPaddingV = 8;
    inline constexpr int TouchTargetMin = 44;  // Minimum touch target

    // Border Widths
    inline constexpr int BorderThin = 1;
    inline constexpr int BorderMedium = 2;
    inline constexpr int BorderThick = 3;
}

} // namespace DesignTokens
