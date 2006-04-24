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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

//Qt headers
#include <QTimer>
#include <QLabel>
//Added by qt3to4:
#include <QTextStream>
#include <QResizeEvent>
//KDE headers
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
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
  a[0]=a[1]=Qt::black;  //array for stocking the pairs
  a[2]=a[3]=Qt::red;    //of colours
  a[4]=a[5]=Qt::yellow;
  a[6]=a[7]=Qt::green;
  a[8]=a[9]=Qt::blue;
  a[10]=a[11]=Qt::darkRed;//QColor(213, 133, 192);
  a[12]=a[13]=Qt::cyan;
  a[14]=a[15]=Qt::magenta;
  a[16]=a[17]=Qt::darkMagenta;
  a[18]=a[19]=Qt::darkCyan;
  a[20]=a[21]=Qt::darkGreen;
  a[22]=a[23]=Qt::darkGray;

  //boardLayout = new QGridLayout( this, 1, 1, 0, 0, "boardLayout");
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

	if (theme == "house" || theme =="animals" || theme =="food" || theme == "opposites")
		loadPixmaps();
	if (theme == "syllables")
	  	loadSyllables();
	m_listButtons.clear();

	switch ( noc )
	{
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
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<m; j++)
		{
			k[i][j]=new KardWidget(this, "wid");
			grid->addWidget( k[i][j], i, j );
			m_listButtons.append(k[i][j]);
			k[i][j]->m_kardW->setAutoFillBackground(true);
			k[i][j]->m_gray->setAutoFillBackground(true);
			k[i][j]->show();
		}
	}
	setLayout(grid);
	setBackgroundColor(Qt::darkGray);//will be the borders color
	
	//choose the cards colors in random
	srand((unsigned int)time((time_t *)NULL));
	int p= rand()%24;       //p will be the first colour
	if (p%2==1) p=(p+1)%24; //p must be even

        //shuffle the cards
	for (int i=0; i<noc; i++)
	{
		t[i]=rand()%(noc);
		for (int j=0; j<i; j++)
		{
			if (t[i]==t[j])
				i=i-1;
		}
	}
	int ni=0;
	QPalette pal;
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<m; j++)
		{
			shuffle[i][j]=(t[ni]+p)%24;

			switch (KardSettings::theme()) {
				case KardSettings::EnumTheme::colors:
					pal.setBrush(QPalette::Window, ( QColor(a[shuffle[i][j]]) ));// QPalette( QColor(a[shuffle[i][j]]) ) );
					k[i][j]->m_kardW->setPalette(pal);
					break;
				case KardSettings::EnumTheme::house:
					k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
					k[i][j]->m_kardW->setScaledContents(true);
					break;
				case KardSettings::EnumTheme::opposites:
					k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
					k[i][j]->m_kardW->setScaledContents(true);
					break;
				case KardSettings::EnumTheme::syllables:
					k[i][j]->m_kardW->setText(text[shuffle[i][j]]);
					k[i][j]->m_kardW->setFont(QFont(KGlobalSettings::generalFont().family(), 44, QFont::Bold));
					k[i][j]->m_kardW->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
					k[i][j]->m_kardW->setScaledContents(true);
					break;
				case KardSettings::EnumTheme::animals:
					k[i][j]->m_kardW->setPixmap(px[shuffle[i][j]]);
					k[i][j]->m_kardW->setScaledContents(true);
				break;
			 	 case KardSettings::EnumTheme::food:
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
	if (theme == "colors")
		color[co] = kardW[co]->m_kardW->palette().color(QPalette::Active, QPalette::Window);
	if (theme == "syllables") 
		mText[co] = kardW[co]->m_kardW->text();

	// or store the background picture serial number of the clicked card
	if (theme == "house" || theme == "animals" || theme == "opposites" || theme == "food")
	{
		if( kardW[co])
		{
			int index = m_listButtons.findRef( kardW[co] );//get the card's position in the list
			int test = -1;
			for (int i=0; i<n; i++)
			{
				for (int j=0; j<m; j++)
				{
					test++;
					if (test == index)//get the i,j pair associated with the card's position
					{
					  	if (theme == "house" || theme == "animals" || theme == "food")
							getRef[co]=px[shuffle[i][j]].serialNumber();//store the serial number
						if (theme == "opposites")
							getRef[co] = opp_numarr[shuffle[i][j]]; 
						break;
					}
				}
			}
		}
	}

	co++;
	if (co==2) //when 2 cards are clicked
	{
		slotDisconnect(); //so that the user cannot click on another card
		QTimer *timer = new QTimer(this); //leave the cards on for 0.5, 1 or 2 seconds
		connect( timer, SIGNAL(timeout()),
		this, SLOT(slotMatch()) );
		timer->start( myTime );
	}
 }

void KardView::slotMatch()
{
	tries++;
	
	emit signalChangeStatusbar(i18n("Tries: %1").arg(tries), IDS_TRY);
	
	co=0;

	if (((theme == "colors") && (color[0]==color[1])) || ((theme == "syllables") && (mText[0]==mText[1])) ) //if same color or same picture
	{
		kardW[0]->slotDisappear(); //hide the 2 same cards
		kardW[1]->slotDisappear();
		count=count+2; //count the cards that are matched by pairs
		if (count==noc)
		{
			slotAll();
			return;
		}
	}
	else if ((theme == "house" || theme == "opposites" ||  theme == "animals" ||  theme == "food") && (getRef[0]==getRef[1]))
	{
		kardW[0]->slotDisappear(); //hide the 2 same cards
		kardW[1]->slotDisappear();
		count=count+2; //count the cards that are matched by pairs
		if (count==noc)
		{
			slotAll();
			return;
		}
	}
	else
	{
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
	kDebug() <<"Tries: " << tries << endl;
	switch(KMessageBox::questionYesNo( this, mString,i18n("Game is Finished") ))
	{
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

	for (int i = 0 ; i < n ; i++)
	{
		for (int j = 0 ; j < m ; j++)
		{
			delete k[i][j];
			k[i][j] = 0;
		}
	}

	n = 0;  m = 0;
	game();
}

void KardView::slotConnect()
{
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<m; j++)
		{
				QObject::connect (k[i][j], SIGNAL(clicked()),k[i][j], SLOT(slotHide()));
				QObject::connect (k[i][j], SIGNAL(clicked()), this, SLOT(slotTimer()));
		}
	}
}

void KardView::slotDisconnect()
{
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<m; j++)
		{
				QObject::disconnect (k[i][j], SIGNAL(clicked()),k[i][j], SLOT(slotHide()));
				QObject::disconnect (k[i][j], SIGNAL(clicked()), this, SLOT(slotTimer()));
		}
	}
}

