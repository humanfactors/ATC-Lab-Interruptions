/**
 * @file:       pact_instructiontask.cpp
 *
 * 		        ATC-Lab Presentation Module
 *              Instruction Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: instructiontask.cpp,v 1.11.2.4 2015/03/24 12:40:37 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "instructiontask.h"
#include "logging/ExcelLogger.h"
#include <sstream>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qscrollview.h>
//------------------------------------------------------------------------------
const int COUNTDOWN_TIMEOUT_INTERVAL = 1000;	//milliseconds

unsigned int pact::InstructionTask::_recall_number = 0;
												/*!
 */
pact::InstructionTask::InstructionTask()
	:pact::Task()
	,_textBox(0)
	,_countdown_timer(0)
	,_countdown_timer_label(0)
	,_cur_time(0)
	,_time_out(0)
	,_show_countdown(false)
	,_recall_textbox(NULL)
	,m_pushButton(NULL)
	,m_questionBoxAnswer("")
	,m_questionBoxResponse("")
{
	QVBoxLayout* layout = new QVBoxLayout(this, 20);

	_textBox = new QLabel(this);
	_textBox->setFrameStyle(QFrame::NoFrame|QFrame::Plain);
	_textBox->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::WordBreak);

	layout->addWidget(_textBox);

	QFont f = font();
	f.setPointSize(14);
	setFont(f);
	setFocusPolicy(QWidget::StrongFocus);
}

/*!
 */
pact::InstructionTask::~InstructionTask()
{
	if (_textBox)
	{
		delete _textBox;
	}

	if (_countdown_timer)
	{
		delete _countdown_timer;
	}

	if (_countdown_timer_label)
	{
		delete _countdown_timer_label;
	}
}

//------------------------------------------------------------------------------
/*!
 */
void 
pact::InstructionTask::setText(const std::string &text)
{
	_textBox->setText(text);
}

void pact::InstructionTask::SetTimeOut(unsigned int time_out)
{
	_time_out = time_out;

	_countdown_timer = new QTimer(this);
	_countdown_timer->start(COUNTDOWN_TIMEOUT_INTERVAL);

	connect(_countdown_timer, SIGNAL(timeout()), 
			SLOT(HandleTimeoutUpdate()));
}

void
pact::InstructionTask::SetButtonTimeEvent(unsigned int a_time_out)
{
	if (m_pushButton.get() != NULL)
	{
		_countdown_timer = new QTimer(this);
		_countdown_timer->start(a_time_out * COUNTDOWN_TIMEOUT_INTERVAL);

		m_pushButton->setEnabled(false);

		connect(_countdown_timer, SIGNAL(timeout()), 
				this, SLOT(HandleButtonTimeEventTimeout()));
	}
}

void pact::InstructionTask::SetShowCountDown(bool show_countdown)
{
	_show_countdown = show_countdown;

	QVBoxLayout* layout = new QVBoxLayout(this, 20);

	_countdown_timer_label = new QLabel(this);
	_countdown_timer_label->setFrameStyle(QFrame::NoFrame|QFrame::Plain);
	_countdown_timer_label->setAlignment(Qt::AlignCenter|Qt::AlignBottom|Qt::WordBreak);
	_countdown_timer_label->setGeometry(500, 435, 40, 25);

	layout->addWidget(_countdown_timer_label);
}

void pact::InstructionTask::SetButton(std::string label)
{
	m_pushButton = std::auto_ptr<QPushButton>(new QPushButton(QString(label), 
												dynamic_cast<QWidget*>(this), 
												label.c_str()));

	m_pushButton->setGeometry(460, 660, 80, 30);
	m_pushButton->setEnabled(false);

	connect(m_pushButton.get(), SIGNAL(clicked()), this, SLOT(HandleButtonClicked()));
	//connect(m_pushButton.get(), SIGNAL(clicked()), this, SIGNAL(closed()));
}

void
pact::InstructionTask::HandleTextboxChange()
{
    if (_recall_textbox->text() != "")
    {
        std::string current_text(_recall_textbox->text());
        
        for (int i = 1; i <= current_text.length(); i++)
        {
            char current_char = *(current_text.substr(i - 1, i).c_str());
            int alpha_numeric = isalnum((int) current_char);

            if (!alpha_numeric)
            {
                _recall_textbox->clear();
                break;
            }
        }
    }

	if (m_pushButton.get() != NULL)
	{
		if (_recall_textbox->text() == "")
		{
			m_pushButton->setEnabled(false);
		}
		else
		{
			m_pushButton->setEnabled(true);
		}
	}
}

void pact::InstructionTask::SetTextbox(std::string label)
{
	_recall_textbox = new QTextEdit(dynamic_cast<QWidget*>(this), label.c_str());
    _recall_textbox->setVScrollBarMode(QScrollView::AlwaysOff);
    _recall_textbox->setHScrollBarMode(QScrollView::AlwaysOff);
	_recall_textbox->setGeometry(300, 600, 400, 30);

	connect(_recall_textbox, SIGNAL(textChanged()), this, SLOT(HandleTextboxChange()));
	_recall_number++;
}

void
pact::InstructionTask::SetQuestionBoxAnswer(std::string a_answer)
{
	m_questionBoxAnswer = FormatText(a_answer);
}

void
pact::InstructionTask::SetQuestionBoxIncorrectResponse(std::string a_response)
{
	m_questionBoxResponse = a_response;
}

void pact::InstructionTask::HandleTimeoutUpdate()
{
	_cur_time += 1;

	if ((_show_countdown) && (_countdown_timer_label))
	{
		std::stringstream str_val;
		
		str_val << (_time_out - _cur_time);

		QString countdown = str_val.str();

		_countdown_timer_label->setText(countdown);
	}

	if (_cur_time >= _time_out)
	{
		_cur_time = 0;
		_countdown_timer->stop();
		terminate();
	}
}

void
pact::InstructionTask::HandleButtonTimeEventTimeout()
{
	if (m_pushButton.get() != NULL)
	{
		m_pushButton->setEnabled(true);
	}
}

void pact::InstructionTask::HandleButtonClicked()
{
	if (_recall_textbox != NULL)
	{
		if (_recall_textbox->text() != "")
		{
			ExcelLogging::ExcelLogger::instance()->LogRecallDetails(_recall_number, 
																	_recall_textbox->text());
		}
		else
		{
			return;
		}
	}

	if (m_questionBoxAnswer != "")
	{
		std::string answer = FormatText(_recall_textbox->text());

		if (answer != m_questionBoxAnswer)
		{
			_textBox->setText(m_questionBoxResponse);
			_recall_textbox->clear();
			m_questionBoxAnswer = "";
		}
		else
		{
			closed();
		}
	}
	else
	{
		closed();
	}
}

std::string
pact::InstructionTask::FormatText(const std::string& a_text)
{
	std::string ret = a_text;

	Utils::Utilities::instance()->TrimString(ret);
	Utils::Utilities::instance()->ToUpper(ret);
	Utils::Utilities::instance()->RemoveExcessChars(ret, " ");

	return ret;
}

//------------------------------------------------------------------------------
