cmake_minimum_required(VERSION 3.10)
project(3DViewer VERSION 2.0 )

# Эти настройки включают автоматическую обработку .ui (Qt Designer), .moc (метаобъекты), и .qrc (ресурсы) файлов.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавление флагов компиляции для всех целей
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")


# Добавляется флаг компиляции -O3 для максимальной оптимизации.
add_compile_options(-O3)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

include_directories(view/) 


if(NOT EXISTS  ${CMAKE_SOURCE_DIR}/ffmpeg/lib/ffmpeg)
    execute_process(
        COMMAND curl -L -o ffmpeg.zip https://evermeet.cx/ffmpeg/ffmpeg-7.1.zip
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ffmpeg/lib
    )

    execute_process(
        COMMAND unzip ffmpeg.zip -d .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ffmpeg/lib
    )

    execute_process(
        COMMAND rm ffmpeg.zip
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ffmpeg/lib
    )
endif()

add_executable(3DViewer
        main.cc
        view/mainwindow.cc
        view/mainwindow.ui
        view/myopenglwidget.cc
        model/parser/parser.cc
        model/affineTransformations/affineStrategy.cc
)

target_link_libraries(3DViewer PRIVATE Qt6::Widgets Qt6::OpenGLWidgets)


# Указываем путь до директорий
target_include_directories(3DViewer PRIVATE
    ${CMAKE_SOURCE_DIR}/model/affineTransformations
    ${CMAKE_SOURCE_DIR}/model/parser
    ${CMAKE_SOURCE_DIR}/model
    ${CMAKE_SOURCE_DIR}/controller
    ${CMAKE_SOURCE_DIR}/view
    ${CMAKE_SOURCE_DIR}
)

