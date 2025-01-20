cmake_minimum_required(VERSION 3.16)

project(musicPlayer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/Users/tahira/Qt/6.8.1/macos/lib/cmake" )
include_directories("/opt/homebrew/Cellar/sfml/3.0.0/include")

#set(SFML_AUDIO ON CACHE BOOL "Enable SFML Audio module")
#set(QT_DEBUG_FIND_PACKAGE ON CACHE BOOL "Enable debugging output for finding Qt packages")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(SFML 3.0 COMPONENTS Audio REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

qt_add_executable(musicPlayer src/main.cpp src/MusicPlayer.cpp src/NonGUIMusicPlayer.cpp)
find_library(SFML_AUDIO_LIBRARY NAMES sfml-audio PATHS "/opt/homebrew/Cellar/sfml/3.0.0/lib/")

target_link_libraries(musicPlayer PRIVATE Qt6::Widgets Qt6::Core ${SFML_AUDIO_LIBRARY})

set_target_properties(musicPlayer PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)