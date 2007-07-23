
add_subdirectory( data ) 
add_subdirectory( pics ) 
add_subdirectory( sounds ) 

#include_directories( ${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(kard_SRCS 
   main.cpp 
   kard.cpp 
   kardview.cpp 
   kardwidget.cpp )

kde4_add_ui_files(kard_SRCS 
   generalui.ui 
   themeui.ui )

kde4_add_kcfg_files(kard_SRCS kardsettings.kcfgc )

kde4_add_executable(kard ${kard_SRCS})

target_link_libraries(kard ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBRARY} ${QT_QT3SUPPORT_LIBRARY} )

install(TARGETS kard  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES kard.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kard.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES kardui.rc DESTINATION ${DATA_INSTALL_DIR}/kard )



