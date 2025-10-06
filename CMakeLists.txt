cmake_minimum_required(VERSION 3.16)
project(unimalen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui LinguistTools)

qt6_standard_project_setup()

# ===== Core Library =====
add_library(unimalen-core STATIC
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

target_link_libraries(unimalen-core PUBLIC Qt6::Core Qt6::Gui)
target_include_directories(unimalen-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ===== Tools Library =====
add_library(unimalen-tools STATIC
    src/tools/Tool.h
    src/tools/ToolManager.h
    src/tools/ToolManager.cpp
    src/tools/PencilTool.h
    src/tools/PencilTool.cpp
    src/tools/BrushTool.h
    src/tools/BrushTool.cpp
    src/tools/EraserTool.h
    src/tools/EraserTool.cpp
    src/tools/LineTool.h
    src/tools/LineTool.cpp
)

target_link_libraries(unimalen-tools PUBLIC unimalen-core Qt6::Core Qt6::Gui)
target_include_directories(unimalen-tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
)

add_library(unimalen-ui STATIC ${UI_SOURCES} ${UI_HEADERS})
target_link_libraries(unimalen-ui PUBLIC unimalen-tools unimalen-core Qt6::Core Qt6::Widgets Qt6::Gui)
target_include_directories(unimalen-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ===== Application =====
qt6_add_executable(unimalen
    src/main.cpp
)

target_link_libraries(unimalen PRIVATE unimalen-ui unimalen-tools unimalen-core Qt6::Core Qt6::Widgets Qt6::Gui)

# Add translation files
set(TRANSLATION_FILES
    translations/unimalen_en.ts
    translations/unimalen_es.ts
    translations/unimalen_fr.ts
    translations/unimalen_de.ts
)

qt6_add_translations(unimalen TS_FILES ${TRANSLATION_FILES})

# Set application properties
set_target_properties(unimalen PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Install fonts
install(DIRECTORY ${CMAKE_SOURCE_DIR}/fonts/
    DESTINATION share/unimalen/fonts
    FILES_MATCHING PATTERN "*.ttf"
)
