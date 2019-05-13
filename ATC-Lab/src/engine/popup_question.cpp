/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: popup_question.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:45 $
      @state    : $State: Exp $

      $Log: popup_question.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:45  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.4  2006/10/20 04:49:23  seth
      reduced dimensions of workload popup question && initial implementation of experiment time (for inclusion in log file output)

      Revision 1.3  2006/10/17 12:23:30  seth
      Altered selection priority (z) of ac-locator over actual sprite. Fixed requirement dialog to include distance

      Revision 1.2  2006/10/17 07:06:20  seth
      fixed popup question layouts

      Revision 1.1  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "popup_question.h"

#include <qhbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>


using namespace atc;


////////////////////////////////////////////////////////////////////////////////
//
// atc::ScaledQuestion
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
ScaledQuestion::ScaledQuestion( QWidget *w )
	  : QDialog( w, 0, 0, WStyle_Customize | WStyle_NormalBorder | WStyle_Title )
	  , _btns( new QHButtonGroup( /* this */ ) )
{}

/*!
 *
 */
ScaledQuestion::~ScaledQuestion() {}

//------------------------------------------------------------------------------
// [public] initialization
//

/*!
 *
 */
void ScaledQuestion::create_buttons( int count ) {
	for ( int i = 0; i < count; ++i ) {
		QButton *btn = new QPushButton( QString( "%1" ).arg( i + 1 ), this );
		_btns->insert( btn );
	}
	connect( 
			_btns, SIGNAL( clicked( int ) )
		  , this, SLOT( button_clicked( int ) ) 
	);
}

//------------------------------------------------------------------------------
// [public slots]
//

/*!
 *
 */
void ScaledQuestion::button_clicked( int id ) {
	done( _btns->find( id )->text().toInt() );
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::WorkloadQuestion
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
WorkloadQuestion::WorkloadQuestion( QWidget *w ) 
	  : ScaledQuestion( w )  
{
	const int BUTTON_COUNT = 10;

	create_buttons( BUTTON_COUNT );

	setCaption( "WORKLOAD:" );

	QFont f( "Courier New", 12, QFont::Normal );
	setFont( f );

	QFontMetrics fm( f );
	int label_w = 20 * fm.maxWidth();
	int label_h =  5 * fm.lineSpacing();
	//setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
	resize( 0, 0 );

	// description strings
	QStringList txts;
	txts.append( "*Low Workload*\nCould accomplish everything easily." );
	txts.append( "*Moderate Workload*\nCould accomplish everything, but took some effort." );
	txts.append( "*High Workload*\nDifficult to accomplish everything, but did not need assistance." );
	txts.append( "*Extreme Workload*\nExtremely difficult to accomplish everything. Assistance was required." );

	// button manager
	QBoxLayout *btnmgr = new QHBoxLayout();
	for ( int btn = 0; btn < BUTTON_COUNT; ++btn ) {
		QButton *button = _btns->find( btn );
		//button->setMinimumSize( button->sizeHint() );
		button->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
		btnmgr->addWidget( button );
	}

	// descriptive text
	QBoxLayout *txtmgr = new QHBoxLayout();
	for ( int txt = 0; txt < txts.size(); ++txt ) {
		QLabel *label = new QLabel( *txts.at( txt ), this );
		label->setMinimumSize( QSize( label_w, label_h ) );
		label->setAlignment( AlignHCenter | AlignTop | WordBreak );
		label->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
		txtmgr->addWidget( label );
	}

	// vertical layout manager
	QBoxLayout *vmgr = new QVBoxLayout( this, 10 );
	vmgr->addLayout( btnmgr );
	vmgr->addLayout( txtmgr, 1 );

	// prevent resize
	setMaximumSize( minimumSize() );
}

/*!
 *
 */
WorkloadQuestion::~WorkloadQuestion() {}


////////////////////////////////////////////////////////////////////////////////
//
// atc::EfficiencyQuestion
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
EfficiencyQuestion::EfficiencyQuestion( QWidget *w ) 
	  : ScaledQuestion( w )  
{
	const int BUTTON_COUNT = 5;

	create_buttons( BUTTON_COUNT );

	setCaption( "EFFICIENCY:" );

	QFont f( "Courier New", 12, QFont::Normal );
	QFontMetrics fm( f );

	int label_w = 12 * fm.maxWidth();
	int label_h =  4 * fm.lineSpacing();

	setFont( f );
	setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
	resize( 0, 0 );

	QStringList txts;
	txts.append( "Imposes a large penalty" );
	txts.append( "Imposes a moderate penalty" );
	txts.append( "Does not disadvantage or advantage the aircraft" );
	txts.append( "Is beneficial for the aircraft" );
	txts.append( "Is highly beneficial for the aircraft" );

	// low -> high scale labels
	QBoxLayout *scalemgr = new QHBoxLayout();
	scalemgr->addWidget( new QLabel( "Low Efficiency", this ) );
	scalemgr->addStretch();
	scalemgr->addWidget( new QLabel( "High Efficiency", this ) );

	// buttons and labels
	QGridLayout *btnmgr = new QGridLayout( 2, BUTTON_COUNT );
	for ( int btn = 0; btn < BUTTON_COUNT; ++btn ) {
		QButton *button = _btns->find( btn );
		QBoxLayout *hbtnmgr = new QHBoxLayout();
		hbtnmgr->addStretch();
		hbtnmgr->addWidget( button );
		hbtnmgr->addStretch();

		QLabel *label = new QLabel( *txts.at( btn ), this );
		label->setMinimumSize( QSize( label_w, label_h ) );
		label->setAlignment( AlignHCenter | AlignTop | WordBreak );
	
		btnmgr->addLayout( hbtnmgr, 0, btn );
		btnmgr->addWidget( label,   1, btn );
	}
	
	// vertical layout
	QBoxLayout *vmgr = new QVBoxLayout( this, 20 );
	vmgr->addLayout( scalemgr );
	vmgr->addLayout( btnmgr, 1 );
	
	// prevent resize
	setMaximumSize( minimumSize() );
}

/*!
 *
 */
EfficiencyQuestion::~EfficiencyQuestion() {}


////////////////////////////////////////////////////////////////////////////////
