/**
 * @file:       atc_task.h
 *
 * 		        ATC-Lab Presentation Module
 *              Base Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: task.h,v 1.1.1.2 2007/06/21 12:22:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_TASK_HDR__
#define __ATC_TASK_HDR__


#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif


#include "document.h"

#include <qdatetime.h>
#include <qkeysequence.h>
#include <qwidget.h>

#include <set>


namespace pact{


	/*!
	 * \class atc::Task
	 */
	class Task : public QWidget, public xml::DocumentView 
	{
		Q_OBJECT
	public:

		Task();
		virtual ~Task();

	public:

		void addKey( const QKeySequence& );

		int time() const { return _time.elapsed(); }

	public slots:

		virtual void terminate( const char * = "" );

	protected: // virtual

		virtual void hideEvent( QHideEvent * );
		virtual void keyPressEvent( QKeyEvent*);
		virtual void mouseDoubleClickEvent( QMouseEvent * );
		virtual void mouseMoveEvent( QMouseEvent * );
		virtual void mousePressEvent( QMouseEvent * );
		virtual void mouseReleaseEvent( QMouseEvent * );
		virtual void showEvent( QShowEvent * );

	private:

		void log_mouse( QMouseEvent * );

	signals:

		void closed();

	private:

		QTime _time;
		std::set< QKeySequence > _keys;

	};


};


/*!
 */
inline bool 
operator<( const QKeySequence& lhs, const QKeySequence& rhs ) {
	return QString( lhs ) < QString( rhs );
}


#endif
