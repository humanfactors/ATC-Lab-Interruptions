#ifndef __EXCEL_LOGGER_HDR__
#define __EXCEL_LOGGER_HDR__


#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif

#include <sstream>
#include <stdexcept>
#include <vector>
#include <qfile.h>
#include "logger.h"
#include "../engine/atc.h"
#include "canvasview.h"
#include <map>
#include <string>

namespace ExcelLogging{

    typedef std::pair<std::string, int>		TrialScore;
	typedef std::vector<TrialScore>		    TrialScores;

    struct KeyPressDetail
    {
        KeyPressDetail( std::string     a_key = "",
                        unsigned int    a_time = 0,
                        double          a_timeHp = 0.0) :
            m_key(a_key),
            m_time(a_time),
            m_timeHp(a_timeHp)
        {
        }

        std::string     m_key;
        unsigned int    m_time;
        double          m_timeHp;
    };

	typedef std::vector<KeyPressDetail> KeyPressDetails;

    struct AircraftSelectionDetail
    {
        AircraftSelectionDetail() :
            m_callsign(""),
            m_selectionTime(0),
            m_selectionTimeHp(0.0)
        {
        }

        AircraftSelectionDetail(std::string     a_callsign,
                                unsigned int    a_selectionTime,
                                double          a_selectionTimeHp) :
            m_callsign(a_callsign),
            m_selectionTime(a_selectionTime),
            m_selectionTimeHp(a_selectionTimeHp)
        {
        }

        std::string     m_callsign;
        unsigned int    m_selectionTime;
        double          m_selectionTimeHp;
    };

	enum TaskError
	{
		TASK_ERROR_UNKNOWN = -1,
		TASK_ERROR_INCORRECT = 1,
		TASK_ERROR_CORRECT = 0
	};

	struct FlashDetails
	{
		FlashDetails() : 
			flash_time(atc::INVALID_VALUE), 
            flash_timeHp((double) atc::INVALID_VALUE),
			call_sign(""), 
			task_complete_time(atc::INVALID_VALUE), 
            task_complete_timeHp((double) atc::INVALID_VALUE),
			selection_time(0),
            selection_timeHp((double) atc::INVALID_VALUE),
			error(TASK_ERROR_UNKNOWN),
            m_automatic(false),
            m_dismiss_time(atc::INVALID_VALUE),
            m_dismiss_timeHp((double) atc::INVALID_VALUE)
		{
		}

		std::string		    script_name;
		unsigned int	    flash_time;
        double              flash_timeHp;
		std::string		    call_sign;
		KeyPressDetails	    keysPressed;
		unsigned int	    task_complete_time;
        double              task_complete_timeHp;
		int	                selection_time;
        double              selection_timeHp;
		TaskError		    error;
        bool                m_automatic;
        int                 m_dismiss_time;
        double              m_dismiss_timeHp;
	};

	struct InterventionDetails
	{
		InterventionDetails() :
			intervention_time(0), 
            intervention_timeHp(-1.0),
            call_sign(""), 
            flight_level(0)
		{
		}

		std::string     script_name;
		unsigned int    intervention_time;
        double          intervention_timeHp;
		std::string     call_sign;
		unsigned long   flight_level;
	};

    struct ReminderBoxDismissDetails
    {
        ReminderBoxDismissDetails()
            :   m_script_name(""),
                m_callsign(""),
                m_dismiss_time(atc::INVALID_VALUE),
                m_dismiss_timeHp((double) atc::INVALID_VALUE)
        {
        }

        std::string m_script_name;
        std::string m_callsign;
        int         m_dismiss_time;
        double      m_dismiss_timeHp;
    };

    struct InterruptionTaskDetails
    {
        InterruptionTaskDetails()
            :   m_script_name(""),
                m_interruption_type(""),
                m_start_time(atc::INVALID_VALUE),
                m_end_time(atc::INVALID_VALUE),
                m_show_full_screen(false),
                m_external_program(""),
                m_auto_accept(false),
                m_auto_handoff(false)
        {
        }

        std::string m_script_name;
        std::string m_interruption_type;
        int         m_start_time;
        int         m_end_time;
        bool        m_show_full_screen;
        std::string m_external_program;
        bool        m_auto_accept;
        bool        m_auto_handoff;
    };

