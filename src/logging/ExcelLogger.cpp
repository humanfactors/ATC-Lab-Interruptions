#include "ExcelLogger.h"
#include "ScoreLogger.h"
#include <qdatetime.h>
#include <qstring.h>
#include <qtextstream.h>
#include <sstream>

using namespace ExcelLogging;

ExcelLogger* ExcelLogger::_instance = 0;

void ExcelLogger::set_name( const std::string &fn ) {
	if ( !_instance ) _instance = new ExcelLogger( fn );
}

ExcelLogger* ExcelLogger::instance() {
	if(!_instance) _instance = new ExcelLogger;
	return _instance;
}

void ExcelLogger::destroy()
{
	if(_instance){
        _instance->WriteTrialScores();
		delete(_instance);
		_instance = 0;
	}
}

//------------------------------------------------------------------------------
// Construction/Destruction
//

ExcelLogger::ExcelLogger()
	: _file( "atc-lab.csv" ) 
	, _ostring()
	, _osstream( _ostring )
	, _script_name("")
{
	init();
}

ExcelLogger::ExcelLogger( const std::string &fn )
	: _file( fn )
	, _ostring()
	, _osstream( _ostring )
	, _script_name("")
{
	init();
}

void ExcelLogger::init() {
	if ( !_file.open(IO_WriteOnly) )
		throw excel_logger_error( "unable to open log file" );
}

ExcelLogger::~ExcelLogger()
{
	_file.close();
}

void ExcelLogger::LogFlashDetailsScriptName(std::string script_name)
{
	_script_name = script_name;
	_missed_aircraft.clear();
	m_acceptanceDetails.clear();
	m_handoffDetails.clear();
    _intervention_details.clear();
    m_reminder_box_dismiss_details.clear();
    m_interruption_task_details.clear();
    m_nback_task_responses.clear();

    m_aircraftSelection.m_callsign = "";
    m_aircraftSelection.m_selectionTime = atc::INVALID_VALUE;
    m_aircraftSelection.m_selectionTimeHp = (double) atc::INVALID_VALUE;
}

void 
ExcelLogger::LogAcceptanceFlashDetails(std::string  call_sign, 
                                       unsigned int flash_time,
                                       double       a_hpFlashTime)
{
	m_acceptanceDetails[call_sign].script_name = _script_name;
	m_acceptanceDetails[call_sign].flash_time = flash_time;
    m_acceptanceDetails[call_sign].flash_timeHp = a_hpFlashTime;
	m_acceptanceDetails[call_sign].call_sign = call_sign;
}

void 
ExcelLogger::LogAcceptanceKeysPressed(  std::string     call_sign, 
										std::string     a_keyPressed, 
										unsigned int    a_time,
                                        double          a_hpTime)
{
	KeyPressDetail kpd = KeyPressDetail(a_keyPressed, a_time, a_hpTime);
	KeyPressDetails kpds;

	if (m_acceptanceDetails.find(call_sign) != m_acceptanceDetails.end())
	{
		kpds = m_acceptanceDetails[call_sign].keysPressed;
	}

	kpds.push_back(kpd);
	m_acceptanceDetails[call_sign].keysPressed = kpds;
}

void 
ExcelLogger::LogAcceptanceTime(std::string  call_sign, 
                               unsigned int time,
                               double       a_timeHp,
                               bool         a_autoAccept)
{
    m_acceptanceDetails[call_sign].m_automatic = a_autoAccept;

    if (a_autoAccept)
    {
        m_acceptanceDetails[call_sign].task_complete_time = m_acceptanceDetails[call_sign].flash_time;
        m_acceptanceDetails[call_sign].task_complete_timeHp = m_acceptanceDetails[call_sign].flash_timeHp;
        m_acceptanceDetails[call_sign].selection_time = atc::INVALID_VALUE;
        m_acceptanceDetails[call_sign].selection_timeHp = atc::INVALID_VALUE;
    }
    else if (call_sign == m_aircraftSelection.m_callsign)
	{
        m_acceptanceDetails[call_sign].task_complete_time = time;
        m_acceptanceDetails[call_sign].task_complete_timeHp = a_timeHp;
		m_acceptanceDetails[call_sign].selection_time = m_aircraftSelection.m_selectionTime;
        m_acceptanceDetails[call_sign].selection_timeHp = m_aircraftSelection.m_selectionTimeHp;
	}
}

void ExcelLogger::LogAcceptanceError(std::string call_sign, TaskError error)
{
	m_acceptanceDetails[call_sign].error = error;
}

