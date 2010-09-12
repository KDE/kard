/*
 *  Copyright  2001-2008 Anne-Marie Mahfouf <annma@kde.org>

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
#include <kactioncollection.h>
#include <kapplication.h>
#include <kcombobox.h>
#include <kconfigdialog.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kselectaction.h>
#include <kstandardaction.h>
#include <kstatusbar.h>
#include <ktogglefullscreenaction.h>
#include <ktoolbar.h>
#include <kurl.h>
#include <Phonon/MediaObject>

//Project headers
#include "kard.h"
#include "kardview.h"
#include "kardtheme.h"
// Settings
#include "kardsettings.h"

#include "kgametheme/kgamethemeselector.h"

Phonon::MediaObject *Kard::mplayer = 0L;

Kard::Kard() : KXmlGuiWindow(), m_view(new KardView(this))
{
    Kard::mplayer = Phonon::createPlayer(Phonon::GameCategory);
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
    // tell the KXmlGuiWindow that this is indeed the main widget
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
    KStandardAction::quit(this, SLOT(slotQuit()), actionCollection());
            
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    m_pFullScreen = KStandardAction::fullScreen( 0, 0, this, actionCollection());
    actionCollection()->addAction("full_screen", m_pFullScreen );
    connect( m_pFullScreen, SIGNAL( toggled( bool )), this, SLOT( slotUpdateFullScreen( bool )));

    m_soundAction = new KToggleAction(i18n("Soun&ds"), actionCollection());
    actionCollection()->addAction("sound", m_soundAction );
    connect(m_soundAction , SIGNAL(triggered()), this, SLOT(slotToggleSound()));

    //adding choice of number of cards action
    m_numAction = new KSelectAction(i18n("View"), actionCollection());
    actionCollection()->addAction("num", m_numAction );
    connect(m_numAction , SIGNAL(triggered(int)), this, SLOT(setNumber(int)));
    QStringList nums;
    nums+=i18np("%1 Card", "%1 Cards", 4);
    nums+=i18np("%1 Card", "%1 Cards", 8);
    nums+=i18np("%1 Card", "%1 Cards", 12);
    nums+=i18np("%1 Card", "%1 Cards", 16);
    nums+=i18np("%1 Card", "%1 Cards", 20);
    nums+=i18np("%1 Card", "%1 Cards", 24);
    m_numAction->setItems(nums);
    m_numAction->setToolTip( i18n( "Choose the number of cards" ) );
    m_numAction->setWhatsThis(  i18n( "Choose the level of difficulty by setting the number of cards from 4 (very easy) to 24 (more difficult)" ) );    

    //adding theme action
    m_themeAction = new KSelectAction(i18n("Themes"), actionCollection());
    actionCollection()->addAction("themes", m_themeAction );
    connect(m_themeAction , SIGNAL(triggered(int)), this, SLOT(slotSetTheme(int )));
    m_themeAction->setItems(KardThemeFactory::instance()->themeUiList());
    m_themeAction->setToolTip( i18n( "Choose the theme for the cards" ) );
    m_themeAction->setWhatsThis(  i18n( "You can choose here the theme for the backs of the cards" ) );
    KStandardAction::fullScreen(this, SLOT(toggleFullScreen()), this, actionCollection());
    setupGUI();
}

void Kard::toggleFullScreen()
{
    KToggleFullScreenAction::setFullScreen( this, actionCollection()->action("fullscreen")->isChecked());
}

void Kard::optionsPreferences()
{
    if ( KConfigDialog::showDialog( "settings" ) ) {
        return; 
    }
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
    ui_general.kcfg_LanguageCombobox->insertItems(0, m_sortedNames);
    ui_general.kcfg_LanguageCombobox->setCurrentIndex(m_languages.indexOf(KardSettings::selectedLanguage()));
    dialog->addPage(generalSettingsDlg, i18n("General"), "wizard");
    QWidget *themeSettingsDlg = new QWidget;
    ui_theme.setupUi(themeSettingsDlg);
    dialog->addPage(themeSettingsDlg, i18n("Theme"), "colors");
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotUpdateSettings(const QString &)));
    dialog->addPage(new KGameThemeSelector(dialog, KardSettings::self(), KGameThemeSelector::NewStuffDisableDownload),
                        i18n("Theme"), "games-config-theme");
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
    m_boardChanged = false;
    //read number of cards from config, set default to 16
    m_view->noc = (KardSettings::numCards()+1)*4 ;
    m_numAction->setCurrentItem(KardSettings::numCards());
    changeStatusbar(i18n("Number of cards: %1", m_view->noc), IDS_NUMBER);
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
    slotUpdateTheme();
    if (m_view->noc != (KardSettings::numCards()+1)*4) {
        m_view->noc = (KardSettings::numCards()+1)*4;
        m_numAction->setCurrentItem(KardSettings::numCards());
        m_boardChanged = true;
    }
    changeLanguage();
    if (m_boardChanged) {
        m_view->newBoard();
    }
}

void Kard::slotSetTheme(int id)
{
    KardSettings::setKardTheme(id);
    KardSettings::self()->writeConfig();
    m_view->theme =KardThemeFactory::instance()->buildTheme(id)->name();
    m_themeAction->setCurrentItem(KardSettings::kardTheme());
    changeStatusbar(i18n("Theme: %1", KardThemeFactory::instance()->buildTheme(id)->uiName()), IDS_THEME);
    changeLanguage();
    if (m_boardChanged) {
	m_view->newBoard();
    }
}

void Kard::slotUpdateTheme()
{
    m_view->theme = KardThemeFactory::instance()->themeList()[KardSettings::kardTheme()];
    m_themeAction->setCurrentItem(KardSettings::kardTheme());
    changeStatusbar(i18n("Theme: %1", KardThemeFactory::instance()->buildTheme(KardSettings::kardTheme())->uiName()), IDS_THEME);
    m_boardChanged = true;
}

void Kard::slotUpdateTimer(int id)
{
    //read timer from config, set default to 1 second
    switch (id)  {
        case 0:
            m_view->myTime = 2000; // 2 seconds
            break;
        case 1:
            m_view->myTime = 1000; // 1 second
            break;
        case 2:
            m_view->myTime = 500; // half a second
            break;
    }
    
    double m_time = double(m_view->myTime)/double(1000);
    
    if (m_time<1)  {
        changeStatusbar(i18nc("fraction of whole second","Timer: %1 second", m_time), IDS_TIME);
    } else  {
        changeStatusbar(i18np("Timer: 1 second","Timer: %1 seconds", (int)m_time), IDS_TIME);
    }
    KardSettings::setTime(id);
    KardSettings::self()->writeConfig(); 
}

void Kard::setNumber(int index)
{
    m_view->noc=(index+1)*4;
    saveSettings();
    changeStatusbar(i18n("Number of cards: %1", m_view->noc), IDS_NUMBER);
    m_view->newBoard();
}

void Kard::slotQuit()
{
    kapp->quit();
}

void Kard::saveSettings()
{
    KardSettings::setNumCards(m_view->noc/4 -1);
    KardSettings::self()->writeConfig(); 
}

void Kard::slotUpdateFullScreen( bool set )
{
    if( set ){
        setWindowState( windowState() | Qt::WindowFullScreen ); // set
        menuBar()->hide();
    } else {
        setWindowState( windowState() & ~Qt::WindowFullScreen ); // reset
        menuBar()->show();
    }
}

void Kard::setLanguage()
{
    m_languages.clear();
    m_sortedNames.clear();
    //the program scans in khangman/data/ to see what languages data is found
    QStringList mdirs = KGlobal::dirs()->findDirs("data", "kard/data/");
    
    if (mdirs.isEmpty()) { 
        return;
    }
    
    for (QStringList::Iterator it =mdirs.begin(); it !=mdirs.end(); ++it ) {
        QDir dir(*it);
        m_languages += dir.entryList(QDir::Dirs, QDir::Name);
        m_languages.removeAll(".");
        m_languages.removeAll("..");
    }
    
    m_languages.sort();
    
    if (m_languages.isEmpty()) {
        return;
    }
    //find duplicated entries in KDEDIR and KDEHOME
    //TODO check that code, seems fishy
    for (int i=0;  i<m_languages.count(); i++) {
        if (m_languages.count(m_languages[i])>1)
            m_languages.removeAt(i);
    }
    //write the present languages in config so they cannot be downloaded
     KConfigGroup config(KGlobal::config(), "KNewStuffStatus");
    for (int i=0;  i<m_languages.count(); i++) {
        QString tmp = m_languages[i];
        //if (!config->readEntry(tmp, QDate::currentDate().toString()));
            config.writeEntry(tmp, QDate::currentDate().toString());
    }
    //we look in $KDEDIR/share/locale/all_languages from /kdelibs/kdecore/all_languages
    //to find the name of the country
    //corresponding to the code and the language the user set
    KConfig entry(KStandardDirs::locate("locale", "all_languages"));
    for (QStringList::Iterator it = m_languages.begin(); it != m_languages.end(); ++it) {
        KConfigGroup group = entry.group(*it);
        m_sortedNames.append(group.readEntry("Name"));
    }
}

void Kard::changeLanguage()
{
    m_boardChanged = true;
    kDebug() <<"change lang " << m_languages[KardSettings::languageCombobox()] << endl;
    KardSettings::setSelectedLanguage(m_languages[KardSettings::languageCombobox()]);
    KardSettings::self()->writeConfig();
    
    if (m_view->theme=="syllables") {
        changeStatusbar(i18n("Language: %1", m_sortedNames[KardSettings::languageCombobox()]), IDS_LANG);
    } else {
        changeStatusbar("", IDS_LANG);
    }
}

void Kard::slotToggleSound()
{
    KardSettings::setSound(!KardSettings::sound());
    KardSettings::self()->writeConfig();
}

void Kard::playSound(const QString &soundname)
{
    if (KardSettings::sound()) {
	mplayer->setCurrentSource(KUrl(KStandardDirs::locate("data", QString("kard/sounds/game/")+soundname)));
        mplayer->play();
    }
}

#include "kard.moc"


