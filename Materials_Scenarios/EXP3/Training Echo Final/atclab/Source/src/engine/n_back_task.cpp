#include "n_back_task.h"
#include "atc_types.h"
#include "ExcelLogger.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <qnamespace.h>
#include <qlayout.h>
#include <qpalette.h>
#include <qpainter.h>

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// Class nBackTask
//
//------------------------------------------------------------------------------
// [public] construction/destruction
//
nBackTask::nBackTask(CanvasView* a_canvasView,
                     pact::nBackTaskParams a_params)
    :   QDialog((QWidget*) a_canvasView, "nBackTask", TRUE, Qt::WStyle_Customize|Qt::WStyle_NoBorder),
        m_params(a_params),
        m_stimuliCount(0),
        m_correct(false),
        m_showFeedback(false),
        m_responded(false),
        m_state(INITIAL_DISPLAY),
        m_countDown(0),
        m_canvasView(a_canvasView)
{
    m_stimuliDisplay = std::auto_ptr<QLabel>(new QLabel(this));
    m_secondsDisplay = std::auto_ptr<QLabel>(new QLabel(this));
    m_stimuliTimer = std::auto_ptr<QTimer>(new QTimer(this));

    connect(m_stimuliTimer.get(), SIGNAL(timeout()), 
			this, SLOT(HandleStimuliTimeout()));

    ConstructTask();
}

nBackTask::~nBackTask()
{
}

void
nBackTask::Start()
{
    showFullScreen();
    this->grabKeyboard();
	m_stimuliTimer->start(DEFAULT_TIMER_INTERVAL_MS);
    m_countDown = m_params.m_initial_display.m_duration_ms;
    m_stimuliCount = 0;
    m_stimuliList.clear();
    m_correct = false;
    m_showFeedback = false;
    m_responded = false;
    m_state = INITIAL_DISPLAY;

    DisplayTime();
}

void
nBackTask::Stop()
{
    hide();
    this->releaseKeyboard();
    m_stimuliTimer->stop();

    // Record stimuli responses
    for (int i = 0; i < m_stimuliList.size(); i++)
    {
        KeyValues kvs;

        kvs.push_back(std::make_pair(std::string("NBackTask_response" ), to_string(i + 1)));
        kvs.push_back(std::make_pair(std::string("stimuli" ), to_string(m_stimuliList[i].m_stimuli)));
        
        std::string response = "";

        switch (m_stimuliList[i].m_response)
        {
        case HIT:
            response = "hit";
            break;
        case MISS:
            response = "miss";
            break;
        case FALSE_ALARM:
            response = "false_alarm";
            break;
        case CORRECT_REJECTION:
            response = "correct_rejection";
            break;
        case INVALID_RESPONSE:
        default:
            response = "invalid";
            break;
        }

        kvs.push_back(std::make_pair(std::string("response" ), response));
        
        emit event_recorded(kvs);

        ExcelLogging::ExcelLogger::instance()->LogNBackTaskResponse(m_stimuliList[i].m_stimuli, (i + 1), response);
    }
}

void
nBackTask::HandleStimuliTimeout()
{
    m_countDown = m_countDown - DEFAULT_TIMER_INTERVAL_MS;

    if (m_state == INITIAL_DISPLAY)
    {
        if (m_countDown == 0)
        {
            m_state = STIMULI_ACTIVE;
            m_countDown = m_params.m_stimuli.m_display_duration_ms;

            DisplayTime();

            m_showFeedback = false;
            update();

            int current_stimuli = NextIntegerStimuli();

            SetupStimuliDisplay(m_params.m_stimuli.m_font,
                                m_params.m_stimuli.m_font_size,
                                m_params.m_stimuli.m_font_colour,
                                atc::to_string(current_stimuli),
                                m_params.m_background_colour,
                                m_params.m_stimuli.m_x,
                                m_params.m_stimuli.m_y);
        }
        else
        {
            DisplayTime();
        }
    }
    else if (m_state == STIMULI_ACTIVE)
    {
        if (m_countDown == 0)
        {
            m_countDown = m_params.m_stimuli.m_display_duration_ms;
            m_showFeedback = false;
            update();

            int current_stimuli = NextIntegerStimuli();

            m_stimuliDisplay->setText(atc::to_string(current_stimuli));
            m_stimuliDisplay->adjustSize();
        }
        
        DisplayTime();     
    }
    
    m_stimuliTimer->start(DEFAULT_TIMER_INTERVAL_MS);
}

