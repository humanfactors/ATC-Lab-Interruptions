#include "interrupt_task.h"
#include <string.h>
#include <qpalette.h>
#include <qpainter.h>

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// Class InterruptTask
//
//------------------------------------------------------------------------------
// [public] construction/destruction
//
InterruptTask::InterruptTask(CanvasView* a_canvasView,
                             pact::InterruptionParams a_params)
    :   QDialog((QWidget*) a_canvasView, "InterruptScreen", TRUE, Qt::WStyle_Customize|Qt::WStyle_NoBorder),
        m_params(a_params),
        m_secondsCount(0),
        m_canvasView(a_canvasView),
        m_state(INTERRUPT_INITIAL_DISPLAY)
{
    setBackgroundColor(DEFAULT_BACKGROUND_COLOUR);
    m_textDisplay = std::auto_ptr<QLabel>(new QLabel(this));
    m_countdownTimer = std::auto_ptr<QTimer>(new QTimer(this));
    
    connect(m_countdownTimer.get(), SIGNAL(timeout()), 
			this, SLOT(HandleCountdownTimerTimeout()));

    if (m_params.m_initial_display.m_duration_ms > 0)
    {
        m_state = INTERRUPT_INITIAL_DISPLAY;

        SetupDisplay(   m_params.m_initial_display.m_font,
                        m_params.m_initial_display.m_font_size,
                        m_params.m_initial_display.m_font_colour,
                        m_params.m_initial_display.m_symbol,
                        m_params.m_initial_display.m_x,
                        m_params.m_initial_display.m_y);
    }
    else
    {
        m_state = INTERRUPT_COUNT_DOWN;

        QRect rect = this->geometry();

        SetupDisplay(   DEFAULT_COUNTDOWN_TIMER_FONT,
                        DEFAULT_COUNTDOWN_TIMER_FONT_SIZE,
                        DEFAULT_COUNTDOWN_TIMER_FONT_COLOUR,
                        std::string(""),
                        rect.width()/2, 
                        rect.height()/2);
    }
}

InterruptTask::~InterruptTask()
{
}

void
InterruptTask::Start()
{
    showFullScreen();
	m_countdownTimer->start(MILLISECONDS_PER_SECOND);

    if (m_state == INTERRUPT_INITIAL_DISPLAY)
    {
        m_secondsCount = (m_params.m_initial_display.m_duration_ms / MILLISECONDS_PER_SECOND);
    }
    else
    {
        QRect rect = this->geometry();

        m_textDisplay->move(rect.width()/2, rect.height()/2);

        m_secondsCount = m_params.m_end - m_params.m_start;

        DisplayTime();
    }   
}

void
InterruptTask::Stop()
{
    hide();
    m_countdownTimer->stop();
}

void
InterruptTask::HandleCountdownTimerTimeout()
{
    m_secondsCount--;

    if (m_state == INTERRUPT_INITIAL_DISPLAY)
    {
        if (m_secondsCount == 0)
        {
            m_state = INTERRUPT_COUNT_DOWN;
            m_secondsCount = m_params.m_end - m_params.m_start - (m_params.m_initial_display.m_duration_ms / MILLISECONDS_PER_SECOND);

            QRect rect = this->geometry();
            
            if (m_params.m_show_timer)
            {
                SetupDisplay(   DEFAULT_COUNTDOWN_TIMER_FONT,
                                DEFAULT_COUNTDOWN_TIMER_FONT_SIZE,
                                DEFAULT_COUNTDOWN_TIMER_FONT_COLOUR,
                                atc::to_string(m_secondsCount),
                                rect.width()/2, 
                                rect.height()/2);
            }
            else
            {
                // Clear the initial display text
                SetupDisplay(   DEFAULT_COUNTDOWN_TIMER_FONT,
                                DEFAULT_COUNTDOWN_TIMER_FONT_SIZE,
                                DEFAULT_COUNTDOWN_TIMER_FONT_COLOUR,
                                std::string(""),
                                rect.width()/2, 
                                rect.height()/2);
            }
        }
    }
    else if (m_state == INTERRUPT_COUNT_DOWN)
    {
        DisplayTime();
    }

    m_countdownTimer->start(MILLISECONDS_PER_SECOND);
}


void
InterruptTask::DisplayTime()
{
    bool show_timer = false;

    if (m_state == INTERRUPT_INITIAL_DISPLAY)
    {
        show_timer = m_params.m_initial_display.m_show_timer;
    }
    else
    {
        show_timer = m_params.m_show_timer;
    }

    if (show_timer)
    {
        m_textDisplay->setText(atc::to_string(m_secondsCount));
        m_textDisplay->adjustSize();
    }
}

void
InterruptTask::SetupDisplay( std::string a_font,
                             int         a_font_size,
                             std::string a_font_colour,
                             std::string a_display,
                             int         a_x,
                             int         a_y)
{
	QFont f(a_font.c_str(),
            a_font_size);

	m_textDisplay->setFont(f);

    QPalette* palette = new QPalette();
    QColor display_colour = Utils::Utilities::instance()->GetQColorFromString(a_font_colour);

    palette->setColor(QColorGroup::Background, DEFAULT_BACKGROUND_COLOUR);
    palette->setColor(QColorGroup::Foreground, display_colour);
    
    m_textDisplay->setPalette(*palette);
    m_textDisplay->setText(a_display);
    m_textDisplay->adjustSize();
    m_textDisplay->move(a_x, a_y);
}