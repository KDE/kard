/***************************************************************************
 *   Copyright  2001-2008 by Anne-Marie Mahfouf                            *
 *   annma@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kardwidget.h"

#include <KDebug>
#include <kglobalsettings.h>
#include <KLocale>
#include <KStandardDirs>

#include <QPainter>
#include <QSvgRenderer>
#include <QToolTip>
#include <QWhatsThis>

KardWidget::KardWidget(QWidget *parent, const char *)
  : QLabel(parent)
{
    //the kard widget inherits of QLabel and is a child of QWidget
    //the m_kardW wiget is a QLabel on top of the kard widget
    //the m_gray widget is a QSvgRenderer that will recover the m_kardW widget i.e. m_gray = back of the card
    m_kardW = new QLabel(this);
    m_kardW->setGeometry(0, 0, width(), height());

    QString theme = "default";
    QString svgpath = KStandardDirs::locate("data", QString("kard/themes/%1.svg").arg(theme));
    m_gray = new QSvgWidget(svgpath, m_kardW);
    m_gray->setGeometry(0, 0, m_kardW->width(), m_kardW->height());
    m_gray->setToolTip( i18n( "Click to see what is on the back of the card" ) );
    m_gray->setWhatsThis( i18n( "Click on two cards to find out what is on the back of the card and try matching a pair" ) );
    m_kardW->setWhatsThis(i18n( "Click on another card and try matching a pair" ) );
}

KardWidget::~KardWidget()
{
}

void KardWidget::mousePressEvent(QMouseEvent * )
{
    // emit clicked() only if card not already clicked
    if (m_gray->isVisible()==true)  {
	emit clicked();
    }
    else  {
	emit illegal(); //is it compulsory?
    }
}

void KardWidget::resizeEvent( QResizeEvent * )
{
    m_kardW->setGeometry(0, 0, width(), height());
    m_gray->setGeometry(0, 0, m_kardW->width(), m_kardW->height());
}

QSize KardWidget::sizeHint() const
{
    return QSize (m_kardW->width(), m_kardW->height());
}


void KardWidget::slotHide()
{
    if (m_gray->isVisible()==true)  {
	m_gray->hide();
    }
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
}

void KardWidget::setKardBack()
{
    QString theme = "default";
    QString svgpath = KStandardDirs::locate("data", QString("kard/theme/%1.svg").arg(theme));
    m_gray->load(svgpath);
}

#include "kardwidget.moc"




