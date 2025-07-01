cmake_minimum_required(VERSION 3.19)
project(ProjetoFinal LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)  # Habilita a geração automática de moc (meta-object compiler) para Qt
set(CMAKE_AUTORCC ON)  # Habilita a geração automática de arquivos .qrc
set(CMAKE_AUTOUIC ON)  # Habilita a geração automática de arquivos .ui

qt_standard_project_setup()

qt_add_executable(ProjetoFinal
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    cadastrousuario.cpp
    cadastrousuario.h
    cadastrousuario.ui
    resources.qrc
    menumainwindow.h menumainwindow.cpp menumainwindow.ui
    cadastroproduto.h cadastroproduto.cpp cadastroproduto.ui
    controleestoque.h controleestoque.cpp controleestoque.ui
    listagemprodutos.h listagemprodutos.cpp listagemprodutos.ui
    detalhesproduto.h detalhesproduto.cpp detalhesproduto.ui
    editarproduto.h editarproduto.cpp editarproduto.ui
)

target_link_libraries(ProjetoFinal
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS ProjetoFinal
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ProjetoFinal
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
