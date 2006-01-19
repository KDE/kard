/***************************************************************************
 *   Copyright (C) 2001-2006 by Anne-Marie Mahfouf                              *
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
#include <QResizeEvent>

class QLabel;

/**
 *
 * Anne-Marie Mahfouf
 **/
class KardWidget : public QLabel
{
Q_OBJECT
public:
	KardWidget(QWidget *parent, const char *name);
	~KardWidget();

	QLabel* gris;
	QLabel *m_kardW;


protected:
	void mousePressEvent( QMouseEvent * );
	virtual void resizeEvent( QResizeEvent * e );

signals: // Signals
	void clicked();
	void illegal();

public slots:
	// gray card is hidden
	void slotHide();
	// the whole card disappears
	void slotDisappear();
	// show gray card again
	void slotShow();

};

#endif
