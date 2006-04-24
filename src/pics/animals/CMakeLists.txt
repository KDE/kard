kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/pics/animals FILES ant.png camel.png duck.png grasshopper.png scorpion.png seal.png bird.png dolphin.png giraffe_fish.png ladybug.png seahorse.png sheep.png)

kde4_footer()



#original Makefile.am contents follow:

#picsdir = $(kde_datadir)/kard/pics/animals
#pics_DATA = ant.png camel.png duck.png grasshopper.png scorpion.png seal.png bird.png dolphin.png giraffe_fish.png ladybug.png seahorse.png sheep.png

#EXTRA_DIST = $(pics_DATA)
