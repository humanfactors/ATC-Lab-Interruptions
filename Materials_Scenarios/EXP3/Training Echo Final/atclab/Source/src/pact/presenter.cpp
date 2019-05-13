/**
 * @file:       atc_presenter.cpp
 *
 * 		ATC-Lab Presentation Module
 *              Presentation Management.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: presenter.cpp,v 1.3 2010/04/08 13:14:34 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "presenter.h"

#include <qkeysequence.h>
#include "task.h"
#include "taskfactory.h"
#include "logger.h"

#ifdef WIN32
	#include <assert.h>
#endif

using namespace pact;

//------------------------------------------------------------------------------

/**
 * Constructor
 */
Presenter::Presenter( QWidget *parent, const char *name )
	  : QWidgetStack( parent, name )
	  , _experiment( 0 )
	  , _task()
{
	QFont f = font();
	f.setPointSize( 26 );
	setFont(f);
}

/**
 * Destructor
 */
Presenter::~Presenter() {
	_experiment = 0;	// not owned by this
}

//------------------------------------------------------------------------------
// Public Interface
//

/**
 * Set the Experiment which defines the presentation tasks.
 *
 * @param exp pointer to the experiment (does not take ownership)
 */
void Presenter::setExperiment( Experiment *exp ) {
	_experiment = exp;

	logging::KVPairs kvs;
	kvs.push_back( std::make_pair(
			std::string( "experiment" ), exp->root()->idx
	) );
	logging::Logger::instance()->info( kvs );
}

/*!
 * Start the Presentation of Tasks as defined in the Experiment
 *
 * pre-condition: the experiment has been set
 */
void Presenter::start() {
	assert( _experiment != 0 );
	_task = _experiment->begin();
	presentTask();
}

//------------------------------------------------------------------------------
// Private Implementaion
//

/**
 * Create and Present the next Task
 */
void Presenter::presentTask() {
	if ( _task != _experiment->end() ) {

		logging::KVPairs kvs;
		kvs.push_back( 
				std::make_pair( std::string("phase"), (*_task)->parent->idx) 
		);
		kvs.push_back( 
				std::make_pair( std::string("type"), (*_task)->type) 
		);
		kvs.push_back( 
				std::make_pair( std::string("task_id"), (*_task)->idx) 
		);
		logging::Logger::instance()->info( kvs );

		QWidget *w = TaskFactory::createTask( _experiment, *_task );
		connect( w, SIGNAL( closed() ), this, SLOT( presentTask() ) );

		QWidget *current = visibleWidget();
		addWidget(w);			// place in stack
		raiseWidget(w);			// move to top of stack

		if ( current ) {
			removeWidget( current );
			delete current;
			current = 0;
		}

		++_task;
		//w->setFocus();
	}
	else
	{
		close();
	}
}


//------------------------------------------------------------------------------
// Private Virtual
//

/*!
 * Event handler for key press events.
 * 
 * The handler deals with the flow control of the experiment's presentation. Key
 * events not handled by the presentation task should get passed down to this 
 * handler. Only 'secret' key combinations are tested for here.
 */
void Presenter::keyPressEvent( QKeyEvent *e ) {
	try
	{
		int key = e->key();
		if ( e->state() & Qt::ShiftButton )   key += Qt::SHIFT;
		if ( e->state() & Qt::ControlButton ) key += Qt::CTRL;
		if ( e->state() & Qt::AltButton )     key += Qt::ALT;
		if ( e->state() & Qt::MetaButton )    key += Qt::META;
		
		QKeySequence key_sequence( key );

		if ( key_sequence == QKeySequence( "Alt+Ctrl+N" ) ) {
			presentTask();
		}
		else if ( key_sequence == QKeySequence( "Alt+Ctrl+Q" ) ) {
			close();
		}
		else{
			e->ignore();
		}
	}
	catch (...)
	{
	}
}

//------------------------------------------------------------------------------

