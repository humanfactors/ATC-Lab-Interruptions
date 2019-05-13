/**
 * @file:       atc_presenter.h
 *
 * 		ATC-Lab Presentation Module
 *              Presentation Management.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: presenter.h,v 1.2 2010/04/08 13:14:34 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_PRESENTER_HDR__
#define __ATC_PRESENTER_HDR__

#include <qwidgetstack.h>
#include "experiment.h"

namespace pact{


	/*!
	 */
	class Experiment;


	/*!
	 */
	class Presenter : public QWidgetStack
	{
		Q_OBJECT

	public:
		Presenter(QWidget * = 0, const char * = 0);
		virtual ~Presenter();

	public:
		void setExperiment(pact::Experiment *);

	public slots:
		void presentTask();

		void start();
		
	protected:
		virtual void keyPressEvent(QKeyEvent *);

	private:
		const pact::Experiment *_experiment;
		pact::Experiment::const_iterator _task;
			
	};
	
}

#endif
