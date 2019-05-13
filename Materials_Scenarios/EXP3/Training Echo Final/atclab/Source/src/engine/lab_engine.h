/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: lab_engine.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2.6.4 $ )
      @date     : $Date: 2015/02/23 13:34:42 $
      @state    : $State: Exp $

      $Log: lab_engine.h,v $
      Revision 1.2.6.4  2015/02/23 13:34:42  Aaron Yeung
      - Added logging of interruption tasks to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element				Description							Values
                *******************************************************************************************************************
      	  interruption_type 			Type of interruption						"Interruption" or "NBack"
          	  start_time				Programmed start time of interruption
      	  end_time				Programmed end time of interruption
          	  show_full_screen			Whether the task was shown in full screen			0 = no, 1 = yes
          	  external_program			Name of external program that was run
          	  auto_accept				Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  auto_handoff				Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added logging of interruption tasks to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description							Values
                *******************************************************************************************************************
      	  Trial			Name of the trial that the interruption occurred in
      	  Interruption Type	Type of interruption						"Interruption" or "NBack"
          	  Start Time		Programmed start time of interruption
      	  End Time		Programmed end time of interruption
          	  Show Full Screen	Whether the task was shown in full screen			0 = no, 1 = yes
          	  External Program	Name of external program that was run
          	  Auto Accept		Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  Auto Handoff		Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added additional field in the aircraft flash details section of the output .csv file called "Automatic" to indicate if an aircraft is
      	   automatically accepted or handed off. Values are 0 (not automatic) or 1 (automatic acceptance/handoff)

      	 - Added logging of reminder box dismiss button times to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  reminder_box		Tag to indicate that the reminder box dismiss button was pressed	Aircraft callsign that had the dismiss button pressed
      	  dismiss_button	Tag to indicate that the reminder box dismiss button was pressed	"pressed"
      	  clicked_time		Millisecond time that the dismiss button was pressed

      	- Added logging of reminder box dismiss button times to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description								Values
                *******************************************************************************************************************
      	  Trial  		Name of the trial that the dismiss button was pressed
      	  Callsign		Aircraft callsign that had the dismiss button pressed
      	  Dismiss Time		Time in seconds into the trial that the dismiss button was pressed
      	  Dismiss Time HP	Time in milliseconds into the trial that the dismiss button was pressed

      	- Added calculation of Nback task results based on participant's inputs/non-inputs in the N back task. The are described as follows:
      	  ***********************************************************************************************************************
      	  Result		Description
      	  ***********************************************************************************************************************
      	  Hit			Participant correctly responds to a stimuli that was the same as the stimuli N times before
      	  Miss			Participant fails to respond to a stimuli that was the same as the stimuli N times before
      	  Correct rejection	Participant correct ignores the stimuli which is not the same as the stimulis N times before
      	  False alarm		Paricipant incorrectly responds to a stimuli that was not the same as the stimuli N times before

      	 - Added logging of N back task results to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  NBackTask_response	Stimuli order of the stimuli that was presented
      	  stimuli		The actual stimuli that was displayed to the participant
      	  response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added logging of N back task results to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  Trial	  		Name of the trial that the N back response was generated in
      	  Stimuli Number	Stimuli order of the stimuli that was presented
      	  Stimuli		The actual stimuli that was displayed to the participant
      	  Response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added a count down timer for the blank interruption task. It can be enabled/disabled with the XML below:
      	  *******************************************************************************************************************
                XML Element		Attribute	Description							Values
                *******************************************************************************************************************
      	  atc:interruption	 atc:show_timer	Enable/disable countdown timer in the blank interruption task	'true' or 'false'

      	  Example:
      	  <atc:interruption atc:start='3' atc:end='21' atc:auto_handoff='true' atc:auto_accept='false' atc:show_blank_screen='true' atc:show_timer='true' atc:external_program='"calc.exe"'/>

      	  - Added ability to enable/disable error pop up dialogs when a paricipant misses an aircraft acceptance or handoff.
      	    This can be achieved with the XML below:
      	  ***********************************************************************************************************************************************
                XML Element	Attribute			Description									Values
                ***********************************************************************************************************************************************
      	  atc:trial	atc:notify_missed_acceptance	Enable/disable pop up error dialog when a paricipant misses an acceptance	'true' or 'false'
      			atc:notify_missed_handoff	Enable/disable pop up error dialog when a paricipant misses a handoff		'true' or 'false'

      	  Example:
      	  <atc:trial atc:idx='test2' atc:param='default' atc:map="map1" atc:sky="script2" atc:ui="ui001" atc:notify_missed_acceptance='false' atc:notify_missed_handoff='false'>

      Revision 1.2.6.3  2015/02/16 14:20:01  Aaron Yeung
      Fixed corrupted files

      Revision 1.2.6.1  2014/12/10 13:26:18  Aaron Yeung
      ATC Version 2.4.5.8:

      - Implemented scriptable interruption task with configurable start time, end time, automatic aircraft acceptance,
      	    automatic aircraft handoff and the displaying of a full black screen
                *******************************************************************************************************************
                XML Element		Attributes  		Description
                *******************************************************************************************************************
      	  atc:interruption	atc:start		Trial time when the interruption begins
      				atc:end			Trial time when the interruption ends
      				atc:auto_handoff	Set if aircraft are automatically handed off during the interruption (true/false)
      				atc:auto_accept		Set if aircraft are automatically accepted during the interruption (true/false)
      				atc:show_blank_screen	Set if a full black screen is to be displayed to hide the aircraft simulation (true/false)
      				atc:external_program	Not implemented yet

      Revision 1.2  2007/07/15 04:03:49  Aaron Yeung
      Added xml scriptable trial time

      Revision 1.1.1.2  2007/06/21 12:22:44  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.7  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.6  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.5  2006/10/20 04:49:23  seth
      reduced dimensions of workload popup question && initial implementation of experiment time (for inclusion in log file output)

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.2  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_LAB_ENGINE_HDR__
#define __ATC_LAB_ENGINE_HDR__