void 
ExcelLogger::LogHandoffFlashDetails(std::string     call_sign, 
                                    unsigned int    flash_time,
                                    double          a_flashTimeHp)
{
	m_handoffDetails[call_sign].script_name = _script_name;
	m_handoffDetails[call_sign].flash_time = flash_time;
    m_handoffDetails[call_sign].flash_timeHp = a_flashTimeHp;
	m_handoffDetails[call_sign].call_sign = call_sign;
}

void 
ExcelLogger::LogHandoffKeysPressed(std::string          call_sign, 
										std::string     a_keyPressed, 
										unsigned int    a_time,
                                        double          a_timeHp)
{
	KeyPressDetail kpd = KeyPressDetail(a_keyPressed, a_time, a_timeHp);
	KeyPressDetails kpds;

	if (m_handoffDetails.find(call_sign) != m_handoffDetails.end())
	{
		kpds = m_handoffDetails[call_sign].keysPressed;
	}

	kpds.push_back(kpd);
	m_handoffDetails[call_sign].keysPressed = kpds;
}

void 
ExcelLogger::LogHandoffTime(    std::string    call_sign, 
                                unsigned int   time,
                                double         a_timeHp,
                                bool           a_autoHandoff)
{
	m_handoffDetails[call_sign].m_automatic = a_autoHandoff;

    if (a_autoHandoff)
    {
        m_handoffDetails[call_sign].task_complete_time = m_handoffDetails[call_sign].flash_time;
	    m_handoffDetails[call_sign].task_complete_timeHp = m_handoffDetails[call_sign].flash_timeHp;
        m_handoffDetails[call_sign].selection_time = atc::INVALID_VALUE;
        m_handoffDetails[call_sign].selection_timeHp = atc::INVALID_VALUE;
    }
	else if (call_sign == m_aircraftSelection.m_callsign)
	{
        m_handoffDetails[call_sign].task_complete_time = time;
	    m_handoffDetails[call_sign].task_complete_timeHp = a_timeHp;
		m_handoffDetails[call_sign].selection_time = m_aircraftSelection.m_selectionTime;
        m_handoffDetails[call_sign].selection_timeHp = m_aircraftSelection.m_selectionTimeHp;
	}

}

void ExcelLogger::LogHandoffError(std::string call_sign, TaskError error)
{
	m_handoffDetails[call_sign].error = error;
}

void
ExcelLogger::LogReminderDismissTime(    std::string     a_callsign,
                                        int             a_time,
                                        double          a_timeHp)
{
    ReminderBoxDismissDetails dismiss_details;

    dismiss_details.m_script_name = _script_name;
    dismiss_details.m_callsign = a_callsign;
    dismiss_details.m_dismiss_time = a_time;
    dismiss_details.m_dismiss_timeHp = a_timeHp;
        
    m_reminder_box_dismiss_details.push_back(dismiss_details);
}

void
ExcelLogger::LogInterruptionDetails(    std::string a_interruption_type,
                                        int         a_start_time,
                                        int         a_end_time,
                                        bool        a_show_full_screen,
                                        std::string a_external_program,
                                        bool        a_auto_accept,
                                        bool        a_auto_handoff)
{
    InterruptionTaskDetails details;

    details.m_script_name = _script_name;
    details.m_interruption_type = a_interruption_type;
    details.m_start_time = a_start_time;
    details.m_end_time = a_end_time;
    details.m_show_full_screen = a_show_full_screen;
    details.m_external_program = a_external_program;
    details.m_auto_accept = a_auto_accept;
    details.m_auto_handoff = a_auto_handoff;

    m_interruption_task_details.push_back(details);
}

void
ExcelLogger::LogNBackTaskResponse(  int a_stimuli,
                                    unsigned a_stimuli_number,
                                    std::string a_response)
{
    NBackTaskResponse response;

    response.m_script_name = _script_name;
    response.m_stimuli = a_stimuli,
    response.m_stimuli_number = a_stimuli_number;
    response.m_response = a_response;

    m_nback_task_responses.push_back(response);
}

void
ExcelLogger::LogAircraftSelection(  std::string a_callsign,
									unsigned int a_time,
                                    double a_timeHp)
{
	m_aircraftSelection = AircraftSelectionDetail(a_callsign, a_time, a_timeHp);
}

