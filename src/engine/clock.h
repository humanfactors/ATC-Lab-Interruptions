/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: clock.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2007/07/19 15:18:14 $
      @state    : $State: Exp $

      $Log: clock.h,v $
      Revision 1.2  2007/07/19 15:18:14  Aaron Yeung
      Added XML scriptable Clock that can control the length of time XML scripted widgets can appear onscreen

      Revision 1.1.1.2  2007/06/21 12:22:43  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.9  2007/02/27 06:07:17  seth
      implemented clock

      Revision 1.8  2006/11/01 04:53:36  seth
      created installer

      Revision 1.7  2006/10/20 04:49:23  seth
      reduced dimensions of workload popup question && initial implementation of experiment time (for inclusion in log file output)

      Revision 1.6  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.5  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/27 13:01:19  seth
      progress ....*.....

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_CLOCK_HDR__
#define __ATC_CLOCK_HDR__


#include <qobject.h>
#include <qdatetime.h>

#include "atc_types.h"


namespace atc {

    //
    // atc::Clock
    //

    class Clock : public QObject {
		Q_OBJECT

    public:
        Clock();
        ~Clock();

    public:
		void set_interval( int );
		void set_multiple( int );

		long elapsed_time();

	public slots:
		void start();
		void stop();

	private:
		bool is_running();

	private slots:
		void tick_dispatch();

	signals:
		void tick( int );
		void tick();
		void seconds_update( long );
		void started();
		void stopped();
		void event_recorded( KeyValues & );

	private:
		int     _interval;
		int     _multiple;
		QTimer *_timer;
		QTime   _time;
		long    _elapsed;
		long	_seconds;

		unsigned int _stops;

	};


};

#endif