    struct NBackTaskResponse
    {
        NBackTaskResponse()
            :   m_script_name(""),
                m_stimuli_number(0),
                m_stimuli(atc::INVALID_VALUE),
                m_response("")
        {
        }

        std::string     m_script_name;
        int             m_stimuli;
        unsigned int    m_stimuli_number;
        std::string     m_response;
    };

	struct excel_logger_error : public std::runtime_error
	{
		excel_logger_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};
	
	class ExcelLogger
	{
	public:

		static void set_name( const std::string & );

		static ExcelLogger* instance();
		static void destroy();

	private:

		ExcelLogger();
		ExcelLogger(const std::string&);
		~ExcelLogger();

	public:
		void add_keyvalue( const std::string &, const std::string & );
		void add_keyvalue( const std::string &, const double &      );
		void write( const char * )const;
		void LogFlashDetailsScriptName(std::string script_name);
		
        void 
        LogAcceptanceFlashDetails(  std::string call_sign, 
                                    unsigned int flash_time,
                                    double hp_flash_time = -1.0);

		void 
        LogAcceptanceKeysPressed(   std::string call_sign, 
                                    std::string a_keyPressed, 
                                    unsigned int a_time,
                                    double a_timeHp = -1.0);

		void 
        LogAcceptanceTime(  std::string     call_sign, 
                            unsigned int    time,
                            double          a_timeHp = -1.0,
                            bool            a_autoAccept = false);
		
        void 
        LogAcceptanceError(std::string call_sign, TaskError error);

        void
        LogReminderDismissTime( std::string     a_callsign,
                                int             a_time,
                                double          a_timeHp);
		
        void 
        LogHandoffFlashDetails( std::string     call_sign, 
                                unsigned int    flash_time,
                                double          a_flashTimeHp = -1.0);

		void 
        LogHandoffKeysPressed(  std::string     call_sign, 
                                std::string     a_keyPressed, 
                                unsigned int    a_time,
                                double          a_timeHp = -1.0);

		void 
        LogHandoffTime( std::string     call_sign, 
                        unsigned int    time,
                        double          a_timeHp = -1.0,
                        bool            a_autoHandoff = false);

		void 
        LogHandoffError(std::string call_sign, TaskError error);		
		
        void
        LogInterruptionDetails( std::string a_interruption_type,
                                int         a_start_time,
                                int         a_end_time,
                                bool        a_show_full_screen,
                                std::string a_external_program,
                                bool        a_auto_accept,
                                bool        a_auto_handoff);

        void
        LogNBackTaskResponse(   int a_stimuli,
                                unsigned int a_stimuli_number,
                                std::string a_response);

        void 
        LogAircraftSelection(   std::string     a_callsign, 
                                unsigned int    a_time,
                                double          a_timeHp = -1.0);

		void WriteResultsForTrial(std::string trial_name);

		void 
        LogInterventionDetails( std::string call_sign, 
								unsigned int time, 
                                double a_timeHp,
								unsigned long flight_level);

		void LogInterventionDetailsMissedAircraft(std::string call_sign);

		void LogRecallDetails(unsigned int recall_number, std::string text);

        void LogTrialScore(std::string a_trial_name);

	private:
		void init();

		std::string compose( const logging::KVPairs& )const;
		void write( const char *, const char * )const;
		int GetMaxKeyPressesForAcceptanceTask();
		int GetMaxKeyPressesForHandoffTask();
		int GetMaxKeyPressesForAllTasks();
        void WriteTrialScores();

	private:

		static ExcelLogger* _instance;

		mutable QFile _file;

		std::vector<std::string> _missed_aircraft;
		std::string			_ostring;
		std::ostringstream	_osstream;
		std::string			_script_name;
		std::map<std::string, FlashDetails>		m_acceptanceDetails;
		std::map<std::string, FlashDetails>		m_handoffDetails;
		std::vector<InterventionDetails>		_intervention_details;
        std::vector<ReminderBoxDismissDetails>  m_reminder_box_dismiss_details;
        std::vector<InterruptionTaskDetails>    m_interruption_task_details;
        std::vector<NBackTaskResponse>          m_nback_task_responses;
		AircraftSelectionDetail	                m_aircraftSelection;
        TrialScores                             m_trialScores;
	};
}; 

#endif 