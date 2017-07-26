#ifndef _REMINDER_BOX_H_
#define _REMINDER_BOX_H_

#include "atc.h"
#include "Utilities.h"
#include "atc_types.h"
#include <memory.h>
#include <qcanvas.h>
#include <qcolor.h>
#include <qobject.h>
#include <qtimer.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <vector>
#include <string>

namespace atc
{
	const unsigned short REMINDER_BOX_FLASH_INTERVAL_MS	= 250;
	const unsigned short REMINDER_BOX_DEFAULT_HEIGHT	= 175;
	const unsigned short REMINDER_BOX_DEFAULT_WIDTH		= 450;
	const QColor REMINDER_BOX_DEFAULT_TEXT_COLOR		= Qt::black;
	const QColor REMINDER_BOX_DEFAULT_BG_COLOR			= Qt::yellow;
	const QColor REMINDER_BOX_DEFAULT_FLASH_COLOR		= Qt::yellow;
	const std::string REMINDER_BOX_DEFAULT_FONT			= "Courier New";
	const unsigned short REMINDER_BOX_DEFAULT_FONT_SIZE = 48;
	const double REMINDER_BOX_DEFAULT_POSX				= 450;
	const double REMINDER_BOX_DEFAULT_POSY				= 175;

	typedef std::pair<unsigned int, unsigned int>		FlashPeriod;
	typedef std::vector<FlashPeriod>					FlashPeriods;
	typedef std::pair<int, int>							DisplayPeriod;
	typedef std::vector<DisplayPeriod>					DisplayPeriods;

	struct ReminderBoxParams
	{
		ReminderBoxParams()
			:	m_text(""),
				m_textSize(0),
				m_BGColor(""),
				m_width(0),
				m_height(0),
				m_posX(0),
				m_posY(0),
				m_displayTimes(),
				m_flashPeriods(),
				m_flashColors(),
                m_dismissButtonEnableTime(INVALID_VALUE),
                m_dismissButtonLabel(""),
                m_dismissButtonFontSize(10),
                m_dismissButtonWidth(10),
                m_dismissButtonHeight(10),
                m_dismissButtonPosX(10),
                m_dismissButtonPosY(10)
		{
		}

		ReminderBoxParams(	const std::string&	a_text,
							int					a_textSize,
							QColor				a_BGColor,
							int					a_width,
							int					a_height,
							double				a_posX,
							double				a_posY,
							DisplayPeriods		a_displayPeriods)
			:	m_text(a_text),
				m_textSize(a_textSize),
				m_BGColor(a_BGColor),
				m_width(a_width),
				m_height(a_height),
				m_posX(a_posX),
				m_posY(a_posY),
				m_displayTimes(a_displayPeriods),
                m_dismissButtonEnableTime(INVALID_VALUE),
                m_dismissButtonLabel(""),
                m_dismissButtonFontSize(10),
                m_dismissButtonWidth(10),
                m_dismissButtonHeight(10),
                m_dismissButtonPosX(10),
                m_dismissButtonPosY(10)
		{
		}
		
		ReminderBoxParams(const ReminderBoxParams& a_rhs)
			:	m_text(a_rhs.m_text),
				m_textSize(a_rhs.m_textSize),
				m_BGColor(a_rhs.m_BGColor),
				m_width(a_rhs.m_width),
				m_height(a_rhs.m_height),
				m_posX(a_rhs.m_posX),
				m_posY(a_rhs.m_posY),
				m_displayTimes(a_rhs.m_displayTimes),
				m_flashPeriods(a_rhs.m_flashPeriods),
				m_flashColors(a_rhs.m_flashColors),
                m_dismissButtonEnableTime(a_rhs.m_dismissButtonEnableTime),
                m_dismissButtonLabel(a_rhs.m_dismissButtonLabel),
                m_dismissButtonFontSize(a_rhs.m_dismissButtonFontSize),
                m_dismissButtonWidth(a_rhs.m_dismissButtonWidth),
                m_dismissButtonHeight(a_rhs.m_dismissButtonHeight),
                m_dismissButtonPosX(a_rhs.m_dismissButtonPosX),
                m_dismissButtonPosY(a_rhs.m_dismissButtonPosY)
		{
		}

		std::string			m_text;
		int					m_textSize;
		QColor				m_BGColor;
		int					m_width;
		int					m_height;
		double				m_posX;
		double				m_posY;
		DisplayPeriods		m_displayTimes;
		FlashPeriods		m_flashPeriods;
		std::vector<QColor>	m_flashColors;
        int                 m_dismissButtonEnableTime;
        std::string         m_dismissButtonLabel;
        unsigned int        m_dismissButtonFontSize;
        unsigned int        m_dismissButtonWidth;
        unsigned int        m_dismissButtonHeight;
        unsigned int        m_dismissButtonPosX;
        unsigned int        m_dismissButtonPosY;
	};

    class CanvasView;

	class ReminderBox : public QObject
	{
		Q_OBJECT

	public:
		ReminderBox(QCanvas* a_canvas);
		ReminderBox::ReminderBox(	QCanvas*					a_canvas, 
									const ReminderBoxParams&	a_params,
                                    std::string                 a_aircraft = "");

		~ReminderBox();

		void Show(bool a_show);
		void SetDisplayTime(	int a_displayStartTime,
								int a_displayEndTime);
		void SetFlashColor(		std::string a_flashColor);
		void SetFlashTime(		unsigned int a_flashStartTime,
								unsigned int a_flashEndTime);
        
        void SetCanvasView(CanvasView* a_canvasView);

		bool IsTimeToDisplay(const long& a_timeSec) const;
		bool IsTimeToHide(const long& a_timeSec) const;
		void StartFlash();
		bool StartFlash(unsigned int a_seconds);
		void StopFlash();
		bool StopFlash(unsigned int a_seconds);
		void Disable();
		void Move(double a_x, double a_y);

    signals:
        void event_recorded(KeyValues &);

	public slots:
		void UpdateSeconds(long a_seconds);
        void Hide();

	private:
		void SetDefaults(QCanvas*			a_canvas,
							std::string		a_text = "", 
							int				a_textSize = REMINDER_BOX_DEFAULT_FONT_SIZE,
							QColor			a_BGColor = REMINDER_BOX_DEFAULT_BG_COLOR,
							int				a_width = REMINDER_BOX_DEFAULT_WIDTH,
							int				a_height = REMINDER_BOX_DEFAULT_HEIGHT,
							double			a_posX = REMINDER_BOX_DEFAULT_POSX, 
							double			a_posY = REMINDER_BOX_DEFAULT_POSY,
							int				a_displayTime = INVALID_VALUE);

        bool IsTimeToEnableDismissButton(const long& a_timeSec) const;

        CanvasView*                     m_canvasView;
		std::auto_ptr<QCanvasText>		m_canvasText;
		std::auto_ptr<QCanvasRectangle> m_canvasRect;
		std::auto_ptr<QTimer>			m_flashTimer;
        std::auto_ptr<QDialog>          m_dismissButtonDialog;
        std::auto_ptr<QPushButton>      m_dismissButton;
		unsigned short					m_flashColorIndex;
		ReminderBoxParams				m_params;
        std::string                     m_aircraft;
        long                            m_current_time;

	private slots:
		void Flash();
	};
};

#endif //_REMINDER_BOX_H_