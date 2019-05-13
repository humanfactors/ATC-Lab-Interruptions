#include "ReminderBox.h"
#include "ExcelLogger.h"
#include "timer.h"
#include "qapplication.h"
#include <qwmatrix.h>
#include "canvasview.h"

using namespace atc;

ReminderBox::ReminderBox(QCanvas* a_canvas)
	:	m_flashColorIndex(0),
        m_dismissButtonDialog(NULL),
        m_dismissButton(NULL),
        m_canvasView(NULL),
        m_aircraft(NULL),
        m_current_time(0)
{
	SetDefaults(a_canvas);
}

ReminderBox::ReminderBox(	QCanvas*					a_canvas, 
							const ReminderBoxParams&	a_params,
                            std::string                 a_aircraft)
	:	m_flashColorIndex(0), 
		m_params(a_params),
        m_dismissButtonDialog(NULL),
        m_dismissButton(NULL),
        m_canvasView(NULL),
        m_aircraft(a_aircraft),
        m_current_time(0)
{
	SetDefaults(a_canvas,
				m_params.m_text,
				m_params.m_textSize,
				m_params.m_BGColor,
				m_params.m_width,
				m_params.m_height,
				m_params.m_posX,
				m_params.m_posY,
                INVALID_VALUE
                );
}

ReminderBox::~ReminderBox()
{
}

void
ReminderBox::SetDefaults(QCanvas*			a_canvas, 
							std::string		a_text, 
							int				a_textSize,
							QColor			a_BGColor,
							int				a_width,
							int				a_height,
							double			a_posX, 
							double			a_posY,
							int				a_displayTime)
{
	m_canvasText = std::auto_ptr<QCanvasText>(new QCanvasText(a_canvas));
	m_canvasText->setColor(REMINDER_BOX_DEFAULT_TEXT_COLOR);	
	m_canvasText->setFont(QFont(REMINDER_BOX_DEFAULT_FONT, 
								a_textSize, 
								QFont::Bold));
	
	std::vector<std::string> tokens;
	Utils::Utilities::instance()->Tokenize(a_text, tokens, "/n");
	std::string displayText = Utils::Utilities::instance()->GetDisplayText(tokens);
	m_canvasText->setText(displayText);
	m_canvasText->move(a_posX, a_posY);
	m_canvasText->setZ(2);
    
	m_flashTimer = std::auto_ptr<QTimer>(new QTimer());
	connect(m_flashTimer.get(), SIGNAL(timeout()), this, SLOT(Flash()));
    
	m_canvasRect = 
		std::auto_ptr<QCanvasRectangle>(new QCanvasRectangle(m_canvasText->boundingRect(), 
																a_canvas));
	m_canvasRect->setSize(a_width, a_height);
	m_canvasRect->setPen(QPen(REMINDER_BOX_DEFAULT_TEXT_COLOR));
	m_params.m_BGColor = a_BGColor;
	m_canvasRect->setBrush(m_params.m_BGColor);

	// If display time is set initially, 
	// assume that the reminder box has not end display time
	if (a_displayTime >= 0)
	{
		SetDisplayTime(a_displayTime, INVALID_VALUE);
	}
}

void
ReminderBox::Show(bool a_show)
{
	m_canvasText->setVisible(a_show);
	
	if (a_show)
	{
        if (m_dismissButtonDialog.get())
        {
		    m_dismissButtonDialog->show();
        }

        m_canvasRect->show();
	}
	else
	{
        if (m_dismissButtonDialog.get())
        {
            m_dismissButtonDialog->hide();
        }

		m_canvasRect->hide();
	}
}

void
ReminderBox::Hide()
{
    ExcelLogging::ExcelLogger::instance()->LogReminderDismissTime(  m_aircraft, 
                                                                    m_current_time,
                                                                    Timer::instance()->elapsed_time());

    KeyValues kvs;
    kvs.push_back(std::make_pair(std::string("reminder_box"), m_aircraft));
    kvs.push_back(std::make_pair(std::string("dismiss_button"), std::string("pressed")));
    kvs.push_back(std::make_pair(std::string("clicked_time"), to_string(Timer::instance()->elapsed_time())));

	emit event_recorded(kvs);

    Show(false);
}

