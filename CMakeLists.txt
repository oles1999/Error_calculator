cmake_minimum_required(VERSION 3.19)
project(Error_calculator LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(Boost_ROOT "E:/CPPLIBS/boost_1_92_0")
set(BOOST_ROOT "E:/CPPLIBS/boost_1_92_0")
set(Boost_NO_SYSTEM_PATHS ON)
find_package(Boost REQUIRED)

qt_standard_project_setup()

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

qt_add_executable(Error_calculator
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    ui/mainwindow.ui
    src/datatable.h
    src/datatable.cpp
    resources/app.rc
    README.uk.md
    LICENSE
    README.md
)


target_link_libraries(Error_calculator
    PRIVATE
        Qt::Core
        Qt::Widgets
        Boost::boost
)

# Налаштування інсталяції
install(TARGETS Error_calculator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Офіційний автоматичний деплой Qt 6 (під MSVC працює без помилок)
qt_generate_deploy_app_script(
    TARGET Error_calculator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})