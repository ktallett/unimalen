cmake_minimum_required(VERSION 3.16)
project(erstelle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui LinguistTools)

qt6_standard_project_setup()

# ===== Core Library =====
add_library(erstelle-core STATIC
    src/core/Types.h
    src/core/Layer.h
    src/core/Layer.cpp
    src/core/Page.h
    src/core/Page.cpp
    src/core/Document.h
    src/core/Document.cpp
    src/core/UndoCommands.h
    src/core/UndoCommands.cpp
)

target_link_libraries(erstelle-core PUBLIC Qt6::Core Qt6::Gui)
target_include_directories(erstelle-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ===== UI Library =====
# For now, keep existing UI components as-is
set(UI_SOURCES
    src/mainwindow.cpp
    src/canvas.cpp
    src/tabwidget.cpp
    src/toolbar.cpp
    src/patternbar.cpp
    src/thicknessbar.cpp
    src/layerpanel.cpp
    src/colorbar.cpp
    src/ui/UpdateDialog.cpp
)

set(UI_HEADERS
    src/mainwindow.h
    src/canvas.h
    src/tabwidget.h
    src/toolbar.h
    src/patternbar.h
    src/thicknessbar.h
    src/layerpanel.h
    src/colorbar.h
    src/ui/UpdateDialog.h
)

add_library(erstelle-ui STATIC ${UI_SOURCES} ${UI_HEADERS})
target_link_libraries(erstelle-ui PUBLIC erstelle-core Qt6::Core Qt6::Widgets Qt6::Gui)
target_include_directories(erstelle-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ===== Application =====
qt6_add_executable(erstelle
    src/main.cpp
)

target_link_libraries(erstelle PRIVATE erstelle-ui erstelle-core Qt6::Core Qt6::Widgets Qt6::Gui)

# Add translation files
# Temporarily commented out due to path issues
# set(TRANSLATION_FILES
#     translations/erstelle_en.ts
#     translations/erstelle_es.ts
#     translations/erstelle_fr.ts
#     translations/erstelle_de.ts
# )
#
# qt6_add_translations(erstelle TS_FILES ${TRANSLATION_FILES})

# Set application properties
set_target_properties(erstelle PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Install fonts
install(DIRECTORY ${CMAKE_SOURCE_DIR}/fonts/
    DESTINATION share/erstelle/fonts
    FILES_MATCHING PATTERN "*.ttf"
)
