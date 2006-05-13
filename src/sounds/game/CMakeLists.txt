

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/sounds/game FILES kard_click.ogg kard_gameover.wav )





#original Makefile.am contents follow:

#sounddir = $(kde_datadir)/kard/sounds/game
#sound_DATA = kard_click.ogg kard_gameover.wav 

#EXTRA_DIST = $(sound_DATA)