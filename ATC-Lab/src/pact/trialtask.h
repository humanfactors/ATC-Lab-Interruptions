/**
 * @file:       atc_trialtask.h
 * @file:       atc_trialtask.h
 *
 * 		        ATC-Lab Presentation Module
 *              Trial Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 * $Id: trialtask.h,v 1.10.2.11 2015/08/19 11:33:57 Aaron Yeung Exp $
 * $Id: trialtask.h,v 1.10.2.11 2015/08/19 11:33:57 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_TRIALTASK_HDR__
#define __ATC_TRIALTASK_HDR__


#ifdef WIN32
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif

#include "atc.h"
#include "task.h"
#include "clock.h"
#include <qsound.h>

namespace atc{

	class Engine;

};


namespace pact{

	/*!
	 *
	 */

    struct TrialInfoBoxParams
    {
        TrialInfoBoxParams()
            :   m_x(0),
                m_y(0),
                m_font("sans-serif"),
                m_font_size(50),
                m_font_colour("white"),
                m_text(""),
                m_border(false)
        {
        }

        TrialInfoBoxParams(const TrialInfoBoxParams& a_rhs)
            :   m_x(a_rhs.m_x),
                m_y(a_rhs.m_y),
                m_font(a_rhs.m_font),
                m_font_size(a_rhs.m_font_size),
                m_font_colour(a_rhs.m_font_colour),
                m_text(a_rhs.m_text),
                m_border(a_rhs.m_border)
        {
        }
        
        int         m_x;
        int         m_y;
        std::string m_font;
        int         m_font_size;
        std::string m_font_colour;
        std::string m_text;
        bool        m_border;
    };

    struct InitialDisplayParams
    {
        InitialDisplayParams()
            :   m_symbol("+"),
                m_font("sans-serif"),
                m_font_size(50),
                m_font_colour("white"),
                m_duration_ms(0),
                m_show_timer(true),
                m_x(0),
                m_y(0)
        {
        }

        InitialDisplayParams(const InitialDisplayParams& a_rhs)
            :   m_symbol(a_rhs.m_symbol),
                m_font(a_rhs.m_font),
                m_font_size(a_rhs.m_font_size),
                m_font_colour(a_rhs.m_font_colour),
                m_duration_ms(a_rhs.m_duration_ms),
                m_show_timer(a_rhs.m_show_timer),
                m_x(a_rhs.m_x),
                m_y(a_rhs.m_y)
        {
        }

        std::string m_symbol;
        std::string m_font;
        int         m_font_size;
        std::string m_font_colour;
        int         m_duration_ms;
        bool        m_show_timer;
        int         m_x;
        int         m_y;
    };

    struct InterruptionParams
    {
        InterruptionParams()
            :   m_start(atc::INVALID_VALUE),
                m_end(atc::INVALID_VALUE),
                m_auto_handoff(false),
                m_auto_accept(false),
                m_show_blank_screen(false),
                m_show_timer(false),
                m_external_program(""),
                m_initial_display()
        {
        }

        InterruptionParams(const InterruptionParams& a_rhs)
            :   m_start(a_rhs.m_start),
                m_end(a_rhs.m_end),
                m_auto_handoff(a_rhs.m_auto_handoff),
                m_auto_accept(a_rhs.m_auto_accept),
                m_show_blank_screen(a_rhs.m_show_blank_screen),
                m_show_timer(a_rhs.m_show_timer),
                m_external_program(a_rhs.m_external_program),
                m_initial_display(a_rhs.m_initial_display)
        {
        }

        int                     m_start;
        int                     m_end;
        bool                    m_auto_handoff;
        bool                    m_auto_accept;
        bool                    m_show_blank_screen;
        bool                    m_show_timer;
        std::string             m_external_program;
        InitialDisplayParams    m_initial_display;
    };

    struct nBackStimuliParams
    {
        nBackStimuliParams()
            :   m_font("sans-serif"),
                m_font_size(50),
                m_font_colour("white"),
                m_display_duration_ms(1000),
                m_show_timer(true),
                m_x(0),
                m_y(0)
        {
        }

        nBackStimuliParams(const nBackStimuliParams& a_rhs)
            :   m_font(a_rhs.m_font),
                m_font_size(a_rhs.m_font_size),
                m_font_colour(a_rhs.m_font_colour),
                m_display_duration_ms(a_rhs.m_display_duration_ms),
                m_show_timer(a_rhs.m_show_timer),
                m_x(a_rhs.m_x),
                m_y(a_rhs.m_y)
        {
        }

        std::string m_font;
        int         m_font_size;
        std::string m_font_colour;
        int         m_display_duration_ms;
        bool        m_show_timer;
        int         m_x;
        int         m_y;
    };

    struct nBackFeedbackParams
    {
        nBackFeedbackParams()
            :   m_symbol("circle"),
                m_colour("white"),
                m_width(15),
                m_height(15),
                m_x(0),
                m_y(0)
        {
        }

        nBackFeedbackParams(const nBackFeedbackParams& a_rhs)
            :   m_symbol(a_rhs.m_symbol),
                m_colour(a_rhs.m_colour),
                m_width(a_rhs.m_width),
                m_height(a_rhs.m_height),
                m_x(a_rhs.m_x),
                m_y(a_rhs.m_y)
        {
        }

        std::string m_symbol;
        std::string m_colour;
        int         m_width;
        int         m_height;
        int         m_x;
        int         m_y;
    };

    struct nBackTimerParams
    {
        nBackTimerParams()
            :   m_font("sans-serif"),
                m_font_size(20),
                m_font_colour("white"),
                m_x(0),
                m_y(0)
        {
        }

        nBackTimerParams(const nBackTimerParams& a_rhs)
            :   m_font(a_rhs.m_font),
                m_font_size(a_rhs.m_font_size),
                m_font_colour(a_rhs.m_font_colour),
                m_x(a_rhs.m_x),
                m_y(a_rhs.m_y)
        {
        }

        std::string m_font;
        int         m_font_size;
        std::string m_font_colour;
        int         m_x;
        int         m_y;
    };

    struct nBackTaskParams
    {
        nBackTaskParams()
            :   m_initial_display(),
                m_stimuli(),
                m_correct_feedback(),
                m_incorrect_feedback(),
                m_timer(),
                m_start(atc::INVALID_VALUE),
                m_end(atc::INVALID_VALUE),
                m_auto_handoff(false),
                m_auto_accept(false),
                m_background_colour("black"),
                m_response_key("space"),
                m_n_factor(2),
                m_repetition_probability(40),
                m_show_task(true)
        {
        }

        nBackTaskParams(const nBackTaskParams& a_rhs)
            :   m_initial_display(a_rhs.m_initial_display),
                m_stimuli(a_rhs.m_stimuli),
                m_correct_feedback(a_rhs.m_correct_feedback),
                m_incorrect_feedback(a_rhs.m_incorrect_feedback),
                m_timer(a_rhs.m_timer),
                m_start(a_rhs.m_start),
                m_end(a_rhs.m_end),
                m_auto_handoff(a_rhs.m_auto_handoff),
                m_auto_accept(a_rhs.m_auto_accept),
                m_background_colour(a_rhs.m_background_colour),
                m_response_key(a_rhs.m_response_key),
                m_n_factor(a_rhs.m_n_factor),
                m_repetition_probability(a_rhs.m_repetition_probability),
                m_show_task(a_rhs.m_show_task)
        {
        }

        InitialDisplayParams    m_initial_display;
        nBackStimuliParams      m_stimuli;
        nBackFeedbackParams     m_correct_feedback;
        nBackFeedbackParams     m_incorrect_feedback;
        nBackTimerParams        m_timer;

        int         m_start;
        int         m_end;
        bool        m_auto_handoff;
        bool        m_auto_accept;
        std::string m_background_colour;
        std::string m_response_key;
        int         m_n_factor;
        int         m_repetition_probability;
        bool        m_show_task;
    };


	/*!
	 * \class pact::TrialTask
	 */
	class TrialTask : public pact::Task
	{
		Q_OBJECT

	public:

		static const char* MAIN_TARGET;

	public:

        TrialTask(const std::string& a_soundFile = "");
		virtual ~TrialTask();

	public:

		atc::Engine * engine(){ return _engine; };

		void initialize();
		void SetTrialName(std::string trial_name);
		void SetTimeEvent(unsigned int timeout);
		void SetUIClock(unsigned int interval);
        void SetnBackTaskParams(nBackTaskParams a_params);
        void SetInterruptParams(InterruptionParams a_params);
        void SetNotifyMissedAcceptance(bool a_notify);
        void SetNotifyMissedHandoff(bool a_notify);
		void SetDisableCalloutRotation(bool a_disable = false);
        void ShowClock(bool a_show = true);

	public slots:
		void pause();
		void resume();
		void HandleTrialTimeout(long elapsed_time);
		void UIClockStart();
		void HandleUIClockTick();
		void HandleTimeEvent(int);
		void HandleAircraftFailedAcceptanceTask(const char*);
		void HandleAircraftSucceededAcceptanceTask(const char* callsign);
		void HandleAircraftFailedHandoffTask(const char*);
		void HandleAircraftSucceededHandoffTask(const char* callsign);
		void HandleAircraftConflictResolved(std::string, std::string, int, int, int);
		void HandleAircraftConflictFalseAlarm();
		void HandleAircraftConflicted(std::pair<std::string, std::string>);

	protected: // virtual
		virtual void keyPressEvent(QKeyEvent*);
		virtual void showEvent(QShowEvent *);

	signals:

		void started();
		void conflicts();
		void TimeEvent1();
		void TimeEvent2();
		void TimeEvent3();
		void TimeEvent4();
		void UIClockTick();
		void UpdateScores(float, float, float);
		void paused();

	private:
		
		atc::Engine *_engine;
		std::vector<unsigned int> _time_events;
		std::auto_ptr<atc::Clock> _ui_clock;
		std::string _trial_name;
        QString m_soundFile;
        QSound* m_sound;
	};

};


#endif
