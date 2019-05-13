#ifndef __N_BACK_TASK_HDR__
#define __N_BACK_TASK_HDR__

#include "trialtask.h"
#include "canvasview.h"
#include "Utilities.h"
#include <qdialog.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qlayout.h>

namespace atc
{
    const int           DEFAULT_FEEDBACK_SYMBOL_X = 0;
    const int           DEFAULT_FEEDBACK_SYMBOL_HEIGHT = 15;
    const int           DEFAULT_FEEDBACK_SYMBOL_WIDTH = 15;
    const int           DEFAULT_FEEDBACK_SYMBOL_Y = 0;
    const int           DEFAULT_TIMER_INTERVAL_MS = 100;
    const int           NUMBERS_ONE_TO_TEN = 10;
    const int           ONE_HUNDRED_PERCENT = 100;

    enum nBackState
    {
        INITIAL_DISPLAY,
        STIMULI_ACTIVE
    };

    enum StimuliType
    {
        STIMULI_TYPE_UNKNOWN,
        STIMULI_TYPE_NON_TARGET,
        STIMULI_TYPE_TARGET
    };

    enum UserResponse
    {
        INVALID_RESPONSE,
        HIT,
        MISS,
        FALSE_ALARM,
        CORRECT_REJECTION
    };

    struct StimuliResponse
    {
        StimuliResponse()
            :   m_stimuliType(STIMULI_TYPE_UNKNOWN),
                m_stimuli(atc::INVALID_VALUE),
                m_response(INVALID_RESPONSE)
        {
        }

        StimuliResponse(const StimuliResponse& a_rhs)
            :   m_stimuliType(a_rhs.m_stimuliType),
                m_stimuli(a_rhs.m_stimuli),
                m_response(a_rhs.m_response)
        {
        }

        int             m_stimuli;
        UserResponse    m_response;
        StimuliType     m_stimuliType;
    };

    class nBackTask : public QDialog 
    {
		Q_OBJECT;

    public:
        nBackTask(CanvasView* a_canvasView, pact::nBackTaskParams a_params);
        ~nBackTask();

        void Start();
        void Stop();

    signals:
        void event_recorded(KeyValues& );
    
    protected:
        void keyPressEvent(QKeyEvent* e);
        void paintEvent(QPaintEvent* a_event);

    private slots:
        void HandleStimuliTimeout();

    private:
        void    ConstructTask();
        int     NextIntegerStimuli();
        void    SetupStimuliDisplay(    std::string a_font,
                                        int         a_font_size,
                                        std::string a_font_colour,
                                        std::string a_stimuli,
                                        std::string a_background_colour,
                                        int         a_x,
                                        int         a_y);
        void SetupTimerDisplay();
        void DisplayTime();
        void CalculateResponse();
        void UpdateFeedback(bool a_correct);

        int                             m_stimuliCount;
        int                             m_countDown;
        std::auto_ptr<QTimer>           m_stimuliTimer;
        std::auto_ptr<QLabel>           m_stimuliDisplay;
        std::auto_ptr<QLabel>           m_secondsDisplay;
        CanvasView*                     m_canvasView;
        pact::nBackTaskParams           m_params;
        std::vector<StimuliResponse>    m_stimuliList;
        bool                            m_correct;
        bool                            m_showFeedback;
        bool                            m_responded;
        nBackState                      m_state;
    };
};

#endif