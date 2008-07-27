/*
 *   Copyright 2001-2008 Anne-Marie Mahfouf                                *
 *   annma@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

//KDE headers
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KLocale>
//Project headers
#include "kard.h"
#include "version.h"

static const char *description =
I18N_NOOP("The classical Memory game for children,\n"
		"with 6 different numbers of cards and\n"
			"several themes.");

int main(int argc, char **argv)
{
    KAboutData about("kard", 0, ki18n("Kard"), KARD_VERSION, ki18n(description),
		    KAboutData::License_GPL, ki18n("(C) 2001-2008 Anne-Marie Mahfouf"), KLocalizedString(), 0, "annma@kde.org");
    about.addAuthor( ki18n("Anne-Marie Mahfouf"), KLocalizedString(), "annma@kde.org" );
    about.addCredit(ki18n("Matt Howe"),
			ki18n("Graphics Opposites Theme"), "mdhowe@bigfoot.com");
    about.addCredit(ki18n("Renaud Blanchard"),
			ki18n("Graphics House Theme"), "kisukuma@chez.com");
    about.addCredit(ki18n("Primoz Anzur"),
			ki18n("Coding help, bugs fixing"), "zerokode@gmx.net");
    about.addCredit(ki18n("Stephan Binner"),
			ki18n("i18n issues"), "binner@kde.org");
    about.addCredit(ki18n("Danny Allen"),
		ki18n("Compiled Animals and Food themes"), "danny@dannyallen.co.uk");
    about.addCredit(ki18n("Bojan"),
		ki18n("Game sounds support, fixes"), "dbojan@gmail.com");
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // see if we are starting with session management
    if (app.isSessionRestored())  {
	RESTORE(Kard)
    }
    else  {
	// no session.. just start up normally
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	if (args->count() == 0)  {
	    Kard *widget = new Kard;
	    widget->show();
	}
	else  {
	    int i = 0;
	    for (; i < args->count(); i++)  {
		Kard *widget = new Kard;
		widget->show();
	    }
	}
	args->clear();
    }

    return app.exec();
}