void
nBackTask::ConstructTask()
{
    QColor bg_color = Utils::Utilities::instance()->GetQColorFromString(m_params.m_background_colour);

    setBackgroundColor(bg_color);

    SetupStimuliDisplay(m_params.m_initial_display.m_font,
                        m_params.m_initial_display.m_font_size,
                        m_params.m_initial_display.m_font_colour,
                        m_params.m_initial_display.m_symbol,
                        m_params.m_background_colour,
                        m_params.m_initial_display.m_x,
                        m_params.m_initial_display.m_y);

    SetupTimerDisplay();

    // Initilize random seed
    srand(time(NULL));
}

int
nBackTask::NextIntegerStimuli()
{
    int next_stimuli = atc::INVALID_VALUE;
    StimuliResponse sr;

    m_stimuliCount++;

    // Determine if this should be an N factor stimuli (ie. repeat the number N times ago)
    if (m_stimuliCount > m_params.m_n_factor)
    {
        if ((rand() % (ONE_HUNDRED_PERCENT + 1)) < m_params.m_repetition_probability)
        {
            next_stimuli = m_stimuliList[(m_stimuliCount - m_params.m_n_factor) - 1].m_stimuli;

            // This is a target stimuli. By default, if there is no user response,
            // then it is a MISS
            sr.m_stimuliType = STIMULI_TYPE_TARGET;
            sr.m_response = MISS;
        }
        else
        {
            // Select a random number that hasn't been selected in the last N factor times
            bool done = false;
            bool repeat_number = false;

            while (!done)
            {
                next_stimuli = rand() % NUMBERS_ONE_TO_TEN;

                repeat_number = false;
                for (int i = 1; i <= m_params.m_n_factor; i++)
                {
                    if (next_stimuli == m_stimuliList[(m_stimuliCount - 1) - i].m_stimuli)
                    {
                        repeat_number = true;
                    }
                }

                if (!repeat_number)
                {
                    done = true;
                }
            }

            // This is a non-target stimuli. By default, if there is no user response,
            // then it is a correct rejection
            sr.m_stimuliType = STIMULI_TYPE_NON_TARGET;
            sr.m_response = CORRECT_REJECTION;
        }
    }
    else
    {
        // Non target stimuli

        if (m_stimuliCount == 1)
        {
            next_stimuli = rand() % NUMBERS_ONE_TO_TEN;
        }
        else
        {
            // Same stimuli values consecutively presented is not allowed
            bool repeat_number = true;
            while (repeat_number)
            {
                next_stimuli = rand() % NUMBERS_ONE_TO_TEN;

                if (next_stimuli != m_stimuliList[(m_stimuliCount - 1) - 1].m_stimuli)
                {
                    repeat_number = false;
                }
            }

        }

        // The first N numbers are not targets. By default, if there is no user response,
        // it is a correct rejection
        sr.m_stimuliType = STIMULI_TYPE_NON_TARGET;
        sr.m_response = CORRECT_REJECTION;
    }

    sr.m_stimuli = next_stimuli;

    m_stimuliList.push_back(sr);
    m_responded = false;

    return m_stimuliList[m_stimuliCount - 1].m_stimuli;
}

void
nBackTask::SetupStimuliDisplay( std::string a_font,
                                int         a_font_size,
                                std::string a_font_colour,
                                std::string a_stimuli,
                                std::string a_background_colour,
                                int         a_x,
                                int         a_y)
{
	QFont f(a_font.c_str(),
            a_font_size);

	m_stimuliDisplay->setFont(f);

    QPalette* palette = new QPalette();
    QColor stimuli_colour = Utils::Utilities::instance()->GetQColorFromString(a_font_colour);

    // Set background colour
    QColor bg_color = Utils::Utilities::instance()->GetQColorFromString(a_background_colour);

    palette->setColor(QColorGroup::Background, bg_color);
    palette->setColor(QColorGroup::Foreground, stimuli_colour);
    
    m_stimuliDisplay->setPalette(*palette);
    m_stimuliDisplay->setText(a_stimuli);
    m_stimuliDisplay->adjustSize();
    m_stimuliDisplay->move(a_x, a_y);
}

void
nBackTask::SetupTimerDisplay()
{
    QFont f(m_params.m_timer.m_font, m_params.m_timer.m_font_size);

	m_secondsDisplay->setFont(f);

    QPalette* palette = new QPalette();

    QColor display_colour = Utils::Utilities::instance()->GetQColorFromString(m_params.m_timer.m_font_colour);
    QColor bg_color = Utils::Utilities::instance()->GetQColorFromString(m_params.m_background_colour);

    palette->setColor(QColorGroup::Background, bg_color);
    palette->setColor(QColorGroup::Foreground, display_colour);
    m_secondsDisplay->setPalette(*palette);
    m_secondsDisplay->move(m_params.m_timer.m_x, m_params.m_timer.m_y);
}

