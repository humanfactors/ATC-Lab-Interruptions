/**
 * @file:       atc_task.cpp
 *
 * 		        ATC-Lab Presentation Module
 *              Base Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: task.cpp,v 1.3 2010/04/08 13:16:17 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "task.h"
#include "logger.h"

#include <sstream>
#include <qtimer.h>

using namespace pact;

//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 *
 */
Task::Task()
	  : QWidget()
	  , _time()
	  , _keys()
{}

/*!
 *
 */
Task::~Task() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 */
void Task::addKey( const QKeySequence &key ) {
	_keys.insert( key );
}

//------------------------------------------------------------------------------
// [public] slots
//

/*!
 *
 */
void Task::terminate( const char *msg ) {
	QTimer::singleShot( 0, this, SIGNAL( closed() ) );
}

//------------------------------------------------------------------------------
// [protected] virtual
//

/*!
 *
 */
void Task::hideEvent( QHideEvent * ) {
	try
	{
		releaseMouse();
		releaseKeyboard();
	/*
		logging::KVPairs kvs;
		kvs.push_back(std::make_pair(
			std::string("action"), 
			std::string("hide")
		));
		logging::Logger::instance()->info(kvs);
	*/
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::hideEvent");
	}
}

/*!
 *
 */
void Task::keyPressEvent( QKeyEvent *e ) {
	try
	{
		int key = e->key();
		if(e->state() & Qt::ShiftButton) key += Qt::SHIFT;
		if(e->state() & Qt::ControlButton) key += Qt::CTRL;
		if(e->state() & Qt::AltButton) key += Qt::ALT;
		if(e->state() & Qt::MetaButton) key += Qt::META;
		
		QKeySequence key_sequence(key);
		
		logging::KVPairs kvs;
		kvs.push_back(std::make_pair(std::string("elapsed"), QString("%1").arg(_time.elapsed()).latin1()));
		kvs.push_back(std::make_pair(std::string("key"), QString(key_sequence).latin1()));

		if ( _keys.count( key_sequence ) ) {
			kvs.push_back( std::make_pair(
					std::string("action")
				  ,	std::string("terminate")
			) );
			
			terminate();
		}
		else {
			e->ignore();
		}
		
		logging::Logger::instance()->info( kvs );
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::keyPressEvent");
	}
}

/*!
 *
 */
void Task::mouseDoubleClickEvent( QMouseEvent* e ) {
	try
	{
		log_mouse( e );
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::mouseDoubleClickEvent");
	}
}

/*!
 *
 */
void Task::mouseMoveEvent( QMouseEvent* e ) {
	try 
	{
		log_mouse( e );
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::mouseMoveEvent");
	}
}

/*!
 *
 */
void Task::mousePressEvent( QMouseEvent* e ) {
	try
	{
		log_mouse( e );
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::mousePressEvent");
	}
}

/*!
 *
 */
void Task::mouseReleaseEvent( QMouseEvent* e ) {
	try
	{
		log_mouse( e );
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::mouseReleaseEvent");
	}
}

/*!
 *
 */
void Task::showEvent(QShowEvent * ) {
	try
	{
		setFocus();
		_time.start();
	/*
		logging::KVPairs kvs;
		kvs.push_back(std::make_pair(std::string("elapsed"), QString("%1").arg(_time.elapsed()).latin1()));
		kvs.push_back(std::make_pair(std::string("action"), std::string("show")));
		logging::Logger::instance()->info(kvs);
	*/
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling Task::showEvent");
	}
}


//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
void Task::log_mouse( QMouseEvent* e ) {
	std::string type =
		e->type() == QEvent::MouseButtonDblClick ? "dbl_clicked" :
		e->type() == QEvent::MouseMove           ? "moved"       :
		e->type() == QEvent::MouseButtonPress    ? "pressed"     :
		e->type() == QEvent::MouseButtonRelease  ? "released"    :
		                                           "UNKOWN"      ;
		
	logging::KVPairs kvs;
	kvs.push_back(std::make_pair(std::string("elapsed"), QString("%1").arg(_time.elapsed()).latin1()));
	kvs.push_back(std::make_pair(std::string("mouse"), type));
	kvs.push_back(std::make_pair(std::string("x"), QString("%1").arg(e->x()).latin1()));
	kvs.push_back(std::make_pair(std::string("y"), QString("%1").arg(e->y()).latin1()));
	kvs.push_back(std::make_pair(std::string("button"), QString("%1").arg(e->button()).latin1()));
	kvs.push_back(std::make_pair(std::string("state"), QString("%1").arg(e->state()).latin1()));
	logging::Logger::instance()->info(kvs);
}

//------------------------------------------------------------------------------