void KardView::resizeEvent( QResizeEvent * )
{
	for (int i=0; i<n; i++)
	{
    	      for (int j=0; j<m; j++)
		{
			k[i][j]->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, k[i][j]->sizePolicy().hasHeightForWidth() ) );
		}
	}
}

//scan the correct picture dir and get all the pics instead of loading with each filename
void KardView::loadPixmaps()
{
    QStringList picsList;
    KStandardDirs *dirs = KGlobal::dirs();
    QString currentDir; 
    if (theme == "house")
        currentDir= "theme1";
    else if (theme == "opposites")
        currentDir = "theme2";
    else currentDir = theme;
    QStringList mfiles = dirs->findAllResources("data", "kard/pics/"+currentDir+"/*.png");
    int i=0;
    if (!mfiles.isEmpty()) {
        for (QStringList::Iterator it = mfiles.begin(); it != mfiles.end(); ++it ) {
            QFile f( *it);
            QString mString = f.fileName();
            picsList+=mString;
            kDebug() << mString  <<endl;
            px[i].load(locate("data",mString));
            if (currentDir == "theme2") {
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
	myFile.setFileName(locate("data", myString));
	QFile openFileStream(myFile.fileName());
	openFileStream.open(QIODevice::ReadOnly);
	QTextStream readFileStr(&openFileStream);
	//allData contains all the words from the file
	QStringList allData = readFileStr.readAll().split("\n");
	openFileStream.close();
	//get text[0] to text[23] from random from the file 
	//with 2 consecutives the same
	int i=0;
	while (i<24)
	{
		int rand = random.getLong(allData.count());
		text[i]=text[i+1] = allData[rand];
		allData.removeAt(rand);//remove the chosen syllable from the list
		i=i+2;
	}
}

//this speeds up the compile process
#include "kardview.moc"







