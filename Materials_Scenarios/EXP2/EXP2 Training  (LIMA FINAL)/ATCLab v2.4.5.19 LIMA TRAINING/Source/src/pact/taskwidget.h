/**
 *
 * ATC-Lab: Presentation Module
 * UI components for use during task
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 23/11/04
 *
 * $Id: taskwidget.h,v 1.1.1.2 2007/06/21 12:22:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __PACT_TASKWIDGET_HDR__
#define __PACT_TASKWIDGET_HDR__

#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif


#include <qdialog.h>
#include <qpushbutton.h>
#include <stdexcept>


namespace pact{


	/*!
	 * \struct atc::taskwidget_error
	 */
	struct taskwidget_error : public std::runtime_error{
		taskwidget_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class pact::TaskDialog
	 */
	class Dialog : public QDialog
	{
		Q_OBJECT

	public:

		Dialog(QWidget*, const char*);
		~Dialog();

	protected: // virtual

		void keyPressEvent(QKeyEvent*); 
		void showEvent(QShowEvent*);

	private slots:

		void log_state();

	};


	/*!
	 * \class pact::PushButton
	 */
	class PushButton : public QPushButton
	{
	public:

		PushButton(QWidget*, const char*);
		~PushButton();

	};

};

#endif
