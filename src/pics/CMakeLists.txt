

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
add_subdirectory(animals)
add_subdirectory(food)
add_subdirectory(theme1)
add_subdirectory(theme2)

########### install files ###############






#original Makefile.am contents follow:

#SUBDIRS = $(AUTODIRS)