bool
ReminderBox::IsTimeToDisplay(const long& a_timeSecs) const
{
	bool ret = false;

	for (DisplayPeriods::const_iterator it = m_params.m_displayTimes.begin(); 
			it != m_params.m_displayTimes.end(); it++)
	{
		if (it->first == a_timeSecs)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool
ReminderBox::IsTimeToHide(const long& a_timeSecs) const
{
	bool ret = false;

	for (DisplayPeriods::const_iterator it = m_params.m_displayTimes.begin(); 
			it != m_params.m_displayTimes.end(); it++)
	{
		if (it->second == a_timeSecs)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool 
ReminderBox::IsTimeToEnableDismissButton(const long& a_timeSec) const
{
    bool ret = false;

    if (a_timeSec >= m_params.m_dismissButtonEnableTime)
    {
        if (m_dismissButton.get())
        {
            if (!m_dismissButton->isEnabled())
            {
                ret = true;
            }
        }
    }

    return ret;
}

void
ReminderBox::SetCanvasView(CanvasView* a_canvasView)
{
    m_canvasView = a_canvasView;

    if (m_params.m_dismissButtonEnableTime > INVALID_VALUE)
    {
        m_dismissButtonDialog = std::auto_ptr<QDialog>(
            new QDialog((QWidget*) m_canvasView,
                        "DismissButton",
                        FALSE,
                        Qt::WStyle_Customize|Qt::WStyle_NoBorder));

        m_dismissButtonDialog->resize(  m_params.m_dismissButtonWidth, 
                                        m_params.m_dismissButtonHeight);

        m_dismissButtonDialog->setFocusPolicy(QWidget::NoFocus);

        m_dismissButton = std::auto_ptr<QPushButton>(
            new QPushButton(QString(m_params.m_dismissButtonLabel), 
							dynamic_cast<QWidget*>(m_dismissButtonDialog.get()), 
							m_params.m_dismissButtonLabel.c_str()));
                                                
        m_dismissButton->resize(  m_params.m_dismissButtonWidth, 
                                  m_params.m_dismissButtonHeight);

        QFont font = m_dismissButton->font();
        font.setPointSize(m_params.m_dismissButtonFontSize);
        m_dismissButton->setFont(font);

        m_dismissButton->setEnabled(false);
        m_dismissButton->setFocusPolicy(QWidget::NoFocus);

        connect(m_dismissButton.get(), SIGNAL(clicked()), this, SLOT(Hide()));

        m_dismissButtonDialog->hide();
        
        // Work around for reminder boxes that are displayed at time = 0
        // When this occurs, m_canvas has not been set yet so when Show is called at time 0,
        // the m_dismissButtonDialog does not get shown. When this procedure then runs after
        // and hence never gets to be called in the Show() procedure
        for (DisplayPeriods::const_iterator it = m_params.m_displayTimes.begin(); 
			it != m_params.m_displayTimes.end(); it++)
	    {
		    if (it->first == 0)
		    {
                m_dismissButtonDialog->show();
            }
        }
        
        m_canvasView->grabKeyboard();
    }
}

void
ReminderBox::SetDisplayTime(int a_displayStartTime,
							int a_displayEndTime)
{
	m_params.m_displayTimes.push_back(std::make_pair<int, int>
							(a_displayStartTime, a_displayEndTime));
}

void 
ReminderBox::SetFlashColor(std::string a_flashColor)
{
	m_params.m_flashColors.push_back(QColor(a_flashColor));
}

void
ReminderBox::SetFlashTime(unsigned int a_flashStartTime,
						  unsigned int a_flashEndTime)
{
	m_params.m_flashPeriods.push_back(std::make_pair<unsigned int, unsigned int>
							(a_flashStartTime, a_flashEndTime));
}

void 
ReminderBox::StartFlash()
{
	m_flashTimer->start(REMINDER_BOX_FLASH_INTERVAL_MS);
}

bool
ReminderBox::StartFlash(unsigned int a_time)
{
	bool start = false;

	for (FlashPeriods::const_iterator it = m_params.m_flashPeriods.begin();
			it != m_params.m_flashPeriods.end(); it++)
	{
		if (it->first == a_time)
		{
			start = true;
			break;
		}
	}

	return start;
}

void 
ReminderBox::StopFlash()
{
	m_flashTimer->stop();
	m_canvasRect->setBrush(m_params.m_BGColor);
}

bool
ReminderBox::StopFlash(unsigned int a_time)
{
	bool stop = false;

	for (FlashPeriods::const_iterator it = m_params.m_flashPeriods.begin();
			it != m_params.m_flashPeriods.end(); it++)
	{
		if (it->second == a_time)
		{
			stop = true;
			break;
		}
	}

	return stop;
}

void
ReminderBox::Flash()
{
	QColor newColor = REMINDER_BOX_DEFAULT_FLASH_COLOR;
	
	if (m_params.m_flashColors.size() != 0)
	{
		m_flashColorIndex = m_flashColorIndex++ % m_params.m_flashColors.size();

		newColor = m_params.m_flashColors[m_flashColorIndex];
	}

	m_canvasRect->setBrush(newColor);
}

void
ReminderBox::Disable()
{
	bool hide = false;

	// If the reminder box has no end time, just stop it from flashing
	// otherwise hide it
	for (DisplayPeriods::const_iterator it = m_params.m_displayTimes.begin(); 
			it != m_params.m_displayTimes.end(); it++)
	{
		if (it->second != atc::INVALID_VALUE)
		{
			hide = true;
			break;
		}
	}

	if (hide)
	{
		Show(false);
	}
	else
	{
		StopFlash();
	}
}

void
ReminderBox::UpdateSeconds(long a_seconds)
{
    m_current_time = a_seconds;

	if (IsTimeToDisplay(a_seconds))
	{
		Show(true);
	}

	if (StartFlash(a_seconds))
	{
		StartFlash();
	}
	
	if (StopFlash(a_seconds))
	{
		StopFlash();
	}

	if (IsTimeToHide(a_seconds))
	{
		Show(false);
	}

    if (IsTimeToEnableDismissButton(a_seconds))
    {
        if (m_dismissButton.get())
        {
            m_dismissButton->setEnabled(true);
        }
    }
}

void 
ReminderBox::Move(double a_x, double a_y)
{
	m_canvasText->move(	a_x + m_params.m_posX, 
						a_y + m_params.m_posY);
	m_canvasRect->move(	a_x + m_params.m_posX, 
						a_y + m_params.m_posY);
    
    if (m_dismissButtonDialog.get())
    {
        // calc cavas view location
        int* tx = new int(0);
        int* ty = new int(0);
	    m_canvasView->worldMatrix().map(a_x, a_y, tx, ty);
	    
        m_dismissButtonDialog->move(*tx + m_params.m_dismissButtonPosX, 
                                    *ty + m_params.m_dismissButtonPosY);
    }
}