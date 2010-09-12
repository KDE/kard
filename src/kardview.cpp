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
#include <QTimer>
#include <QLabel>
#include <QTextStream>
#include <QResizeEvent>
//KDE headers
#include <kapplication.h>
#include <kdebug.h>
#include <KLocale>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>
//C++ headers
#include <time.h>
#include <stdlib.h>
//Project headers
#include "kardview.h"
#include "kardsettings.h"
#include "kard.h"

KardView::KardView(QWidget *parent)
    : QWidget(parent)
{
    ///Define the colors
    a << Qt::black << Qt::black;  //array for stocking the pairs
    a << Qt::red << Qt::red;    //of colours
    a << Qt::yellow << Qt::yellow;
    a << Qt::green << Qt::green;
    a << Qt::blue << Qt::blue;
    a << Qt::darkRed << Qt::darkRed;//QColor(213, 133, 192);
    a << Qt::cyan << Qt::cyan;
    a << Qt::magenta << Qt::magenta;
    a << Qt::darkMagenta << Qt::darkMagenta;
    a << Qt::darkCyan << Qt::darkCyan;
    a << Qt::darkGreen << Qt::darkGreen;
    a << Qt::darkGray << Qt::darkGray;
 }

KardView::~KardView()
{
}

void KardView::game()
{
    co = 0;
    count = 0;
    tries = 0;
    emit signalChangeStatusbar(i18n("Tries: %1", tries), IDS_TRY);

    if (theme == "syllables")
	loadSyllables();
    else if (theme != "colors") {
	loadPixmaps();
    }
    m_listButtons.clear();

    switch ( noc )  {
	case 4:
		n=2;
		m=2;
	break;

	case 8:
		n=2;
		m=4;
	break;

	case 12:
		n=3;
		m=4;
	break;

	default:
	case 16:
		n=4;
		m=4;
	break;

	case 20:
		n=5;
		m=4;
	break;

	case 24:
		n=4;
		m=6;
	break;
    }
    setAutoFillBackground(true);
    //create the GUI
    grid = new QGridLayout;
    grid->setSpacing(3);
    grid->setMargin(2);
    //create the cards
    for (int i=0; i<n; i++)  {
	for (int j=0; j<m; j++)  {
	    k[i][j]=new KardWidget(this, "wid");
	    grid->addWidget( k[i][j], i, j );
	    m_listButtons.append(k[i][j]);
	    k[i][j]->m_kardW->setAutoFillBackground(true);
	    k[i][j]->m_gray->setAutoFillBackground(true);
	    k[i][j]->show();
	}
    }
    setLayout(grid);
    /*QPalette pal;
    pal.setBrush( QPalette::Window, Qt::darkGray);
    setPalette(pal);*/

    //choose the cards colors in random
    srand((unsigned int)time((time_t *)NULL));
    int p= rand()%24;       //p will be the first colour
    if (p%2==1) {
	p=(p+1)%24; //p must be even
    }
    //shuffle the cards
    for (int i=0; i<noc; i++)  {
	t[i]=rand()%(noc);
	for (int j=0; j<i; j++)  {
	    if (t[i]==t[j])
		    i=i-1;
	}
    }
    int ni=0;
    for (int i=0; i<n; i++)  {
	for (int j=0; j<m; j++)  {
	    shuffle[i][j]=(t[ni]+p)%24;
	    QPalette pal;
	    switch (KardSettings::kardTheme()) {
		case KardSettings::EnumKardTheme::colors:
		    pal.setColor(QPalette::Active, QPalette::Window, QColor(a[shuffle[i][j]]));
		    k[i][j]->m_kardW->setPalette(pal);
		    break;
		case KardSettings::EnumKardTheme::house:
		    k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
		    k[i][j]->m_kardW->setScaledContents(true);
		    break;
		case KardSettings::EnumKardTheme::opposites:
		    k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
		    k[i][j]->m_kardW->setScaledContents(true);
		    break;
		case KardSettings::EnumKardTheme::syllables:
		    k[i][j]->m_kardW->setText(text[shuffle[i][j]]);
		    k[i][j]->m_kardW->setFont(QFont(KGlobalSettings::generalFont().family(), 44, QFont::Bold));
		    k[i][j]->m_kardW->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		    k[i][j]->m_kardW->setScaledContents(true);
		    break;
		case KardSettings::EnumKardTheme::animals:
		    k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
		    k[i][j]->m_kardW->setScaledContents(true);
		    break;
		case KardSettings::EnumKardTheme::food:
		    k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
		    k[i][j]->m_kardW->setScaledContents(true);
		    break;
	    }
	    ni++;
	}
    }
    //enable the connections
    slotConnect();
    updateGeometry();//this is very important
}

void KardView::slotTimer()
{
    Kard::playSound("kard_click.ogg");

    kardW[co] = (KardWidget* ) sender();

    // store the background color of the clicked card
    if (theme == "colors")  {
	color[co] = kardW[co]->m_kardW->palette().color(QPalette::Active, QPalette::Window);
    }
    // store the text on the card
    else if (theme == "syllables") {
	mText[co] = kardW[co]->m_kardW->text();
    }
    // or store the background picture serial number of the clicked card
    else {
	if( kardW[co])  {
	    int index = m_listButtons.indexOf( kardW[co] );//get the card's position in the list
	    int test = -1;
	    for (int i=0; i<n; i++)  {
		for (int j=0; j<m; j++)  {
		    test++;
		    if (test == index)  {//get the i,j pair associated with the card's position
			if (theme == "house" || theme == "animals" || theme == "food")  {
			    getRef[co]=px[shuffle[i][j]].serialNumber();//store the serial number
			}
			if (theme == "opposites")  {
			    getRef[co] = opp_numarr[shuffle[i][j]]; 
			}
			break;
		    }
		}
	    }
	}
    }
    kDebug() << "--- co " << co << endl;
    co++;
    kDebug() << "--- co " << co << endl;
    if (co==2) {//when 2 cards are clicked
	slotDisconnect(); //so that the user cannot click on another card
	QTimer::singleShot(myTime, this, SLOT(slotMatch()));
    }
 }

