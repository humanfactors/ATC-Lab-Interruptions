/**
 *
 *  @project   ATC-Lab. 
 *             Air Traffic Control Experiments.
 *
 *  @author    seth  
 *  @email     atc-support@humanfactors.uq.edu.au
 *
 *  @version   $Name: atclab-rel-2_4_5 $
 *             $Id: param_impl.h,v 1.2 2007/07/09 12:41:57 Aaron Yeung Exp $
 *
 *  @copyright 2005 Key Centre for Human Factors 
 *             & Applied Cognitive Psychology.
 *
 **/

#ifndef __ATC_PARAM_IMPL_HDR__
#define __ATC_PARAM_IMPL_HDR__


#include "param.h"
#include <string>


namespace atc{


	/*!
	 * \class atc::ParamImpl
	 * \brief Experimental arameters.
	 */
	class ParamImpl : public atc::Param
	{
	public:
		 
		ParamImpl();
		~ParamImpl();

	public: // Param interface

		void set_update_rate(unsigned int rate);
		void set_update_multiplier(unsigned int);
		void set_counter_format(const char *format);
		void set_hdoms(int);
		void set_vdoms(int);
		void SetSectorEntryDistance(int distanceNM);
		void SetSectorExitDistance(int distanceNM);

	public: // accessors

		int update_rate()const{ return _rate; }
		int update_multiplier()const{ return _multiplier; }

		const std::string& counter_format()const{ return _format; }

		int hdoms()const{ return _hdoms; }
		int vdoms()const{ return _vdoms; }
		int SectorEntryDistance() const { return _sector_entry_dist; }
		int SectorExitDistance() const { return _sector_exit_dist; }

	private: // members

		int _rate;
		int _multiplier;
		std::string _format;
		int _hdoms;
		int _vdoms;
		int _sector_entry_dist;
		int _sector_exit_dist;
	};

}

#endif
