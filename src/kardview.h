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
    Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _KARDVIEW_H_
#define _KARDVIEW_H_

//Qt headers
#include <qlayout.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3GridLayout>
#include <QResizeEvent>
#include <Q3PtrList>

#include <krandomsequence.h>
//Project headers
#include "kardwidget.h"

/**
 * This class serves as the main view for Kard.  It handles the code
 * and algorithms.
 *
 * @short View class
 * @author Anne-Marie Mahfouf <annma@kde.org>
 * @version 0.1
 */

class KardView : public QWidget
{
	Q_OBJECT
public:
	KardView(QWidget *parent);
	~KardView();

	///Start a game: create the GUI and shuffle the colors
	void game();
	///noc is the Number Of Cards
	int noc;
	///Hold the current theme string, "colors" or "house"
	QString theme;
	///How long the cards will be displayed (read from config or changed in Configure Kard... dialog)
	int myTime;

private:
	///Allow the clicked() signals to be connected for the cards
	void slotConnect();
	///Allow the clicked() signals to be disconnected for the cards
	void slotDisconnect();
	///When all the cards are discovered and matched
	void slotAll();
	///Instance of KardWidgets [lines][columns]
	KardWidget* k[50][50];
	///The 2 clicked KardWidgets
	KardWidget* kardW[2];
	///Contains the colors for theme "colors"
	QColor a[24];
	///Store the colors of the 2 discovered cards
	QColor color[2];
	///Contains the pixmaps for theme "theme1"
	QPixmap px[24];
	///Contains the pixmaps for theme "theme2"
	QPixmap opp[24];
	///Contains the pixmaps for theme "animals"
	QPixmap ani[24];
	///Contains the pixmaps for theme "animals"
	QPixmap food[24];
	
	QString text[24];
	
	QString mText[2];
	///New positions on the grid
	int opp_numarr[50];
	///The new way of comparing the opposites...
	int shuffle[50][50];
	///This array holds the random numbers from 0 to noc-1
	int t[24];
	///n and m are the divisors of noc (number of cards on height and number of cards on width
	int n, m;
	///Count the number of cards that are currently discovered
	int count;
	///Count the number of cards that are clicked
	int co;
	///Serial number of the QPixmap background for the 2 clicked cards
	int getRef[2];
	///Lists of all kardWidgets labels
	Q3PtrList<KardWidget>        m_listButtons;
	///Number of tries before succeeding
	int tries;
	///KDE random generator
	KRandomSequence random;

signals:
	/**
	* Use this signal to change the content of the statusbar
	*/
	void signalChangeStatusbar(const QString& text, int);

private slots:
	/**
	*Store the color of the clicked cards
	* when 2 cards are clicked, disconnect the card signals and
	*enable a timer so the kid has time to see the cards
	*/
	void slotTimer();
	/**
	*Check if the cards match or not
	*and check if all the cards are discovered
	*/
	void slotMatch();

protected:
	Q3GridLayout* grid;
	Q3GridLayout* boardLayout;

protected:
	virtual void resizeEvent( QResizeEvent * e );
	///Scan the dirs theme1, food and animals to get the pics
	void loadPixmaps();
	///Load syllables 
	void loadSyllables();
	///Load animals theme pictures

public slots:
	/**
	*Delete the board after running Configure Kard
	*and after after finishing a game
	*/
	void newBoard();


};

#endif // _KARDVIEW_H_
