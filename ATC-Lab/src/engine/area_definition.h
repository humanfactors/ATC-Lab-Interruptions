/**
      @project  : ATC-Lab

      @file     : $RCSfile: area_definition.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:42 $
      @state    : $State: Exp $

      $Log: area_definition.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.2  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.1  2006/05/25 04:15:25  seth
      implemented weather, few minor bug fixes, initial cfl/vel selection


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#ifndef __ATC_AREA_DEFINITION_HDR__
#define __ATC_AREA_DEFINITION_HDR__

#include <string>
#include <vector>


namespace atc{

	/*!
	 *
	 */
	struct AreaDescriptor;


	/*!
	 *
	 */
	struct AreaDefinition {

		AreaDefinition();
		AreaDefinition( const char * );
		~AreaDefinition();

		std::vector< atc::AreaDescriptor* >::const_iterator	begin() const {
			return descriptors.begin();
		}
		std::vector< atc::AreaDescriptor* >::const_iterator	end() const {
			return descriptors.end();
		}

		std::string uid;
		std::vector< atc::AreaDescriptor* > descriptors;

		typedef std::vector< atc::AreaDescriptor* >::const_iterator const_iterator;
	};


	/*!
	 *
	 */
	struct SectorDefinition : public AreaDefinition {

		SectorDefinition() : AreaDefinition(), active( false ) {}
		SectorDefinition( const char *uid ) 
			  : AreaDefinition( uid ), active( false ) {}

		void set_active() { active = true; }

		bool active;
	};


	/*!
	 * 
	 */
	struct AreaDescriptor {

		enum { 
				RTTI_VERTEX
			  , RTTI_ARC
			  , RTTI_ELLIPSE
		};
		
		AreaDescriptor( double x, double y, int rtti = RTTI_VERTEX )
			:x(x), y(y), rtti(rtti) {}
		virtual ~AreaDescriptor() {};

		int rtti;
		double x, y;
	};


	/*!
	 * 
	 */
	typedef atc::AreaDescriptor VertexDescriptor;


	/*!
	 * 
	 */
	struct ArcDescriptor : public AreaDescriptor{

		ArcDescriptor( double x, double y, double r )
			:AreaDescriptor( x, y, RTTI_ARC ), r(r) {}

		double r;
	};

	/*!
	 * 
	 */
	struct EllipseDescriptor : public AreaDescriptor{

		EllipseDescriptor( double x, double y, double w, double h, double a )
			:AreaDescriptor( x, y, RTTI_ELLIPSE ), w(w), h(h), a(a) {}

		double w;
		double h;
		double a;
	};


	/*!
	 *
	 */
	typedef atc::AreaDescriptor SectorDescriptor;

}
#endif
