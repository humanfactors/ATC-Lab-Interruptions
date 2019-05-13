/**
 * @file:       atc_instructiontask.h
 *
 * 		        ATC-Lab Presentation Module
 *              Instruction Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: instructiontask.h,v 1.8.2.4 2015/03/23 13:41:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_INSTRUCTIONTASK_HDR__
#define __ATC_INSTRUCTIONTASK_HDR__

#include <qtimer.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include "task.h"


class QLabel;


namespace pact{


	/*!
	 * \class atc::InstructionTask
	 */
	class InstructionTask : public pact::Task
	{
		Q_OBJECT

	public:

		InstructionTask();
		virtual ~InstructionTask();

	public:

		void setText(const std::string&);
		void SetTimeOut(unsigned int time_out);
		void SetShowCountDown(bool show_countdown);
		void SetButton(std::string label);
		void SetTextbox(std::string label);
		void SetButtonTimeEvent(unsigned int a_time_out);
		void SetQuestionBoxAnswer(std::string a_answer);
		void SetQuestionBoxIncorrectResponse(std::string a_response);

	public slots:

	private slots:
		void HandleTimeoutUpdate();
		void HandleButtonClicked();
		void HandleButtonTimeEventTimeout();
		void HandleTextboxChange();

	private:
		std::string FormatText(const std::string& a_text);

		QTimer *_countdown_timer;
		QLabel *_textBox;
		QLabel *_countdown_timer_label;
		QTextEdit* _recall_textbox;
		std::auto_ptr<QPushButton> m_pushButton;
		unsigned int _time_out;
		unsigned int _cur_time;
		bool _show_countdown;
		static unsigned int _recall_number;
		std::string m_questionBoxAnswer;
		std::string m_questionBoxResponse;
	};


};


#endif
