/*
 * Copyright (C) 2001-2006 Anne-Marie Mahfouf    *
 *   annemarie.mahfouf@free.fr   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

//KDE headers
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
//Project headers
#include "kard.h"
#include "version.h"

static const char *description =
    I18N_NOOP("The classical Memory game for children,\n"
	          "with 6 different numbers of cards and\n"
			  "several themes.");

static KCmdLineOptions options[] =
{
    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    { 0, 0, 0 }
};

int main(int argc, char **argv)
{
    KAboutData about("kard", I18N_NOOP("Kard"), KARD_VERSION, description,
                     KAboutData::License_GPL, "(C) 2001-2005 Anne-Marie Mahfouf", 0, 0, "annma@kde.org");
    about.addAuthor( "Anne-Marie Mahfouf", 0, "annma@kde.org" );
    about.addCredit("Matt Howe",
                            I18N_NOOP("Graphics Opposites Theme"), "mdhowe@bigfoot.com");
    about.addCredit("Renaud Blanchard",
                            I18N_NOOP("Graphics House Theme"), "kisukuma@chez.com");
    about.addCredit("Primoz Anzur",
    			    I18N_NOOP("Coding help, bugs fixing"), "zerokode@gmx.net");
    about.addCredit("Stephan Binner",
    			    I18N_NOOP("i18n issues"), "binner@kde.org");
    about.addCredit("Danny Allen",
		    I18N_NOOP("Compiled Animals and Food themes"), "danny@dannyallen.co.uk");
    about.addCredit("Bojan",
		    I18N_NOOP("Game sounds support, fixes"), "dbojan@gmail.com");
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // see if we are starting with session management
    if (app.isSessionRestored())
        RESTORE(Kard)
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            Kard *widget = new Kard;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                Kard *widget = new Kard;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
