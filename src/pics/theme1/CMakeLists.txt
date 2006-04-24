kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/pics/theme1 FILES ampoule.png argent.png bague.png bol.png cd.png livre.png montre.png mug.png poste.png table.png telephone.png tv.png)

kde4_footer()



#original Makefile.am contents follow:

#picsdir = $(kde_datadir)/kard/pics/theme1
#pics_DATA = ampoule.png argent.png bague.png bol.png cd.png livre.png montre.png mug.png poste.png table.png telephone.png tv.png

#EXTRA_DIST = $(pics_DATA)
