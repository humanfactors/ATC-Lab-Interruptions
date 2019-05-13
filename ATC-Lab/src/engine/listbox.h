/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: listbox.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2.6.1 $ )
      @date     : $Date: 2014/12/06 08:13:23 $
      @state    : $State: Exp $

      $Log: listbox.h,v $
      Revision 1.1.1.2.6.1  2014/12/06 08:13:23  Aaron Yeung
      ATCLab v2.4.5.7
      - Fixed two issues:
      		1) Canvasview grabs all the keypress events which causes other widgets like the altitude selection
      		   tool to not be able to be escaped
      		2) When aircraft are set with a customized handoff key, the default handoff key 'h' is not usable
      		   for handoff

      Revision 1.1.1.2  2007/06/21 12:22:44  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.7  2006/10/17 12:23:30  seth
      Altered selection priority (z) of ac-locator over actual sprite. Fixed requirement dialog to include distance

      Revision 1.6  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.5  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.4  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.3  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.2  2006/06/01 03:41:23  seth
      implemented CFL and speed selectors

      Revision 1.1  2006/05/25 04:15:25  seth
      implemented weather, few minor bug fixes, initial cfl/vel selection


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#ifndef __ATC_LIST_BOX_HDR__
#define __ATC_LIST_BOX_HDR__

#include <qlistbox.h>
#include <qdialog.h>

//
// Qt
//
class QLabel;
class QLineEdit;
class QPushButton;


namespace atc{

	//
	// atc::
	//
	class AircraftAgent;


	//
	// atc::ListBox
	//
	class ListBox : public QDialog {
		Q_OBJECT;

	public:
		ListBox( QWidget * = 0, const char * = 0 );
		~ListBox();

	public:
		virtual void set_aircraft( AircraftAgent *ac ) { _agent = ac; }
		void  clear_aircraft() { _agent = 0; }

	public:
		virtual const char* type() const = 0;

		const char* aircraft_id() const;
		double value() const;
    
    signals:
        void Selected();
        void Escaped();

	protected: // QWidget
		virtual void keyPressEvent( QKeyEvent * );

	private slots:
		virtual void selection_event( const QString & ) = 0;

	protected:
		AircraftAgent *_agent;
		QListBox      *_list;
		int            _value;

	};


	/*!
	 *
	 */
	class CFLSelector : public ListBox {
		Q_OBJECT
	
	public:
		CFLSelector( QWidget *w = 0, const char *n = 0 )
			  : ListBox( w, n ) {}
		~CFLSelector() {}

	public: // virtual
		void set_aircraft( AircraftAgent * );
		const char* type() const { return "cleared_flight_level"; }

	private slots:
		void selection_event( const QString & );

	};


	/*!
	 *
	 */
	class VelocitySelector : public atc::ListBox {
		Q_OBJECT
	public:
		VelocitySelector( QWidget *w = 0, const char *n = 0 )
			  : ListBox( w, n ) {}
		~VelocitySelector() {}

	public:
		void set_aircraft( AircraftAgent * );
		const char* type() const { return "cleared_airspeed"; }

	private slots:
		void selection_event( const QString & );

	};

	//
	// atc::HeadingSelector
	//
	class HeadingSelector : public ListBox {
		Q_OBJECT

	public:
		HeadingSelector( QWidget * = 0, const char * = 0 );
		~HeadingSelector();

	public:
		void set_aircraft( AircraftAgent * );
		const char* type() const { return "heading_selector"; }

	private slots:
		void selection_event( const QString & );

	};

	//
	// atc::RequirementSelector
	//
	class RequirementSelector : public QDialog {
		Q_OBJECT

	public:
		RequirementSelector( QWidget * );
		~RequirementSelector();

	public:
		void set_aircraft( AircraftAgent * );

	public:
		const char* aircraft_id() const;
		double level   () const;
		double distance() const;

	protected slots:
		void accept();


	private:
		AircraftAgent *_ac;

		QLineEdit   *_level;
		QLineEdit   *_distance;
		QPushButton *_ok;
		QPushButton *_cancel;

	};

};

#endif