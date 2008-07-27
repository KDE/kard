/***************************************************************************
 *   Copyright   2007                      Pino Toscano <pino@kde.org>     *
 *   Copyright   2008                Anne-Marie Mahfouf <annmakde.org>     * 
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

//project headers
#include "kardtheme.h"

#include <KLocale>

KardTheme::KardTheme()
{
}

KardTheme::~KardTheme()
{
}

/// The 'colors' theme
class KardThemeColors : public KardTheme
{
public:
    KardThemeColors()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "colors";
    }

    virtual QString uiName() const
    {
        return i18n("Colors");
    }

    virtual bool load()const
    {

        return true;
    }

};

/// The 'house' theme
class KardThemeHouse : public KardTheme
{
public:
    KardThemeHouse()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "house";
    }

    virtual QString uiName() const
    {
        return i18n("House");
    }

    virtual bool load() const
    {

        return true;
    }

};


/// The 'opposites' theme
class KardThemeOpposites : public KardTheme
{
public:
    KardThemeOpposites()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "opposites";
    }

    virtual QString uiName() const
    {
        return i18n("Opposites");
    }

    virtual bool load() const
    {

        return true;
    }
};

/// The 'syllables' theme
class KardThemeSyllables : public KardTheme
{
public:
    KardThemeSyllables()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "syllables";
    }

    virtual QString uiName() const
    {
        return i18n("Syllables");
    }

    virtual bool load() const
    {

        return true;
    }
};

/// The 'animals' theme
class KardThemeAnimals : public KardTheme
{
public:
    KardThemeAnimals()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "animals";
    }

    virtual QString uiName() const
    {
        return i18n("Animals");
    }

    virtual bool load() const
    {

        return true;
    }
};

/// The 'food' theme
class KardThemeFood : public KardTheme
{
public:
    KardThemeFood()
            : KardTheme()
    {
    }

    virtual QString name() const
    {
        return "food";
    }

    virtual QString uiName() const
    {
        return i18n("Food");
    }

    virtual bool load() const
    {

        return true;
    }
};

KardThemeFactory* KardThemeFactory::instance()
{
    static KardThemeFactory factory;
    return &factory;
}

KardThemeFactory::KardThemeFactory()
{
}

KardThemeFactory::~KardThemeFactory()
{
}

KardTheme* KardThemeFactory::buildTheme(int id) const
{
    switch (id)
    {
        case 0:
            return new KardThemeColors();
        case 1:
            return new KardThemeHouse();
        case 2:
            return new KardThemeOpposites();
        case 3:
            return new KardThemeSyllables();
        case 4:
            return new KardThemeAnimals();
        case 5:
            return new KardThemeFood();
    }
    return 0;
}

#define ADD_THEME_UI_NAME( themeclass, list ) \
{ \
   themeclass x; \
   list.append( x.uiName() ); \
}

QStringList KardThemeFactory::themeUiList() const
{
    QStringList ret;
    ADD_THEME_UI_NAME( KardThemeColors, ret )
    ADD_THEME_UI_NAME( KardThemeHouse, ret )
    ADD_THEME_UI_NAME( KardThemeOpposites, ret )
    ADD_THEME_UI_NAME( KardThemeSyllables, ret )
    ADD_THEME_UI_NAME( KardThemeAnimals, ret )
    ADD_THEME_UI_NAME( KardThemeFood, ret )
    return ret;
}

#define ADD_THEME_NAME( themeclass, list ) \
{ \
   themeclass x; \
   list.append( x.name() ); \
}

QStringList KardThemeFactory::themeList() const
{
    QStringList ret;
    ADD_THEME_NAME( KardThemeColors, ret )
    ADD_THEME_NAME( KardThemeHouse, ret )
    ADD_THEME_NAME( KardThemeOpposites, ret )
    ADD_THEME_NAME( KardThemeSyllables, ret )
    ADD_THEME_NAME( KardThemeAnimals, ret )
    ADD_THEME_NAME( KardThemeFood, ret )
    return ret;
}
// kate: space-indent on; tab-width 4; indent-width 4; mixed-indent off; replace-tabs on;
// vim: set et sw=4 ts=4 cino=l1,cs,U1:

