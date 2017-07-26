#ifndef __INTERRUPT_TASK_HDR__
#define __INTERRUPT_TASK_HDR__

#include "canvasview.h"
#include "trialtask.h"
#include <qdialog.h>
#include <qlabel.h>
#include <qtimer.h>

namespace atc
{
    const QColor        DEFAULT_BACKGROUND_COLOUR = Qt::black;
    const std::string   DEFAULT_COUNTDOWN_TIMER_FONT = "SansSerif";
    const std::string   DEFAULT_COUNTDOWN_TIMER_FONT_COLOUR = "white";
    const int           DEFAULT_COUNTDOWN_TIMER_FONT_SIZE = 20;

    enum InterruptState
    {
        INTERRUPT_INITIAL_DISPLAY,
        INTERRUPT_COUNT_DOWN
    };

    class InterruptTask : public QDialog 
    {
		Q_OBJECT;

    public:
        InterruptTask(CanvasView* a_canvasView, pact::InterruptionParams a_params);
        ~InterruptTask();

        void Start();
        void Stop();

    private slots:
        void HandleCountdownTimerTimeout();

    private:
        void DisplayTime();
        void SetupDisplay(  std::string a_font,
                            int         a_font_size,
                            std::string a_font_colour,
                            std::string a_display,
                            int         a_x,
                            int         a_y);

        int                         m_secondsCount;
        std::auto_ptr<QTimer>       m_countdownTimer;
        std::auto_ptr<QLabel>       m_textDisplay;
        CanvasView*                 m_canvasView;
        pact::InterruptionParams    m_params;
        InterruptState              m_state;
    };
};

#endif