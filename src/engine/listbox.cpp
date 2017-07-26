/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: listbox.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2.6.1 $ )
      @date     : $Date: 2014/12/06 08:13:23 $
      @state    : $State: Exp $

      $Log: listbox.cpp,v $
      Revision 1.2.6.1  2014/12/06 08:13:23  Aaron Yeung
      ATCLab v2.4.5.7
      - Fixed two issues:
      		1) Canvasview grabs all the keypress events which causes other widgets like the altitude selection
      		   tool to not be able to be escaped
      		2) When aircraft are set with a customized handoff key, the default handoff key 'h' is not usable
      		   for handoff

      Revision 1.2  2008/05/13 17:00:31  Aaron Yeung
      Added functionality to:

      - Allow an aircraft's callsign to flash for an acceptance task
      - Add a minimum time to enable an ok button on an instruction form
      - Create reminder box

      Revision 1.1.1.2  2007/06/21 12:22:44  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.10  2006/11/01 04:53:36  seth
      created installer

      Revision 1.9  2006/10/17 12:23:30  seth
      Altered selection priority (z) of ac-locator over actual sprite. Fixed requirement dialog to include distance

      Revision 1.8  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.7  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.6  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.5  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.4  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.3  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.2  2006/06/01 03:41:23  seth
      implemented CFL and speed selectors

      Revision 1.1  2006/05/25 04:15:25  seth
      implemented weather, few minor bug fixes, initial cfl/vel selection


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#include "listbox.h"
#include "atc.h"
#include "aircraft_agent.h"
#include "logging/logger.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>


#ifdef _WIN32
	#include <assert.h>
#endif

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// Class ListBox
//
//------------------------------------------------------------------------------
// [public] construction/destruction
//

/*!
 *
 */
ListBox::ListBox( QWidget *w, const char *n )
	  : QDialog( w, n, 0,WStyle_Customize | WStyle_NoBorder  )
	  , _list( 0 )
	  , _value( 0 )
{
	QFont font;
	font.setFamily( LISTBOX_FONT_FAMILY );
	font.setPointSize( LISTBOX_FONT_POINT );
	setFont( font );
	
	resize( 0, 0 );

	_list = new QListBox( this );

	QBoxLayout *l = new QVBoxLayout( this );
	l->addWidget( _list );


	connect( 
			_list, SIGNAL( selected( const QString & ) )
		  , this, SLOT( selection_event( const QString & ) )
	);
}


/*!
 *
 */
ListBox::~ListBox() {}

//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
const char* ListBox::aircraft_id() const {
	return _agent->callsign();
}

/*!
 *
 */
double ListBox::value() const {
	return _value;
}

//------------------------------------------------------------------------------
// [protected] QWidget
//

/*!
 *
 */
