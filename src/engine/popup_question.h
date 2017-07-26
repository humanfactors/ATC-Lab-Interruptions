/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: popup_question.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:45 $
      @state    : $State: Exp $

      $Log: popup_question.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:45  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.1  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __POPUP_QUESTIONS_HDR__
#define __POPUP_QUESTIONS_HDR__

#include <qdialog.h>

//
// Qt
//
class QButtonGroup;


namespace atc {

	// 
	// atc::ScaledQuestion
	//
	class ScaledQuestion : public QDialog {
		Q_OBJECT

	protected:
		ScaledQuestion( QWidget * );
		~ScaledQuestion();

	protected:
		void create_buttons( int );

	protected slots:
		void button_clicked( int );

	protected:
		QButtonGroup *_btns;

	};


    //
    // atc::WorkloadQuestion
    //

    class WorkloadQuestion : public ScaledQuestion {

    public:
        WorkloadQuestion( QWidget * );
        ~WorkloadQuestion();

	};

    //
    // atc::EfficiencyQuestion
    //

    class EfficiencyQuestion : public ScaledQuestion {

    public:
        EfficiencyQuestion( QWidget * );
        ~EfficiencyQuestion();

	};


};

#endif