void ExcelLogger::LogInterventionDetails(   std::string     call_sign, 
											unsigned int    time, 
                                            double          a_timeHp,
											unsigned long   flight_level)
{
	InterventionDetails details;

	details.script_name = _script_name;
	details.intervention_time = time;
    details.intervention_timeHp = a_timeHp;
	details.call_sign = call_sign;
	details.flight_level = flight_level;

	_intervention_details.push_back(details);
}

void ExcelLogger::LogInterventionDetailsMissedAircraft(std::string call_sign)
{
	bool found = false;

	for (std::vector<std::string>::iterator it = _missed_aircraft.begin();
			it != _missed_aircraft.end(); it++)
	{
		if (*it == call_sign)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		_missed_aircraft.push_back(call_sign);
	}
}

void ExcelLogger::LogRecallDetails(unsigned int recall_number, std::string text)
{
	bool first_recall_for_trial = ((recall_number % 2) != 0);

	if (first_recall_for_trial)
	{
		write("Recall No,Text Entered");
	}

	std::stringstream data;

	data << recall_number << "," << text;
	write(data.str().c_str());

	if (!first_recall_for_trial)
	{
		write("");
	}
}

void
ExcelLogger::LogTrialScore(std::string a_trial_name)
{
    float total_acceptance_task_score = 0.0;
	float total_conflict_score = 0.0;
	float totalHandoffTaskScore = 0.0;
    float total_score = 0.0;

    Score::ScoreLogger::instance()->GetTotalScoreForTrial(
				a_trial_name.c_str(),
				total_acceptance_task_score, 
				total_conflict_score,
				totalHandoffTaskScore);

			
    total_score = (total_acceptance_task_score + total_conflict_score + totalHandoffTaskScore);

    m_trialScores.push_back(std::pair<std::string, int>(a_trial_name, (int) total_score));
}