void KardView::slotMatch()
{
    kDebug() << "in slot match" << endl;
    tries++;
    
    emit signalChangeStatusbar(i18n("Tries: %1", tries), IDS_TRY);
    
    co=0;

    if (((theme == "colors") && (color[0]==color[1])) || ((theme == "syllables") && (mText[0]==mText[1])) )  {//if same color or same picture
	kardW[0]->slotDisappear(); //hide the 2 same cards
	kardW[1]->slotDisappear();
	count=count+2; //count the cards that are matched by pairs
	if (count==noc)  {
	    slotAll();
	    return;
	}
    }
    else if ((theme == "house" || theme == "opposites" ||  theme == "animals" ||  theme == "food") && (getRef[0]==getRef[1]))  {
	kardW[0]->slotDisappear(); //hide the 2 same cards
	kardW[1]->slotDisappear();
	count=count+2; //count the cards that are matched by pairs
	if (count==noc)  {
	    slotAll();
	    return;
	}
    }
    else {
	kardW[0]->slotShow();
	kardW[1]->slotShow();
    }

    slotConnect();
}

void KardView::slotAll()
{
    Kard::playSound("kard_gameover.wav");
    int bestNum = n*m/2;
    QString mString = i18n("Congratulations!\n"
    "You finished the game in %1 tries.\n"
    "The best you could have done is %2 tries.\n"
    "Do you want to play again?",  tries, bestNum);
    switch(KMessageBox::questionYesNo( this, mString,i18n("Game is Finished") ))  {
    case 3: // yes
	//put an icon for yes (animal nodding) and no
	newBoard();
	break;
    case 4:	// no
	kapp->quit();
	break;
    }
}

void KardView::newBoard()
{
    delete grid;
    grid = 0;

    for (int i = 0 ; i < n ; i++)  {
	for (int j = 0 ; j < m ; j++)  {
	    delete k[i][j];
	    k[i][j] = 0;
	}
    }

    n = 0;  m = 0;
    game();
}

void KardView::slotConnect()
{
    kDebug() << "in slot connect" << endl;
    for (int i=0; i<n; i++)  {
	for (int j=0; j<m; j++)  {
	    QObject::connect (k[i][j], SIGNAL(clicked()),k[i][j], SLOT(slotHide()));
	    QObject::connect (k[i][j], SIGNAL(clicked()), this, SLOT(slotTimer()));
	}
    }
}

void KardView::slotDisconnect()
{
    kDebug() << "in slot disconnect" << endl;
    for (int i=0; i<n; i++)  {
	for (int j=0; j<m; j++)  {
	    QObject::disconnect (k[i][j], SIGNAL(clicked()),k[i][j], SLOT(slotHide()));
	    QObject::disconnect (k[i][j], SIGNAL(clicked()), this, SLOT(slotTimer()));
	}
    }
}

void KardView::resizeEvent( QResizeEvent * )
{
    for (int i=0; i<n; i++)    {
	for (int j=0; j<m; j++)  {
		k[i][j]->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	}
    }
}

//scan the correct picture dir and get all the pics instead of loading with each filename
void KardView::loadPixmaps()
{
    QStringList picsList;
    KStandardDirs *dirs = KGlobal::dirs();
    QStringList themesFiles = dirs->findAllResources("data", "kard/pics/"+theme+"/*.png");
    themesFiles.sort();
    int i=0;
    if (!themesFiles.isEmpty()) {
        for (QStringList::Iterator it = themesFiles.begin(); it != themesFiles.end(); ++it ) {
            QFile f( *it);
            QString picture = f.fileName();
            picsList+=picture;
            px[i].load(KStandardDirs::locate("data", picture));
            if (theme == "opposites") {
                if (i%2 ==0) 
                    opp_numarr[i] = i;
                else 
                    opp_numarr[i] = i-1;
                i++;
            }
            else {
                px[i+1] = px[i];
                i=i+2;
            }
        }
        //TODO else tell no files had been found
    }
}

void KardView::loadSyllables()
{
    kDebug() << "Language: " << KardSettings::selectedLanguage() << endl;
    QString myString=QString("kard/data/%1/syllables.txt").arg(KardSettings::selectedLanguage());
    QFile myFile;
    myFile.setFileName(KStandardDirs::locate("data", myString));
    QFile openFileStream(myFile.fileName());
    openFileStream.open(QIODevice::ReadOnly);
    QTextStream readFileStr(&openFileStream);
    //allData contains all the words from the file
    QStringList allData = readFileStr.readAll().split("\n");
    openFileStream.close();
    //get text[0] to text[23] from random from the file 
    //with 2 consecutives the same
    int i=0;
    while (i<24)  {
	int rand = random.getLong(allData.count());
	text[i]=text[i+1] = allData[rand];
	allData.removeAt(rand);//remove the chosen syllable from the list
	i=i+2;
    }
}

//this speeds up the compile process
#include "kardview.moc"







