kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kard/data/fr FILES syllables.txt )

kde4_footer()



#original Makefile.am contents follow:

#textdir = $(kde_datadir)/kard/data/fr
#text_DATA = syllables.txt

#EXTRA_DIST = $(text_DATA)