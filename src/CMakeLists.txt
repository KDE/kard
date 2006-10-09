

add_subdirectory( data ) 
add_subdirectory( pics ) 
add_subdirectory( sounds ) 

include_directories( ${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(kard_SRCS 
   main.cpp 
   kard.cpp 
   kardview.cpp 
   kardwidget.cpp )

kde4_automoc(${kard_SRCS})

kde4_add_ui_files(kard_SRCS 
   generalui.ui 
   themeui.ui )

kde4_add_kcfg_files(kard_SRCS kardsettings.kcfgc )

kde4_add_executable(kard ${kard_SRCS})

target_link_libraries(kard  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} phononcore)

install(TARGETS kard  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kard.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kard.kcfg )
install_files( ${DATA_INSTALL_DIR}/kard FILES kardui.rc )





#original Makefile.am contents follow:

## Makefile.am for kard

#bin_PROGRAMS = kard
#kard_SOURCES = main.cpp kard.cpp kardview.cpp kardwidget.cpp themeui.ui generalui.ui kardsettings.kcfgc
#kard_LDADD   = $(LIB_KFILE)

#SUBDIRS = $(AUTODIRS)

#EXTRA_DIST = kard.h kard.desktop mainw.ui kardview.h kardwidget.h version.h pref.h
#KDE_ICON = AUTO

#appsdir = $(kde_appsdir)/Edutainment/Miscellaneous
#apps_DATA = kard.desktop

# set the include path for X, qt and KDE
#INCLUDES= $(all_includes)

#METASOURCES = AUTO

#kde_kcfg_DATA = kard.kcfg

# the library search path.
#kard_LDFLAGS = $(all_libraries) $(KDE_RPATH)

#rc_DATA = kardui.rc
#rcdir = $(kde_datadir)/kard

#messages:  rc.cpp
#	$(EXTRACTRC) *.ui *.rc > rc.cpp
#	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp #-o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
#	if test -n "$$LIST"; then \
#	 $(XGETTEXT) $$LIST -o $(podir)/kard.pot; \
#	fi