#include <qobject.h>
#include "atc_types.h"
#include "trialtask.h"
#include <memory>


namespace atc {

	//
	// atc classes
	//
	
	class Airspace;
	class Canvas;
	class Clock;

    enum RUN_MODE{
	    RUN_MODE_NORMAL,
		RUN_MODE_INTERRUPTION,  // Automatically handoff and accept aircrafts while the participant
                                // is being interrupted with another display
	};

    //
    // atc::LabEngine
    //

    class LabEngine : public QObject {
		Q_OBJECT

    public:
        LabEngine();
        ~LabEngine();

    public: // builder interface
		void set_airspace( std::auto_ptr< Airspace > & );
		void set_clock( std::auto_ptr< Clock > & );
		void set_canvas( std::auto_ptr< Canvas > & );
        void SetInterruptParams(pact::InterruptionParams a_params);
        void SetnBackTaskParams(pact::nBackTaskParams a_params);

		void TrialTime(unsigned int trial_time)
		{
			_trial_time = trial_time;
		}

	public:
		void start();
		void stop();

	public slots:
		void handle_information( KeyValues & ); 
		void HandleClockUpdate(long seconds_elapsed);

	private:
		void initalize_visualizer();

	signals:
		void state_data( KeyValues & ); 
		void TrialTimeout(long seconds_elapsed);
        void event_recorded( KeyValues & );

	private:
		std::auto_ptr< Airspace >   _airspace;
		std::auto_ptr< Canvas >     _canvas;
		std::auto_ptr< Clock >      _clock;

		unsigned int                _trial_time;
        RUN_MODE                    m_runMode;
        pact::InterruptionParams    m_interruptParams;
        pact::nBackTaskParams       m_nBackTaskParams;
	};

	//
	// typedefs
	//

	typedef std::auto_ptr< Airspace > AirspacePtr;
	typedef std::auto_ptr< Canvas >   CanvasPtr;
	typedef std::auto_ptr< Clock >    ClockPtr;


};


#endif
