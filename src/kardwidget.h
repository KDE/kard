/***************************************************************************
 *   Copyright  2001-2008 by Anne-Marie Mahfouf                            *
 *   annma@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KARDWIDGET_H_
#define _KARDWIDGET_H_

//Qt headers
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QSvgWidget>

class QSvgRenderer;

/**
 * This class represents each single card widget.  The m_gray label
 * represents the back of the card, the m_kardW represents the painted side of the card.
 *
 * @short Card widget class
 * @author Anne-Marie Mahfouf <annma@kde.org>
 * @version 0.4.2
*/

class KardWidget : public QLabel
{
Q_OBJECT
public:
    /**
    * construtor of KardWidget
    */
    KardWidget(QWidget *parent, const char *name);
    /**
    * destructor
    */
    ~KardWidget();

    ///the back of the card
    QSvgWidget* m_gray;
    ///the painted side of the card
    QLabel *m_kardW;


protected:
    /**
    implement a clicked() signal for the KardWidget class
    i.e. when mouse press kard then clicked() event
    */
    void mousePressEvent( QMouseEvent * );
    /**
    the gray card should cover entirely the colored one
    * @param e the event
    */
    virtual void resizeEvent( QResizeEvent * e );
    
    QSize sizeHint() const;
    
    void setKardBack();
    
    //void paintEvent( QPaintEvent * );


signals: // Signals
    void clicked();
    void illegal();

public slots:
    /**
    hide the gray part of the card
    */
    void slotHide();
    /**
    show the gray card again
    */
    void slotShow();
    /**
    card is matched -> hide it
    */
    void slotDisappear();
};

#endif