void ListBox::keyPressEvent( QKeyEvent *e ) {
	try
	{
		if ( Qt::Key_Escape == e->key() ) {
			_value = 0;
            
            _list->releaseKeyboard();
			done( Rejected );
            emit Escaped();
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling ListBox::keyPressEvent");
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// Class CFLSelector
//
//------------------------------------------------------------------------------
// [public] ListBox interface
//

/*!
 *
 */
void CFLSelector::set_aircraft( AircraftAgent *ac ) {
	ListBox::set_aircraft( ac );

	_list->clear();

	double min = ac->cfl_min();
	//double std = ac->cfl_std();
	double max = ac->cfl_max();

	int cfl0 = min / CFL_INCREMENT;
	++ cfl0;
	cfl0 *= CFL_INCREMENT;

	for ( int cfl = cfl0; cfl <= max; cfl += CFL_INCREMENT ) {
		_list->insertItem( QString( "%1" ).arg( cfl, CFL_FIELD_WIDTH ) );
	}

	QFontMetrics fm( font() );
	resize(	QSize( 
			fm.width( '0' ) * (CFL_FIELD_WIDTH + 4)
		  , (LISTBOX_ITEMS_SHOWN + 1) * (fm.lineSpacing() + 1) 
	) );

	move( 20, 20 );

	_list->setCurrentItem( 
		(int)( ( ac->calt() - cfl0 ) / CFL_INCREMENT + ZERO_PT_FIVE ) 
	);
	_list->setTopItem( _list->currentItem() - 5 );

    _list->grabKeyboard();
}

//------------------------------------------------------------------------------
// [private]
//

/*!
 *
 */
void CFLSelector::selection_event( const QString &e ) {
	_value = e.toInt();
	_agent->set_cfl( _value );

    _list->releaseKeyboard();
	done( Accepted );
    emit Selected();
}



////////////////////////////////////////////////////////////////////////////////
//
// Class VelocitySelector
//
//------------------------------------------------------------------------------
// [public] ListBox interface
//

/*!
 *
 */
void VelocitySelector::set_aircraft( AircraftAgent *ac ) {
	ListBox::set_aircraft( ac );

	_list->clear();

	double min = ac->vel_min();
	//double std = ac->vel_std();
	double max = ac->vel_max();

	int v0 = min / VELOCITY_INCREMENT;
	++ v0;
	v0 *= VELOCITY_INCREMENT;

	for ( int v = v0; v <= max; v += VELOCITY_INCREMENT ) {
		_list->insertItem( QString( "%1" ).arg( v, VELOCITY_FIELD_WIDTH ) );
	}

	QFontMetrics fm( font() );
	resize(	QSize( 
			fm.width( '0' ) * (VELOCITY_FIELD_WIDTH + 4)
		  , (LISTBOX_ITEMS_SHOWN + 1) * (fm.lineSpacing() + 1) 
	) );

	move( 20, 20 );

	_list->setCurrentItem( 
		(int)( ( ac->speed() - v0 ) / VELOCITY_INCREMENT + ZERO_PT_FIVE) 
	);
	_list->setTopItem( _list->currentItem() - 5 );
}


//------------------------------------------------------------------------------
// [private]
//

/*!
 *
 */
void VelocitySelector::selection_event( const QString &e ) {
	_value = e.toInt();
	_agent->set_velocity( _value );
	done( Accepted );
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::HeadingSelector
//
//------------------------------------------------------------------------------
// [public] ListBox interface
//

/*!
 *
 */
HeadingSelector::HeadingSelector( QWidget *w, const char *n )
	  : ListBox( w, n ) 
{
	for ( int a = 0 ; a <= THREE_SIXTY; a += TEN ) {
		_list->insertItem( QString( "%1" ).arg( a, HEADING_FIELD_WIDTH ) );
	}

	QFontMetrics fm( font() );
	resize(	QSize( 
			fm.width( '0' ) * (HEADING_FIELD_WIDTH + 4)
		  , (LISTBOX_ITEMS_SHOWN + 1) * (fm.lineSpacing() + 1) 
	) );

	move( 20, 20 );
}

/*!
 *
 */
HeadingSelector::~HeadingSelector() {}

/*!
 *
 */
void HeadingSelector::set_aircraft( AircraftAgent *ac ) {
	ListBox::set_aircraft( ac );

	// convert radians to heading degrees ( from North )
	double h = ac->a() * THREE_SIXTY / TWO_PI;
	h -= NINETY;
	h = -h;
	if ( h <  ZERO        ) h += THREE_SIXTY;
	if ( h >= THREE_SIXTY ) h -= THREE_SIXTY;

	// nearest increment
	_list->setCurrentItem( (int)( h / TEN + ZERO_PT_FIVE ) );
	_list->setTopItem( _list->currentItem() - 5 );
}

/*!
 *
 */
void HeadingSelector::selection_event( const QString &e ) {
	double a = e.toDouble();
	a  = -a;
	a += NINETY;
	a  *= TWO_PI / THREE_SIXTY;
	if ( a <= -PI ) a += TWO_PI;
	if ( a >   PI ) a -=     PI;

	_value = (int)a;
	_agent->vector( a );
	done( Accepted );
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::RequirementSelector
//
//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
RequirementSelector::RequirementSelector( QWidget *w )
	  : QDialog( w, 0, 0, WStyle_Customize | WStyle_NormalBorder | WStyle_Title )  
	  , _ac( 0 )
{
	setCaption( "REQUIREMENT:" );
	setFont( QFont( "Courier New", 12, QFont::Normal ) );
	resize( 0, 0 );

	_level       = new QLineEdit( this );
	_distance    = new QLineEdit( this );
	_ok          = new QPushButton( "OK", this );
	_cancel      = new QPushButton( "Cancel", this );

	QGridLayout *mainmgr = new QGridLayout( 3, 3, 5 );
	mainmgr->addWidget( new QLabel( "A level of:", this ), 0, 0 );
	mainmgr->addWidget( _level, 0, 1 );
	mainmgr->addWidget( new QLabel( "(ft)", this ), 0, 2 );
	mainmgr->addWidget( new QLabel( "A distance of:", this ), 1, 0 );
	mainmgr->addWidget( _distance, 1, 1 );
	mainmgr->addWidget( new QLabel( "(nm)", this ), 1, 2 );
	mainmgr->addMultiCellWidget( 
			new QLabel( "is required from current position.", this )
		  , 2, 2, 0, 3 
	);

	QBoxLayout *btnmgr = new QHBoxLayout();
	btnmgr->addStretch();
	btnmgr->addWidget( _ok );
	btnmgr->addWidget( _cancel );

	QBoxLayout *vmgr = new QVBoxLayout( this, 10, 3 );
	vmgr->addLayout( mainmgr );
	vmgr->addLayout( btnmgr  );

	connect( _ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( _cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*!
 *
 */
RequirementSelector::~RequirementSelector() {
	;
}

//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
void RequirementSelector::set_aircraft( AircraftAgent *a ) {
	_ac = a;
	_distance->setText( "10.0" );
}


//------------------------------------------------------------------------------
// [public] accessors
//


/*!
 *
 */
const char* RequirementSelector::aircraft_id() const { 
	return _ac->callsign();
}

/*!
 *
 */
double RequirementSelector::level() const {
	return _level->text().toDouble();
}

/*!
 *
 */
double RequirementSelector::distance() const {
	return _distance->text().toDouble();
}


//------------------------------------------------------------------------------
// [protected slots]
//

/*!
 *
 */
void RequirementSelector::accept() {
	_ac->require_level( level() );
	QDialog::accept();
}


////////////////////////////////////////////////////////////////////////////////
