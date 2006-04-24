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

KardWidget::KardWidget(QWidget *parent, const char *)
  : QLabel(parent)
{
	//the kard widget inherits of QLabel and is a child of QWidget
	//the m_kardW wiget is a QLabel on top of the kard widget
	//the m_gray widget is a QLabel that will recover the m_kardW widget i.e. m_gray = back of the card
	//setBackgroundColor(Qt::black); //the color for the ?
	m_kardW = new QLabel(this);
	m_kardW->setGeometry(0, 0, width(), height());
	m_gray=new QLabel(m_kardW);
	m_gray->setText("?");
	m_gray->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//TODO change the ? size depending of the card size
	m_gray->setFont(QFont(KGlobalSettings::generalFont().family(), 40, QFont::Bold));
	m_gray->setToolTip( i18n( "Click to see what is on the back of the card" ) );
	m_gray->setWhatsThis( i18n( "Click on two cards to find out what is on the back of the card and try matching a pair" ) );
	QPalette palette1;
	//palette1.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(Qt::lightGray));
	palette1.setBrush( QPalette::WindowText, Qt::black);
	palette1.setBrush( QPalette::Window, Qt::lightGray);
	m_gray->setPalette(palette1);
	m_kardW->setWhatsThis(i18n( "Click on another kard and try matching a pair" ) );
}

KardWidget::~KardWidget()
{
	delete m_gray;
	delete m_kardW;
}

void KardWidget::mousePressEvent(QMouseEvent * )
{
	// emit clicked() only if card not already clicked
	if (m_gray->isVisible()==true)
		emit clicked();
	else
		emit illegal(); //is it compulsory?
}

void KardWidget::resizeEvent( QResizeEvent * )
{
	m_kardW->setGeometry(0, 0, width(), height());
	m_gray->setGeometry(0, 0, m_kardW->width(), m_kardW->height());
}


void KardWidget::slotHide()
{
	if (m_gray->isVisible()==true)
		m_gray->hide();
	m_kardW->show();
}

void KardWidget::slotShow()
{
	m_gray->show();
}

void KardWidget::slotDisappear()
{
	m_kardW->hide();
	setAutoFillBackground(true);
	QPalette pal;
	pal.setBrush( QPalette::Window, Qt::white);
	setPalette(pal);
	//setBackgroundColor(Qt::white);
}

#include "kardwidget.moc"