void ExcelLogger::WriteResultsForTrial(std::string trial_name)
{
	int count = 0;
	int totalAcceptanceError = 0;
	int totalHandoffError = 0;
	std::stringstream heading;

	heading << "Trial,Flash Time,Flash Time (HP),Flash CS,Type,Automatic,Selection Time,Selection Time (HP),";

	for (int i = 1; i <= GetMaxKeyPressesForAllTasks(); i++)
	{
		heading << "Key Pressed,Time,Time (HP),";
	}

	heading << "Acceptance Time,Acceptance Time (HP),Error,Error Total,Score";

	ExcelLogging::ExcelLogger::instance()->write(heading.str().c_str());

	// Write acceptance task details
	for (std::map<std::string, FlashDetails>::iterator it = 
			m_acceptanceDetails.begin(); it != m_acceptanceDetails.end(); it++)
	{
		if (it->second.script_name == trial_name)
		{
			if ((it->second.error == TASK_ERROR_INCORRECT) ||
				(it->second.error == TASK_ERROR_UNKNOWN))
			{
				totalAcceptanceError++;
			}
		}
	}

	for (std::map<std::string, FlashDetails>::iterator it2 = 
			m_acceptanceDetails.begin(); it2 != m_acceptanceDetails.end(); it2++)
	{
		if (it2->second.script_name == trial_name)
		{
			std::stringstream results;
			results << it2->second.script_name	<< "," 
				<< it2->second.flash_time		<< "," 
                << it2->second.flash_timeHp     << ","
				<< it2->second.call_sign		<< "," 
				<< "accept"						<< ","
                << it2->second.m_automatic      << ","
				<< it2->second.selection_time	<< ","
                << it2->second.selection_timeHp	<< ",";

			int counter = 0;
			for (KeyPressDetails::iterator it3 = it2->second.keysPressed.begin();
					it3 != it2->second.keysPressed.end(); it3++)
			{
				results << it3->m_key	<< "," << it3->m_time << "," << it3->m_timeHp << ",";
				counter++;
			}

			int maxAcceptanceKeyPresses = GetMaxKeyPressesForAllTasks();
			if (counter < maxAcceptanceKeyPresses)
			{
				for (int j = counter; j < maxAcceptanceKeyPresses; j++)
				{
					results << ",,,";
				}
			}

			if (it2->second.task_complete_time == atc::INVALID_VALUE)
			{
				results	<< ",";
			}
			else
			{
				results << it2->second.task_complete_time << ",";
			}
			
            if (it2->second.task_complete_timeHp < 0)
			{
				results	<< ",";
			}
			else
			{
				results << it2->second.task_complete_timeHp << ",";
			}

			if (it2->second.error == atc::INVALID_VALUE)
			{
				results << ",";
			}
			else
			{
				results << it2->second.error << ",";
			}

			if (count == 0)
			{
				results << totalAcceptanceError << ",";
			}
			else
			{
				results << ",";
			}

			float total_acceptance_task_score = 0.0;
			float total_conflict_score = 0.0;
			float totalHandoffTaskScore = 0.0;

			Score::ScoreLogger::instance()->GetTotalScoreForTrial(
				trial_name.c_str(),
				total_acceptance_task_score, 
				total_conflict_score,
				totalHandoffTaskScore);

			results << (total_acceptance_task_score + total_conflict_score + totalHandoffTaskScore) << ",";

			write(results.str().c_str());
			count++;
		}
	}

	write("");
	count = 0;

	// Write handoff task details
	for (std::map<std::string, FlashDetails>::iterator it6 = 
			m_handoffDetails.begin(); it6 != m_handoffDetails.end(); it6++)
	{
		if (it6->second.script_name == trial_name)
		{
			if ((it6->second.error == TASK_ERROR_INCORRECT) ||
				(it6->second.error == TASK_ERROR_UNKNOWN))
			{
				totalHandoffError++;
			}
		}
	}

	for (std::map<std::string, FlashDetails>::iterator it4 = 
			m_handoffDetails.begin(); it4 != m_handoffDetails.end(); it4++)
	{
		if (it4->second.script_name == trial_name)
		{
			std::stringstream results;
			results << it4->second.script_name	<< "," 
				<< it4->second.flash_time		<< "," 
                << it4->second.flash_timeHp		<< "," 
				<< it4->second.call_sign		<< "," 
				<< "handoff"					<< ","
                << it4->second.m_automatic      << ","
				<< it4->second.selection_time	<< ","
                << it4->second.selection_timeHp	<< ",";

			int counter = 0;
			for (KeyPressDetails::iterator it5 = it4->second.keysPressed.begin();
					it5 != it4->second.keysPressed.end(); it5++)
			{
				results << it5->m_key	<< "," << it5->m_time << "," << it5->m_timeHp << ",";
				counter++;
			}

			int maxHandoffKeyPresses = GetMaxKeyPressesForAllTasks();
			if (counter < maxHandoffKeyPresses)
			{
				for (int j = counter; j < maxHandoffKeyPresses; j++)
				{
					results << ",,,";
				}
			}

			if (it4->second.task_complete_time == atc::INVALID_VALUE)
			{
				results	<< ",";
			}
			else
			{
				results << it4->second.task_complete_time << ",";
			}
			
            if (it4->second.task_complete_timeHp < 0)
			{
				results	<< ",";
			}
			else
			{
				results << it4->second.task_complete_timeHp << ",";
			}

			if (it4->second.error == atc::INVALID_VALUE)
			{
				results << ",";
			}
			else
			{
				results << it4->second.error << ",";
			}

			if (count == 0)
			{
				results << totalHandoffError << ",";
			}
			else
			{
				results << ",";
			}

			float total_acceptance_task_score = 0.0;
			float total_conflict_score = 0.0;
			float totalHandoffTaskScore = 0.0;

			Score::ScoreLogger::instance()->GetTotalScoreForTrial(
				trial_name.c_str(),
				total_acceptance_task_score, 
				total_conflict_score,
				totalHandoffTaskScore);

			results << (total_acceptance_task_score + total_conflict_score + totalHandoffTaskScore) << ",";

			write(results.str().c_str());
			count++;
		}
	}

	// Write Intervention results details
	write("");
	write("Intervene Time,Intervention Time (HP),Intervene CS,Level,Missed");
	count = 0;

	for (std::vector<InterventionDetails>::iterator it7 = _intervention_details.begin();
			it7 != _intervention_details.end(); it7++)
	{
		if (it7->script_name == trial_name)
		{
			std::stringstream results;

			results << it7->intervention_time << ","
                    << it7->intervention_timeHp << ","
					<< it7->call_sign << ","
					<< it7->flight_level << ",";

			if (count == 0)
			{
				results << (_missed_aircraft.size()/2) << ",";
				count++;
			}

			write(results.str().c_str());
		}
	}

    // Write reminder box dismiss button details
    write("");
    write("Trial,Callsign,Dismiss Time,Dismiss Time HP");

    for (std::vector<ReminderBoxDismissDetails>::iterator it8 = m_reminder_box_dismiss_details.begin();
            it8 != m_reminder_box_dismiss_details.end(); it8++)
    {
        if (it8->m_script_name == trial_name)
        {
            std::stringstream results;

            results << it8->m_script_name    << ","
                    << it8->m_callsign       << ","
                    << it8->m_dismiss_time   << ","
                    << it8->m_dismiss_timeHp;

            write(results.str().c_str());
        }
    }

    // Write Interruption task details
    write("");
    write("Trial,Interruption Type,Start Time,End Time,Show Full Screen,External Program,Auto Accept,Auto Handoff");

    for (std::vector<InterruptionTaskDetails>::iterator it9 = m_interruption_task_details.begin();
            it9 != m_interruption_task_details.end(); it9++)
    {
        if (it9->m_script_name == trial_name)
        {
            std::stringstream results;

            results << it9->m_script_name       << ","
                    << it9->m_interruption_type << ","
                    << it9->m_start_time        << ","
                    << it9->m_end_time          << ","
                    << it9->m_show_full_screen  << ","
                    << it9->m_external_program  << ","
                    << it9->m_auto_accept       << ","
                    << it9->m_auto_handoff;

            write(results.str().c_str());
        }
    }

    // Write NBack Task responses
    write("");
    write("Trial,Stimuli Number,Stimuli,Response");

    for (std::vector<NBackTaskResponse>::iterator it10 = m_nback_task_responses.begin();
            it10 != m_nback_task_responses.end(); it10++)
    {
        std::stringstream results;

        results << it10->m_script_name      << ","
                << it10->m_stimuli_number   << ","
                << it10->m_stimuli          << ","
                << it10->m_response;

        write(results.str().c_str());
    }

	write("");
}

