/**
 * @file:       atc_questiontask.h
 *
 * 		        ATC-Lab Presentation Module
 *              Question Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: questiontask.h,v 1.1.1.2 2007/06/21 12:22:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_QUESTIONTASK_HDR__
#define __ATC_QUESTIONTASK_HDR__


#ifdef WIN32
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif


#include "task.h"


class QHButtonGroup;
class QLabel;
class QVBoxLayout;


namespace pact{


	/*!
	 * \class atc::Question
	 */
	class QuestionTask : public pact::Task
	{
		Q_OBJECT
	public:

		QuestionTask();
		~QuestionTask();

	public:

		void setText(const std::string&);
		void setScale(int, int);

	public slots:

		void setAnswer(int);

	private:

		QVBoxLayout *_layout;
		QLabel *_textBox;
		QHButtonGroup *_btngrp;



	};


};


#endif
