/***************************************************************************
 *   Copyright (C) 2001-2006 by Anne-Marie Mahfouf                              *
 *   annma@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kardwidget.h"

#include <kglobalsettings.h>
#include <klocale.h>

#include <QToolTip>
#include <QWhatsThis>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QLabel>

//#include "kard.h"

KardWidget::KardWidget(QWidget *parent, const char *name)
  : QLabel(parent, name)
{
	//the kard widget inherits of QLabel and is a child of QWidget
	//the m_kardW wiget is a QLabel on top of the kard widget
	//the gris widget is a QLabel that will recover the m_kardW widget
	setBackgroundColor(Qt::black); //the color for the ?
	m_kardW = new QLabel(this);
	m_kardW->setGeometry(0, 0, width(), height());
	gris=new QLabel(m_kardW);
	gris->setText("?");
	gris->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//TODO change the ? size depending of the card size
	gris->setFont(QFont(KGlobalSettings::generalFont().family(), 40, QFont::Bold));
	gris->setToolTip( i18n( "Click to see what is on the back of the card" ) );
	gris->setWhatsThis( i18n( "Click on two cards to find out what is on the back of the card and try matching a pair" ) );
	QPalette palette1;
	palette1.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(Qt::lightGray));
	gris->setPalette(palette1);
	m_kardW->setWhatsThis(i18n( "Click on another kard and try matching a pair" ) );
}

KardWidget::~KardWidget()
{
	delete gris;
	delete m_kardW;
}

/**
Implement a clicked() signal for the KardWidget class
i.e. when mouse press kard then clicked() event
*/
void KardWidget::mousePressEvent(QMouseEvent * )
{
	// emit clicked() only if card not already clicked
	if (gris->isVisible()==true)
		emit clicked();
	else
		emit illegal(); //is it compulsory?
}

/**
hide the gray part of the card
*/
void KardWidget::slotHide()
{
	if (gris->isVisible()==true)
		gris->hide();
	m_kardW->show();
}

/**
the gray card should cover entirely the colored one
*/
void KardWidget::resizeEvent( QResizeEvent * )
{
	m_kardW->setGeometry(0, 0, width(), height());
	gris->setGeometry(0, 0, m_kardW->width(), m_kardW->height());
}

/**
show the gray card again
*/
void KardWidget::slotShow()
{
	gris->show();
}

/**
card is matched -> hide it
*/
void KardWidget::slotDisappear()
{
	m_kardW->hide();
	setAutoFillBackground(true);
	setBackgroundColor(Qt::white);
}

#include "kardwidget.moc"