void 
ExcelLogger::WriteTrialScores()
{
	std::stringstream heading;
    int total_score = 0;

	heading << "Trial,Total Score";

	ExcelLogging::ExcelLogger::instance()->write(heading.str().c_str());

    for (TrialScores::iterator it = m_trialScores.begin(); 
            it != m_trialScores.end(); it++)
	{
        std::stringstream results;

		results << it->first	<< "," 
				<< it->second; 

        total_score = total_score + it->second;
		write(results.str().c_str());
	}
    
    std::stringstream total;
    total << "Total" << "," << total_score;
    write(total.str().c_str());
}

//------------------------------------------------------------------------------
// [private] implementation
//
std::string ExcelLogger::compose(const logging::KVPairs& kvpairs)const
{
	std::ostringstream oss;
	logging::KVPairsCIt kv = kvpairs.begin();
	for( ; kv != kvpairs.end(); ++kv){
		oss 
			<< "<" << kv->first << ">"
			<< kv->second
			<< "</" << kv->first << ">";
	}
	return oss.str();
}

void ExcelLogger::write(const char *tag, const char * msg)const{
	write(QString("%1,%2")
			.arg(tag)
			.arg(msg)
			.latin1());
}

void ExcelLogger::write(const char *msg)const
{
	QTextStream os(&_file);
	os << msg << "\n";

}

void ExcelLogger::add_keyvalue( const std::string &key, const std::string &val ) {
	_osstream << "<" << key << ">" << val << "</" << key << ">";
}

void ExcelLogger::add_keyvalue( const std::string &key, const double &val ) {
	_osstream << "<" << key << ">" << val << "</" << key << ">";
}

int
ExcelLogger::GetMaxKeyPressesForAcceptanceTask()
{
	int maxKeyPresses = 0;

	for (std::map<std::string, FlashDetails>::iterator it = m_acceptanceDetails.begin();
			it != m_acceptanceDetails.end(); it++)
	{
		if (it->second.keysPressed.size() > maxKeyPresses)
		{
			maxKeyPresses = it->second.keysPressed.size();
		}			
	}

	return maxKeyPresses;
}

int
ExcelLogger::GetMaxKeyPressesForHandoffTask()
{
	int maxKeyPresses = 0;

	for (std::map<std::string, FlashDetails>::iterator it = m_handoffDetails.begin();
			it != m_handoffDetails.end(); it++)
	{
		if (it->second.keysPressed.size() > maxKeyPresses)
		{
			maxKeyPresses = it->second.keysPressed.size();
		}			
	}

	return maxKeyPresses;
}

int
ExcelLogger::GetMaxKeyPressesForAllTasks()
{
	int maxAcceptanceTaskKeyPresses = GetMaxKeyPressesForAcceptanceTask();
	int maxHandoffTaskKeyPresses = GetMaxKeyPressesForHandoffTask();

	return (maxAcceptanceTaskKeyPresses > maxHandoffTaskKeyPresses) ? 
			maxAcceptanceTaskKeyPresses : maxHandoffTaskKeyPresses;
}