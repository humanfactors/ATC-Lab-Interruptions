/**
 * @file:       pact_taskfactory.h
 *
 * 		ATC-Lab Presentation Module
 *              Creation of presentation tasks.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: taskfactory.h,v 1.1.1.2.6.3 2015/08/18 11:55:12 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __PACT_TASKFACTORY_HDR__
#define __PACT_TASKFACTORY_HDR__


#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif


//#include "experiment.h"
#include <stdexcept>


namespace xml{

	class Document;
	struct DocumentNode;

};


namespace pact{


	/*!
	 * \struct pact::taskfactory_error
	 */
	struct taskfactory_error : public std::runtime_error{
		taskfactory_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class pact::Task
	 * \class pact::InstructionTask
	 * \class pact::QuestionTask
	 * \class pact::TrialTask
	 */
	class Task;
	class InstructionTask;
	class QuestionTask;
	class TrialTask;


	/*!
	 * \namespace pact::TaskFactory
	 */
	namespace TaskFactory{

		//
		// public interface
		//

		pact::Task* createTask(
			const xml::Document*, const xml::DocumentNode*
		);

		//
		// private: dispatch creational methods
		//

		pact::InstructionTask* createInstructionTask(
			const xml::Document*, const xml::DocumentNode*
		);
		pact::QuestionTask* createQuestionTask(
			const xml::Document*, const xml::DocumentNode*
		);
		pact::TrialTask* createTrialTask(
			const xml::Document*, const xml::DocumentNode*
		);
      
        void
        interruptionNodeHandler(const xml::Document*       a_doc,
                                const xml::DocumentNode*   a_node,
                                TrialTask*                 a_task);

        void nbackNodeHandler(const xml::Document*      a_doc,	
                              const xml::DocumentNode*  a_node,
                              TrialTask*                a_task);
	};



};


#endif
