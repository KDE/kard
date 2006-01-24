/*
 * Copyright (C) 2001-2006 Anne-Marie Mahfouf <annma@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KARD_H
#define KARD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>

#include "generalui.h"
#include "themeui.h"

class KComboBox;
class KSelectAction;
class KToggleAction;
class QButtonGroup;
class KardView;

const int IDS_NUMBER	= 100;
const int IDS_TIME		= 102;
const int IDS_THEME	= 104;
const int IDS_LANG		= 106;
const int IDS_TRY		= 108;

/**
 * This class serves as the main window for Kard.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Anne-Marie Mahfouf <annma@kde.org>
 * @version 0.3
 */
class Kard : public KMainWindow
{
    Q_OBJECT
public:

    ///Constructor
    Kard();
    ///Destructor
    virtual ~Kard();

	static void playSound(const QString &soundname);

private slots:

    ///In Settings menu, Configure Kard... menu item
    void optionsPreferences();
    ///Nice method for updating the statusBar when several items
    void changeStatusbar(const QString& text, int id);
    ///Call this slot if you want to change the caption on the main window
    void changeCaption(const QString& text);
    /**
     *Read settings from config file
     */
    void readConfig();
    ///Write number of cards in config
    void saveSettings();
     /**
     *When youchange the number of cards via the Combobox, the
     *statusBar is updated, config is written, view is refreshed
      @param index number of cards is (index+1)*4
     */
    void setNumber(int index);
    ///Write settings in config file then quit the program
    void slotQuit();
    /**Full Screen Mode: this slot is called when the Full Screen icon is clicked or when it is checked in the Settings menu
     @param bool wether or not full screen is selected
     */
    void slotUpdateFullScreen( bool );
    ///After Configure Kard dialog is run and some settings changed
    void slotUpdateSettings(const QString &);
    /// After timer is changed in Configure Kard dialog, set correct time
    void slotUpdateTimer(int);
    ///After theme is changed in Configure Kard dialog
    void slotUpdateTheme();
    ///When theme is changed in Combobox
    void slotSetTheme(int );
    ///When the sound option is toggled
    void slotToggleSound();

private:

    ///set to true when the view needs to be refreshed
    bool boardChanged;
    ///Full-Screen mode action
    KAction* m_pFullScreen;
    /// Sound toggle action
    KToggleAction* m_soundAction;
    ///Define the actions for the MenuBar and Toolbar
    void setupActions();
    void setLanguage();
    void changeLanguage();
    
    QStringList m_languages;
    
    QStringList m_sortedNames;

    //settings
    Ui::generalui ui_general;
    Ui::themeui ui_theme;

protected:
    ///Call an instance of KardView to set it as main widget
    KardView *m_view;
    ///The combobox where the number of cards is chosen
    KComboBox *m_numCombo;
    ///The combobox where the theme is chosen
    KComboBox *m_themeCombo;

    QButtonGroup *groupTimer;
};

#endif // KARD_H
