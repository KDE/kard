

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/pics/food FILES apple_core.png candy1.png cherries.png orange.png apple.png candy2.png grape.png pineapple.png birthday_cake.png candy3.png lemon.png strawberry.png)





#original Makefile.am contents follow:

#picsdir = $(kde_datadir)/kard/pics/food
#pics_DATA = apple_core.png candy1.png cherries.png orange.png apple.png candy2.png grape.png pineapple.png birthday_cake.png candy3.png lemon.png strawberry.png

#EXTRA_DIST = $(pics_DATA)
