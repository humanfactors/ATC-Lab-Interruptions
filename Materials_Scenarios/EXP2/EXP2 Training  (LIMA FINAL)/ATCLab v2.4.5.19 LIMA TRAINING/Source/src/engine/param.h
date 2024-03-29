/**
 *
 *  @project   ATC-Lab. 
 *             Air Traffic Control Experiments.
 *
 *  @author    seth  
 *  @email     atc-support@humanfactors.uq.edu.au
 *
 *  @version   $Name: atclab-rel-2_4_5 $
 *             $Id: param.h,v 1.2 2007/07/09 12:41:42 Aaron Yeung Exp $
 *
 *  @copyright 2005 Key Centre for Human Factors 
 *             & Applied Cognitive Psychology.
 *
 **/
#ifndef __ATC_PARAM_HDR__
#define __ATC_PARAM_HDR__


namespace atc{


	/*!
	 * \class atc::Param
	 * \brief Interface (mutator) for setting up experimental parameters.
	 */
	class Param
	{
	public:

		/*!
		 * Set the update (advance) rate in milliseconds.
		 *
		 * \param rate generally values >1000ms should be used (untested).
		 */
		virtual void set_update_rate(unsigned int rate) = 0;

		/*!
		 * Set the update (advance) multiplier.
		 *
		 * The multiplier is used to run the experiment in 'quick-time'. The
		 * experiment clock will be sped up by the multipier so that events
		 * will happen at consistent times no matter what value is set.
		 *
		 * \param multiplier >0 (untested; upper limit determined by hardware).
		 */
		virtual void set_update_multiplier(unsigned int) = 0;

		/*!
		 * Set the update counter formatter string.
		 * 
		 * Setting this value implies that the counter should be displayed.
		 * The format should contain '%1' which will be the insert position 
		 * for the number of updates.
		 *
		 * \param format 
		 */
		virtual void set_counter_format(const char *format) = 0;

		/*!
		 * Set the horizontal distance of minimum seperation.
		 *
		 * The minimum horizontal distance between two aircraft for the 
		 * for the aircraft not to be in conflict with each other. Both 
		 * a vertical and a horizontal violation would need to occur for
		 * an actual conflict situation.
		 *
		 * A value for the horizontal DOMS should always be set.
		 *
		 * \param dom
		 *
		 * \sa set_vdoms()
		 */
		virtual void set_hdoms(int) = 0;

		/*!
		 * Set the vertical distance of minimum seperation.
		 *
		 * The minimum vertical distance between two aircraft for the 
		 * for the aircraft not to be in conflict with each other.
		 *
		 * If no value is set for the vertial DOMS then altitude violations
		 * are not considered when determining conflict situations.
		 *
		 * \param dom
		 *
		 * \sa set_hdoms()
		 */
		virtual void set_vdoms(int) = 0;

		/*!
		 * Set the sector entry distance (nautical miles).
		 *
		 * The sector entry distance is the threshold at which an aircraft approaches
		 * the boundary of a sector before it goes into a CS_PROPOSED state and
		 * flashes orange
		 */
		virtual void SetSectorEntryDistance(int distanceNM) = 0;

		/*!
		 * Set the sector exit distance (nautical miles).
		 *
		 * The sector exit distance is the threshold at which an aircraft leaves
		 * the boundary of a sector before it goes into a CS_HANDOFF state 
		 */
		virtual void SetSectorExitDistance(int distanceNM) = 0;
	};

}

#endif
