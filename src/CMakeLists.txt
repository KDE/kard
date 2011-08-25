
add_subdirectory( data ) 
add_subdirectory( pics ) 
add_subdirectory( sounds ) 

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


########### next target ###############

set(kard_SRCS 
   main.cpp 
   kard.cpp 
   kardview.cpp 
   kardwidget.cpp 
   kardtheme.cpp 
   kgametheme/kgametheme.cpp
   kgametheme/kgamethemeselector.cpp)

kde4_add_ui_files(kard_SRCS 
   generalui.ui 
   themeui.ui 
   kgametheme/kgamethemeselector.ui)

kde4_add_kcfg_files(kard_SRCS kardsettings.kcfgc )

kde4_add_executable(kard ${kard_SRCS})

target_link_libraries(kard ${KDE4_KDEUI_LIBS} ${KDE4_KNEWSTUFF3_LIBS} ${KDE4_PHONON_LIBRARY} )

install(TARGETS kard  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( FILES kard.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kard.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES kardui.rc DESTINATION ${DATA_INSTALL_DIR}/kard )



