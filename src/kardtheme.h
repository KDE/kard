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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef KARDTHEME_H
#define KARDTHEME_H

#include <QPalette>

class KardTheme
{
public:
    KardTheme();
    virtual ~KardTheme();

    ///The name of theme as in the folder
    virtual QString name() const = 0;
    ///The name of the theme in the menu
    virtual QString uiName() const = 0;

    virtual bool load() const = 0;
    
};

class KardThemeFactory
{
public:
    static KardThemeFactory* instance();
    ~KardThemeFactory();

    KardTheme* buildTheme(int id) const;
    ///List of the themes folder names
    QStringList themeList() const;
    ///List of the themes menu names
    QStringList themeUiList() const;

private:
    KardThemeFactory();
};

#endif

// kate: space-indent on; tab-width 4; indent-width 4; mixed-indent off; replace-tabs on;
// vim: set et sw=4 ts=4 cino=l1,cs,U1:

