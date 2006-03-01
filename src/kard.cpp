/*
 * Copyright (C) 2001-2005 Anne-Marie Mahfouf <annma@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

//Qt headers
#include <QButtonGroup>
#include <QToolTip>
#include <QWhatsThis>
//KDE headers
#include <kaction.h>
#include <kapplication.h>
#include <kcombobox.h>
#include <kconfigdialog.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kaudioplayer.h>
#include <ktoolbar.h>
//Project headers
#include "kard.h"
#include "kardview.h"
// Settings
#include "kardsettings.h"

Kard::Kard() : KMainWindow( 0,"Kard" ), m_view(new KardView(this))
{
    // Create a status bar
    statusBar( )->insertItem("   ",IDS_NUMBER, 0);
    statusBar( )->insertItem("   ", 101, 0);
    statusBar( )->insertItem("   ",IDS_TIME, 0);
    statusBar( )->insertItem("   ", 103, 0);
    statusBar( )->insertItem("   ",IDS_THEME, 0);
    statusBar( )->insertItem("   ", 105, 0);
    statusBar( )->insertItem("   ",IDS_LANG, 0);
    statusBar( )->insertItem("   ", 107, 0);
    statusBar( )->insertItem("   ",IDS_TRY, 0);
    // setup our actions
    setupActions();
    setGeometry(0, 0, 600, 500);
    resize( QSize(600, 500).expandedTo(minimumSizeHint()) );
    // and a tool bar
    toolBar()->insertSeparator(-1, 1); //id=1 for separator
    toolBar()->insertCombo(i18n("4 Cards"), 2, false, SIGNAL(activated(int)), this, SLOT(setNumber(int)));
    m_numCombo = toolBar()->getCombo(2);
    m_numCombo->insertItem(i18n("8 Cards"), 1);
    m_numCombo->insertItem(i18n("12 Cards"), 2);
    m_numCombo->insertItem(i18n("16 Cards"), 3);
    m_numCombo->insertItem(i18n("20 Cards"), 4);
    m_numCombo->insertItem(i18n("24 Cards"), 5);
    m_numCombo->setToolTip( i18n( "Choose the number of cards" ) );
    m_numCombo->setWhatsThis(  i18n( "Choose the level of difficulty by setting the number of cards from 4 (very easy) to 24 (more difficult)" ) );
    
    //adding themes combobox
    QStringList themesList;
    themesList += i18n("colors");
    themesList += i18n("house");
    themesList += i18n("opposites");
    themesList +=i18n("syllables");
    themesList +=i18n("animals");
    themesList +=i18n("food");
    toolBar()->insertSeparator(-1, 1); //id=1 for separator
    toolBar()->insertCombo(themesList, 4, false, SIGNAL(activated(int )), this, SLOT(slotSetTheme(int )));
    m_themeCombo = toolBar()->getCombo(4);
    m_themeCombo->setToolTip( i18n( "Choose the theme for the cards" ) );
    m_themeCombo->setWhatsThis(  i18n( "You can choose here the theme for the backs of the cards" ) );
    // tell the KMainWindow that this is indeed the main widget
    setCentralWidget(m_view);
    
    // allow the view to change the statusbar and caption
    connect(m_view, SIGNAL(signalChangeStatusbar(const QString&, int)), this, SLOT(changeStatusbar(const QString&, int)));
    
    readConfig();
    m_view->game();
}

Kard::~Kard()
{
}

void Kard::setupActions()
{
    KStdAction::quit(this, SLOT(slotQuit()), actionCollection());
            
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    m_pFullScreen = KStdAction::fullScreen( 0, 0, actionCollection(), this);
    connect( m_pFullScreen, SIGNAL( toggled( bool )), this, SLOT( slotUpdateFullScreen( bool )));

    m_soundAction = new KToggleAction(i18n("Soun&ds"), 0, this,
                                SLOT(slotToggleSound()), actionCollection(), "sound");
        
    setupGUI();
}

void Kard::optionsPreferences()
{
    if ( KConfigDialog::showDialog( "settings" ) ) 
        return; 
    
    //KConfigDialog didn't find an instance of this dialog, so lets create it : 
    KConfigDialog* dialog = new KConfigDialog( this, "settings",  KardSettings::self() );
    dialog->setModal(true); //makes it modal even if it's not the default
    QWidget *generalSettingsDlg = new QWidget;
    ui_general.setupUi(generalSettingsDlg);
    QButtonGroup *groupTimer = new QButtonGroup;
    groupTimer->addButton(ui_general.slow, 0);
    groupTimer->addButton(ui_general.medium, 1);
    groupTimer->addButton(ui_general.quick, 2);
    connect(groupTimer, SIGNAL(buttonClicked(int)), this, SLOT(slotUpdateTimer(int)));
    ui_general.kcfg_LanguageCombobox->insertStringList(m_sortedNames, 0);
    ui_general.kcfg_LanguageCombobox->setCurrentItem(m_languages.findIndex(KardSettings::selectedLanguage()));
    ui_general.kcfg_sound->setChecked(KardSettings::sound());
    dialog->addPage(generalSettingsDlg, i18n("General"), "wizard");
    QWidget *themeSettingsDlg = new QWidget;
    ui_theme.setupUi(themeSettingsDlg);
    dialog->addPage(themeSettingsDlg, i18n("Theme"), "colors");
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotUpdateSettings(const QString &)));
    dialog->show();
}

void Kard::changeStatusbar(const QString& text, int id)
{
    // display the text on the statusbar
    statusBar()->changeItem(text, id);
}

void Kard::changeCaption(const QString& text)
{
	// display the text on the caption
	setCaption(text);
}

void Kard::readConfig()
{
    boardChanged = false;
    //read number of cards from config, set default to 16
    m_view->noc = (KardSettings::numCards()+1)*4 ;
    m_numCombo->setCurrentItem(KardSettings::numCards());
    changeStatusbar(i18n("Number of cards: %1").arg(m_view->noc), IDS_NUMBER);
    //read theme from kconfigskeleton, "colors" is default
    slotUpdateTheme();
    //read timer from config, set default to 1 second
    slotUpdateTimer(KardSettings::time());
    setLanguage();
    changeLanguage();
    m_soundAction->setChecked(KardSettings::sound());
} 

void Kard::slotUpdateSettings(const QString &)
{
    //TODO slotUpdateTimer();
    slotUpdateTheme();
    if (m_view->noc != (KardSettings::numCards()+1)*4) {
        m_view->noc = (KardSettings::numCards()+1)*4;
        m_numCombo->setCurrentItem(KardSettings::numCards());
        boardChanged = true;
    }
    changeLanguage();
    if (boardChanged)
        m_view->newBoard();	
    m_soundAction->setChecked(KardSettings::sound());
}

void Kard::slotSetTheme(int id)
{
    KardSettings::setTheme(id);
    KardSettings::writeConfig();
    slotUpdateTheme();
    changeLanguage();
    if (boardChanged)
		m_view->newBoard();	
}

void Kard::slotUpdateTheme()
{
    m_themeCombo->setCurrentItem(KardSettings::theme());
    switch (KardSettings::theme()) {
        case KardSettings::EnumTheme::colors:
            if (m_view->theme=="house" || m_view->theme=="opposites"|| m_view->theme=="syllables" || m_view->theme=="animals") 
                boardChanged = true;
            m_view->theme = "colors";
            break;
        case KardSettings::EnumTheme::house:
            if (m_view->theme=="colors" || m_view->theme=="opposites" || m_view->theme=="syllables" || m_view->theme=="animals" || m_view->theme=="food")
                boardChanged = true;
            m_view->theme = "house";
            break;
        case KardSettings::EnumTheme::opposites:
            if (m_view->theme=="house" || m_view->theme=="colors"|| m_view->theme=="syllables" || m_view->theme=="animals" || m_view->theme=="food") 
                boardChanged = true;
            m_view->theme = "opposites";
            break;
        case KardSettings::EnumTheme::syllables:
            if (m_view->theme=="house" || m_view->theme=="colors" || m_view->theme=="opposites" || m_view->theme=="animals" || m_view->theme=="food") 
                boardChanged = true;
            m_view->theme = "syllables";
            break;
        case KardSettings::EnumTheme::animals:
            if (m_view->theme=="house" || m_view->theme=="colors" || m_view->theme=="opposites" || m_view->theme=="syllables" || m_view->theme=="food") 
                boardChanged = true;
            m_view->theme = "animals";
        break;
        case KardSettings::EnumTheme::food:
            if (m_view->theme=="house" || m_view->theme=="colors" || m_view->theme=="opposites" || m_view->theme=="syllables" || m_view->theme=="animals") 
                boardChanged = true;
            m_view->theme = "food";
            break;
    }
    
    changeStatusbar(i18n("Theme: %1").arg(m_view->theme), IDS_THEME);
}

void Kard::slotUpdateTimer(int id)
{
    //read timer from config, set default to 1 second
    switch (id)  {
        case 0:
            m_view->myTime = 2000;
            break;
        case 1:
            m_view->myTime = 1000;
            break;
        case 2:
            m_view->myTime = 500;
            break;
    }
    double m_time = double(m_view->myTime)/double(1000);
    
    if (m_time<1)
        changeStatusbar(i18n("fraction of whole second","Timer: %1 seconds").arg(m_time), IDS_TIME);
    else
        changeStatusbar(i18n("Timer: 1 second","Timer: %n seconds", (int)m_time), IDS_TIME);
   KardSettings::setTime(id);
   KardSettings::writeConfig(); 
}

void Kard::setNumber(int index)
{
    m_view->noc=(index+1)*4;
    saveSettings();
    changeStatusbar(i18n("Number of cards: %1").arg(m_view->noc), IDS_NUMBER);
    m_view->newBoard();
}

void Kard::slotQuit()
{
    kapp->quit();
}

void Kard::saveSettings()
{
    KardSettings::setNumCards(m_view->noc/4 -1);
    KardSettings::writeConfig(); 
}

void Kard::slotUpdateFullScreen( bool set )
{
    if( set ){
        showFullScreen();
        menuBar()->hide();
    } else {
        showNormal();
        menuBar()->show();
    }
}

void Kard::setLanguage()
{
    m_languages.clear();
    m_sortedNames.clear();
    //the program scans in khangman/data/ to see what languages data is found
    QStringList mdirs = KGlobal::dirs()->findDirs("data", "kard/data/");
    if (mdirs.isEmpty()) 
        return;
    for (QStringList::Iterator it =mdirs.begin(); it !=mdirs.end(); ++it ) {
        QDir dir(*it);
        m_languages += dir.entryList(QDir::Dirs, QDir::Name);
        m_languages.remove(m_languages.find("."));
        m_languages.remove(m_languages.find(".."));
    }
    m_languages.sort();
    if (m_languages.isEmpty()) 
        return;
    //find duplicated entries in KDEDIR and KDEHOME
    for (int i=0;  i<m_languages.count(); i++) {
        if (m_languages.count(m_languages[i])>1)
            m_languages.remove(m_languages[i]);
    }
            //write the present languages in config so they cannot be downloaded
    KConfig *config=kapp->sessionConfig();
    config->setGroup("KNewStuffStatus");
    for (int i=0;  i<m_languages.count(); i++) {
        QString tmp = m_languages[i];
        //if (!config->readEntry(tmp, QDate::currentDate().toString()));
            config->writeEntry(tmp, QDate::currentDate().toString());
    }
    //we look in $KDEDIR/share/locale/all_languages from /kdelibs/kdecore/all_languages
    //to find the name of the country
    //corresponding to the code and the language the user set
    KConfig entry(locate("locale", "all_languages"));
    const QStringList::ConstIterator itEnd = m_languages.end();
    for (QStringList::Iterator it = m_languages.begin(); it != m_languages.end(); ++it) {
        entry.setGroup(*it);
        m_sortedNames.append(entry.readEntry("Name"));
    }
}

void Kard::changeLanguage()
{
    boardChanged = true;
    kDebug() <<"change lang " << m_languages[KardSettings::languageCombobox()] << endl;
    KardSettings::setSelectedLanguage(m_languages[KardSettings::languageCombobox()]);
    KardSettings::writeConfig();
    if (m_view->theme=="syllables")
        changeStatusbar(i18n("Language: %1").arg(m_sortedNames[KardSettings::languageCombobox()]), IDS_LANG);
    else
        changeStatusbar("", IDS_LANG);
}

void Kard::slotToggleSound()
{
    KardSettings::setSound(!KardSettings::sound());
    KardSettings::writeConfig();
}

void Kard::playSound(const QString &soundname)
{
    if (KardSettings::sound())
    	KAudioPlayer::play(locate("data", QString("kard/sounds/game/")+soundname));
}

#include "kard.moc"


