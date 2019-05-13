/**
 * ATC-Lab: Presentation Module
 * UI components for use during task
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 23/11/04
 *
 * $Id: taskwidget.cpp,v 1.2 2008/05/13 17:00:31 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "taskwidget.h"
#include "trialtask.h"
#include "logger.h"
#include <qapplication.h>
#include <qlineedit.h>
#include <qobjectlist.h> 


//==============================================================================
//
// Dialog
//
//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 */
pact::Dialog::Dialog(QWidget* parent, const char* name)
	:QDialog(
		parent, name, false, 
		WStyle_Customize|WStyle_DialogBorder|WStyle_Title
	)
{
}

/*!
 */
pact::Dialog::~Dialog()
{
	hide();
}

//------------------------------------------------------------------------------
// [protected] virtual
//

/*!
 *
 */
void pact::Dialog::keyPressEvent(QKeyEvent* e)
{
	try
	{
		QObject* target = parentWidget()->child(pact::TrialTask::MAIN_TARGET);
		QKeyEvent* keyevent = new QKeyEvent(
			QKeyEvent::KeyPress,
			e->key(),
			e->ascii(),
			e->state(),
			e->text(),
			e->isAutoRepeat(),
			e->count()
		);
		QApplication::postEvent(target, keyevent);
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling pact::Dialog::keyPressEvent");
	}
}

/*!
 *
 */
void pact::Dialog::showEvent(QShowEvent* e)
{
    QObjectList *l = queryList("QPushButton");
    QObjectListIt it( *l ); // iterate over the buttons
    QObject *obj;
    while ( (obj = it.current()) != 0 ) {
        ++it;
		connect(
			(QPushButton*)obj, SIGNAL(pressed()),
			this, SLOT(log_state())
		);
    }
    delete l; // delete the list, not the objects

}

//------------------------------------------------------------------------------
// [private slots]
//

/*!
 *
 */
void pact::Dialog::log_state()
{
	logging::KVPairs kvs;
	kvs.push_back(std::make_pair(
		std::string("elapsed"), 
		QString("%1").arg(dynamic_cast<pact::Task*>(parent())->time()).latin1() 
	));
	kvs.push_back(std::make_pair(name(), std::string("")));
	kvs.push_back(std::make_pair(sender()->name(), std::string("clicked")));

    QObjectList *l = queryList("QLineEdit");
    QObjectListIt it( *l ); // iterate over the buttons
    QObject *obj;
    while ((obj = it.current()) != 0){
        ++it;
		kvs.push_back(std::make_pair(obj->name(), ((QLineEdit*)obj)->text()));
    }
	logging::Logger::instance()->info(kvs);
    delete l; // delete the list, not the objects
}

//==============================================================================
//
// PushButton
//
//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 *
 */
pact::PushButton::PushButton(QWidget* parent, const char* name)
	:QPushButton(parent, name)
{
	setFocusPolicy(QWidget::NoFocus);
	setDefault(false);
	setAutoDefault(false);
}

pact::PushButton::~PushButton()
{}

//==============================================================================
