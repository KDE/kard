kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/pics/theme2 FILES bluehex2.png bluehex.png cyancircle2.png cyancircle.png greenhex2.png greenhex.png greenstar2.png greenstar.png orangecircle2.png orangecircle.png oval2.png oval.png redsquare2.png redsquare.png rhombus2.png rhombus.png star2.png star.png trapez2.png trapez.png triangle2.png triangle.png violetsquare2.png violetsquare.png)

kde4_footer()



#original Makefile.am contents follow:

#picsdir = $(kde_datadir)/kard/pics/theme2
#pics_DATA = bluehex2.png bluehex.png cyancircle2.png cyancircle.png greenhex2.png greenhex.png greenstar2.png greenstar.png orangecircle2.png orangecircle.png oval2.png oval.png redsquare2.png redsquare.png rhombus2.png rhombus.png star2.png star.png trapez2.png trapez.png triangle2.png triangle.png violetsquare2.png violetsquare.png

#EXTRA_DIST = $(pics_DATA)