void
nBackTask::DisplayTime()
{
    bool show_timer = true;

    switch (m_state)
    {
    case INITIAL_DISPLAY:
        show_timer = m_params.m_initial_display.m_show_timer;
        break;
    case STIMULI_ACTIVE:
    default:
        show_timer = m_params.m_stimuli.m_show_timer;
        break;
    }

    if (show_timer)
    {
        char* time_display = "";
        float time_remaining = (float) ((float) m_countDown / (float) MILLISECONDS_PER_SECOND);
        sprintf(time_display, "%2.1f", time_remaining);
        m_secondsDisplay->setText("Time: " + std::string(time_display));
        m_secondsDisplay->adjustSize();
    }
}

void 
nBackTask::keyPressEvent(QKeyEvent* e) 
{
    try
	{	
        int key = e->key();
		if (e->state() & Qt::ShiftButton)   key += Qt::SHIFT;
		if (e->state() & Qt::ControlButton) key += Qt::CTRL;
		if (e->state() & Qt::AltButton)     key += Qt::ALT;
		if (e->state() & Qt::MetaButton)    key += Qt::META;
		
		QKeySequence key_sequence(key);

		if (key_sequence == QKeySequence("Alt+Ctrl+Q"))
        {
			Stop();
		}
	    else if (e->state() == Qt::NoButton) 
	    {
            int key_pressed = e->key();
		    std::string key_pressed_char;

		    if (key_pressed == Qt::Key_F1)
		    {
			    key_pressed_char = "F1";
		    }
		    else
		    {
                if (m_state == STIMULI_ACTIVE)
                {
			        key_pressed_char = (char) key_pressed;
                    std::string response_key = "";

                    if (m_params.m_response_key == "space")
                    {
                        response_key = " ";
                    }
                    else
                    {
                        response_key = toupper(*(m_params.m_response_key.c_str()));
                    }

                    if ((key_pressed_char == response_key) && (!m_responded))
                    {
                        CalculateResponse();
                        m_responded = true;
                    }
                }
		    }
	    }

	    e->ignore();
	}
	catch (...)
	{
	}
}

void 
nBackTask::CalculateResponse() 
{
    bool correct = false;

    if (m_stimuliCount > m_params.m_n_factor)
    {
        correct = ( m_stimuliList[m_stimuliCount - 1].m_stimuli == 
                    m_stimuliList[m_stimuliCount - 1 - m_params.m_n_factor].m_stimuli);
    }

    if (correct)
    {
        m_stimuliList[m_stimuliCount - 1].m_response = HIT;
    }
    else
    {
        m_stimuliList[m_stimuliCount - 1].m_response = FALSE_ALARM;
    }

    UpdateFeedback(correct);
}

void
nBackTask::UpdateFeedback(bool a_correct)
{
    m_correct = a_correct;
    m_showFeedback = true;

    update();
}

void
nBackTask::paintEvent(QPaintEvent* a_event)
{
    if (m_showFeedback)
    {
        QPainter painter(this);
        QColor feedback_colour = Qt::red;
        std::string feedback_shape = "circle";
        int width = DEFAULT_FEEDBACK_SYMBOL_WIDTH;
        int height = DEFAULT_FEEDBACK_SYMBOL_HEIGHT;
        int x = DEFAULT_FEEDBACK_SYMBOL_X;
        int y = DEFAULT_FEEDBACK_SYMBOL_Y;

        if (m_correct)
        {
            feedback_colour = Utils::Utilities::instance()->GetQColorFromString(m_params.m_correct_feedback.m_colour);
            feedback_shape = m_params.m_correct_feedback.m_symbol;
            width = m_params.m_correct_feedback.m_width;
            height = m_params.m_correct_feedback.m_height;
            x = m_params.m_correct_feedback.m_x;
            y = m_params.m_correct_feedback.m_y;
        }
        else
        {
            feedback_colour = Utils::Utilities::instance()->GetQColorFromString(m_params.m_incorrect_feedback.m_colour);
            feedback_shape = m_params.m_incorrect_feedback.m_symbol;
            width = m_params.m_incorrect_feedback.m_width;
            height = m_params.m_incorrect_feedback.m_height;
            x = m_params.m_incorrect_feedback.m_x;
            y = m_params.m_incorrect_feedback.m_y;
        }

        QBrush brush(feedback_colour, Qt::SolidPattern);
        painter.setBrush(brush);

        if (feedback_shape == "circle")
        {
            painter.drawEllipse(x, y, width, height);
        }
        else if (feedback_shape == "square")
        {
            painter.drawRect(x, y, width, height);
        }
    }
}
