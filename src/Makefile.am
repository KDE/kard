## Makefile.am for kard

bin_PROGRAMS = kard
kard_SOURCES = main.cpp kard.cpp kardview.cpp kardwidget.cpp theme.ui timer.ui kardsettings.kcfgc
kard_LDADD   = $(LIB_KFILE)

SUBDIRS = $(AUTODIRS)

EXTRA_DIST = kard.h kard.desktop mainw.ui kardview.h kardwidget.h version.h pref.h
KDE_ICON = AUTO

appsdir = $(kde_appsdir)/Edutainment/Miscellaneous
apps_DATA = kard.desktop

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

kde_kcfg_DATA = kard.kcfg

# the library search path.
kard_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rc_DATA = kardui.rc
rcdir = $(kde_datadir)/kard

messages:  rc.cpp
	$(EXTRACTRC) *.ui *.rc > rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kard.pot; \
	fi

