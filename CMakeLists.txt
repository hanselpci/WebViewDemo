cmake_minimum_required(VERSION 3.10)

set(WebViewDir D:/Data/Develop/C++/WebView/0.12.0)

project(WebviewTextViewer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(text-viewer src/main.c)

if (WIN32)
    target_include_directories(text-viewer PRIVATE ${WebViewDir}/include)
    target_link_directories(text-viewer PRIVATE ${WebViewDir}/lib)
    set(EXTRA_LIBS webview_static user32.lib)
elseif (LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)
    include_directories(${GTK_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})
    link_directories(${GTK_LIBRARY_DIRS} ${WEBKIT_LIBRARY_DIRS})
    add_definitions(${GTK_CFLAGS_OTHER} ${WEBKIT_CFLAGS_OTHER})
    set(EXTRA_LIBS ${GTK_LIBRARIES} ${WEBKIT_LIBRARIES})
elseif (APPLE)
    find_library(WEBKIT_LIB WebKit)
    find_library(COCOA_LIB Cocoa)
    set(EXTRA_LIBS ${WEBKIT_LIB} ${COCOA_LIB})
endif()

target_link_libraries(text-viewer ${EXTRA_LIBS})

# Copy HTML file to build directory
# file(COPY ${PROJECT_SOURCE_DIR}/web DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/web DESTINATION ${CMAKE_BINARY_DIR}/Debug)