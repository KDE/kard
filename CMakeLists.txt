kde4_header()
project(kard)

add_subdirectory( src ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_create_doxygen_docs(  )

kde4_footer()

#original Makefile.am contents follow:
#SUBDIRS = src

#EXTRA_DIST = TODO README AUTHORS Doxyfile INSTALL ChangeLog COPYING

#AUTOMAKE_OPTIONS = foreign