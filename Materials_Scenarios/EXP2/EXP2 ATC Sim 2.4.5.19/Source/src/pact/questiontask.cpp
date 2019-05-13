/**
 * @file:       pact_questiontask.cpp
 *
 * 		        ATC-Lab Presentation Module
 *              Question Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: questiontask.cpp,v 1.1.1.2 2007/06/21 12:22:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "questiontask.h"

#include <qhbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>


#ifdef _WIN32
	#include <assert.h>
#endif


//------------------------------------------------------------------------------

/*!
 */
pact::QuestionTask::QuestionTask()
	:pact::Task()
{
	_textBox = new QLabel(this);
	_textBox->setFrameStyle(QFrame::NoFrame);
	_textBox->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::WordBreak);

	_btngrp = new QHButtonGroup(this);
	_btngrp->setFrameStyle(QFrame::NoFrame | QFrame::Plain);

	_layout = new QVBoxLayout(this, 20);

	_layout->addStretch();
	_layout->addWidget(_textBox);
	
	QHBoxLayout *btnmgr = new QHBoxLayout(_layout, 20);
	btnmgr->addStretch();
	btnmgr->addWidget(_btngrp);
	btnmgr->addStretch();

	_layout->addStretch();


	QFont f = font();
	f.setPointSize(14);
	setFont(f);
}

/*!
 */
pact::QuestionTask::~QuestionTask()
{}

//------------------------------------------------------------------------------

/*!
 */
void pact::QuestionTask::setText(const std::string& text){
	_textBox->setText(text);
}

/*!
 */
void pact::QuestionTask::setScale(int min, int max)
{
	assert(0 <= min);
	assert(min <= max);

	for(int scale = min; scale <= max; ++scale){
		QPushButton *btn = new QPushButton(_btngrp);
		btn->setText(QString("%1").arg(scale));
		_btngrp->insert(btn, scale);
	}

	connect(_btngrp, SIGNAL(clicked(int)), this, SLOT(setAnswer(int)));

}

//------------------------------------------------------------------------------

/*!
 */
void pact::QuestionTask::setAnswer(int answer)
{
	terminate(QString("%1 ==>> %2").arg(_textBox->text()).arg(answer));
}

//------------------------------------------------------------------------------
