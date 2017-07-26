/**
 *
 *  @project   ATC-Lab. 
 *             Air Traffic Control Experiments.
 *
 *  @author    seth  
 *  @email     atc-support@humanfactors.uq.edu.au
 *
 *  @version   $Name: atclab-rel-2_4_5 $
 *             $Id: param_impl.cpp,v 1.3 2008/06/04 11:33:33 Aaron Yeung Exp $
 *
 *  @copyright 2005 Key Centre for Human Factors 
 *             & Applied Cognitive Psychology.
 *
 **/

#include "param_impl.h"
#include "atc.h"

//------------------------------------------------------------------------------
// Construction / Destruction
//

/*!
 * Default Constructor.
 */
atc::ParamImpl::ParamImpl()
	:_rate(CLOCK_TICK_PERIOD)
	,_multiplier(1)
	,_format("")
	,_hdoms(5)
	,_vdoms(0)
	,_sector_entry_dist(DEFAULT_SECTOR_ENTRY_DIST)
	,_sector_exit_dist(DEFAULT_SECTOR_EXIT_DIST)
{}

/*!
 * Destructor
 */
atc::ParamImpl::~ParamImpl()
{}


//------------------------------------------------------------------------------
// atc::Param interface (mutator methods)
//

void atc::ParamImpl::set_update_rate(unsigned int rate){
	_rate = rate;
}

void atc::ParamImpl::set_update_multiplier(unsigned int multiplier){
	_multiplier = multiplier;
}

void atc::ParamImpl::set_counter_format(const char *format){
	_format = format;
}

void atc::ParamImpl::set_hdoms(int doms){
	_hdoms = doms;
}

void atc::ParamImpl::set_vdoms(int doms){
	_vdoms = doms;
}

void atc::ParamImpl::SetSectorEntryDistance(int distanceNM)
{
	_sector_entry_dist = distanceNM;
}

void atc::ParamImpl::SetSectorExitDistance(int distanceNM)
{
	_sector_exit_dist = distanceNM;
}
//------------------------------------------------------------------------------
