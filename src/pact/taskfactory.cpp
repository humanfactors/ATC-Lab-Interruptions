/**
 * @file:       pact_taskfactory.cpp
 *
 * 		ATC-Lab Presentation Module
 *              Creation of presentation tasks.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: taskfactory.cpp,v 1.3.6.10 2015/08/19 11:33:57 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "taskfactory.h"

#include "instructiontask.h"
#include "questiontask.h"
#include "trialtask.h"
#include "engine.h"
#include "nodehandler.h"
#include "document.h"			// "xml_document.h"
#include "trialtask.h"

#ifdef _WIN32
	#include <assert.h>
#endif

using namespace pact;

//------------------------------------------------------------------------------
// [public] interface
//

/**
 * Factory Method for creating presentaiton task widgets.
 * The caller is responsible for heap object.
 *
 * @param doc The Document.
 * @param node Document Node to process.
 * @return A Task widget.
 * @throws taskfactory_error if node is not a valid type.
 *
 * pre-condition: doc->isValid()
 * pre-condition: node->isValid()
 */
Task* TaskFactory::createTask(
	const xml::Document* doc,
	const xml::DocumentNode* node
){
	assert( doc );
	assert( node );

	if ( "instruction" == node->type ) {
		return TaskFactory::createInstructionTask( doc, node );
	}
	else if ( "question" == node->type ) {
		return TaskFactory::createQuestionTask( doc, node );
	}
	else if ( "trial" == node->type ) {
		return TaskFactory::createTrialTask( doc, node );
	}
	else {
		throw taskfactory_error(
			std::string( "illegal task entry <" ) + node->type + ">"
		);
	}
};


//------------------------------------------------------------------------------
// [public] Hiden constructors
//

/**
 * Build Instruction Presentation Task.
 *
 * pre-condition: node->type == "instruction"
 */
InstructionTask* TaskFactory::createInstructionTask(
	const xml::Document* doc,
	const xml::DocumentNode* node
){
	assert("instruction" == node->type);
	InstructionTask *task = new InstructionTask();
	doc->process_children(
		doc->getReferencedNode(doc->getAttributeValue(node, "idxref")), 
		InstructionNodeHandler(doc, task)
	);
	return task;
}


/**
 * Build Question Presentation Task.
 *
 * pre-condition: node->type == "question"
 */
QuestionTask* TaskFactory::createQuestionTask(
	const xml::Document* doc,
	const xml::DocumentNode* node
){
	assert("question" == node->type);

	const xml::DocumentNode* qNode = doc->getReferencedNode(
		doc->getAttributeValue(node, "idxref")
	);

	QuestionTask *task = new QuestionTask();
	task->setText(doc->getStringContent(qNode, "text"));
	task->setScale(
		doc->getIntegerContent(qNode, "scale min"),
		doc->getIntegerContent(qNode, "scale max")
	);
	return task;
}

	
/**
 * Build ATC Trial Presentation Task.
 *
 * pre-condition: node->type == "trial"
 */
TrialTask* TaskFactory::createTrialTask (
		const xml::Document* doc
	  ,	const xml::DocumentNode* node
){
	assert( "trial" == node->type );

    std::string soundFile;

    if (doc->attributeExists(node, "sound"))
    {
        soundFile = doc->getAttributeValue(node, "sound");
    }

	TrialTask *task = new TrialTask(soundFile);

	if (doc->exists(node, "timeEvent"))
	{
		int trial_time = doc->getIntegerContent(node, "timeEvent");
		task->engine()->TrialTime(trial_time);
	}
    
    if (doc->exists(node, "info_box"))
    {
        const xml::DocumentNode* info_box_node = NULL;

        for (std::vector<xml::DocumentNode*>::const_iterator it = node->children.begin();
            it != node->children.end(); it++)
        {
            if ((*it)->type == "info_box")
            {
                info_box_node = *it;
                break;
            }
        }

        if (info_box_node)
        {
            TrialInfoBoxParams params;

            if (doc->attributeExists(info_box_node, "x"))
            {
                params.m_x = atoi(doc->getAttributeValue(info_box_node, "x").c_str());
            }

            if (doc->attributeExists(info_box_node, "y"))
            {
                params.m_y = atoi(doc->getAttributeValue(info_box_node, "y").c_str());
            }

            if (doc->attributeExists(info_box_node, "font"))
            {
                params.m_font = doc->getAttributeValue(info_box_node, "font");
            }

            if (doc->attributeExists(info_box_node, "font_size"))
            {
                params.m_font_size = atoi(doc->getAttributeValue(info_box_node, "font_size").c_str());
            }

            if (doc->attributeExists(info_box_node, "font_colour"))
            {
                params.m_font_colour = doc->getAttributeValue(info_box_node, "font_colour");
            }

            if (doc->attributeExists(info_box_node, "border"))
            {
                params.m_border = (doc->getAttributeValue(info_box_node, "border") == std::string("true"));
            }

            params.m_text = doc->getStringContent(info_box_node);

            task->engine()->SetInfoBox(params);
        }
    }

    if (doc->exists(node, "interruption"))
    {
        interruptionNodeHandler(doc, node, task);
    }
    else if (doc->exists(node, "nbackTask"))
    {
        nbackNodeHandler(doc, node, task);
    }

	task->SetTrialName(doc->getAttributeValue(node, "idx"));

	//
	// Parameters
	//
	const xml::DocumentNode* paramNode = doc->getReferencedNode(
			doc->getAttributeValue( node, "param" )
	);
	doc->process_children( paramNode, ParamNodeHandler( doc, task ) );

	//
	// Map
	//
	const xml::DocumentNode* mapNode = doc->getReferencedNode(
			doc->getAttributeValue( node, "map" )
	);
	doc->process_children( mapNode, MapNodeHandler( doc, task ) );

	//
	// Sky
	//
	const xml::DocumentNode* skyNode = doc->getReferencedNode(
			doc->getAttributeValue( node, "sky" )
	);
	doc->process_children( skyNode, SkyNodeHandler( doc, task ) );

	//
	// UI
	//
	if ( doc->attributeExists( node, "ui" ) ) {
		const xml::DocumentNode* uiNode = doc->getReferencedNode(
				doc->getAttributeValue( node, "ui" )
		);
		doc->process_children( uiNode, UINodeHandler( doc, task ) );
	}

    bool notify_missed_acceptance = true;
    if (doc->attributeExists(node, "notify_missed_acceptance"))
    {
        notify_missed_acceptance = (doc->getAttributeValue(node, "notify_missed_acceptance") == "true");
    }

    bool notify_missed_handoff = true;
    if (doc->attributeExists(node, "notify_missed_handoff"))
    {
        notify_missed_handoff = (doc->getAttributeValue(node, "notify_missed_handoff") == "true");
    }

    bool disable_callout_rotation = false;
    if (doc->attributeExists(node, "disable_callout_rotation"))
    {
        disable_callout_rotation = (doc->getAttributeValue(node, "disable_callout_rotation") == "true");
    }

    task->SetNotifyMissedAcceptance(notify_missed_acceptance);
    task->SetNotifyMissedHandoff(notify_missed_handoff);
    task->SetDisableCalloutRotation(disable_callout_rotation);
	task->initialize();
	
    return task;
}

void
TaskFactory::interruptionNodeHandler(const xml::Document*       a_doc,
                                     const xml::DocumentNode*   a_node,
                                     TrialTask*                 a_task)
{
    const xml::DocumentNode* initialDisplay_node = NULL;
    const xml::DocumentNode* interuption_node = NULL;

    for (std::vector<xml::DocumentNode*>::const_iterator it = a_node->children.begin();
            it != a_node->children.end(); it++)
    {
        if ((*it)->type == "interruption")
        {
            interuption_node = *it;
            break;
        }
    }
    
    if (interuption_node != NULL)
    {
        int start = atc::INVALID_VALUE;
        int end = atc::INVALID_VALUE;
        bool auto_handoff = false;
        bool auto_accept = false;
        bool show_blank_screen = false;
        bool show_timer = false;
        std::string external_program = "";

        if (a_doc->attributeExists(interuption_node, std::string("start")))
        {
            start = atoi(a_doc->getAttributeValue(interuption_node, "start").c_str());
        }

	    if (a_doc->attributeExists(interuption_node, std::string("end")))
        {
            end = atoi(a_doc->getAttributeValue(interuption_node, "end").c_str());
        }

        if (a_doc->attributeExists(interuption_node, std::string("auto_handoff")))
        {
            if (a_doc->getAttributeValue(interuption_node, "auto_handoff") == "true")
            {
                auto_handoff = true;
            }
        }

        if (a_doc->attributeExists(interuption_node, std::string("auto_accept")))
        {
            if (a_doc->getAttributeValue(interuption_node, "auto_accept") == "true")
            {
                auto_accept = true;
            }
        }

        if (a_doc->attributeExists(interuption_node, std::string("show_blank_screen")))
        {
            if (a_doc->getAttributeValue(interuption_node, "show_blank_screen") == "true")
            {
                show_blank_screen = true;
            }
        }

        if (a_doc->attributeExists(interuption_node, std::string("external_program")))
        {
            external_program = a_doc->getAttributeValue(interuption_node, "external_program");
        }

        if (a_doc->attributeExists(interuption_node, std::string("show_timer")))
        {
            if (a_doc->getAttributeValue(interuption_node, "show_timer") == "true")
            {
                show_timer = true;
            }
        }

        InterruptionParams interruption_params;

        interruption_params.m_start = start;
        interruption_params.m_end = end;
        interruption_params.m_auto_accept = auto_accept;
        interruption_params.m_auto_handoff = auto_handoff;
        interruption_params.m_show_blank_screen = show_blank_screen;
        interruption_params.m_show_timer = show_timer;
        interruption_params.m_external_program = external_program;

        for (std::vector<xml::DocumentNode*>::const_iterator it = interuption_node->children.begin();
            it != interuption_node->children.end(); it++)
        {
            if ((*it)->type == "initial_display")
            {
                initialDisplay_node = *it;
                
                if (a_doc->attributeExists(initialDisplay_node, std::string("symbol")))
                {
                    interruption_params.m_initial_display.m_symbol = a_doc->getAttributeValue(initialDisplay_node, "symbol");
                }
                
                if (a_doc->attributeExists(initialDisplay_node, std::string("font")))
                {
                    interruption_params.m_initial_display.m_font = a_doc->getAttributeValue(initialDisplay_node, "font");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("font_size")))
                {
                    interruption_params.m_initial_display.m_font_size = atoi(a_doc->getAttributeValue(initialDisplay_node, "font_size").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("font_colour")))
                {
                    interruption_params.m_initial_display.m_font_colour = a_doc->getAttributeValue(initialDisplay_node, "font_colour");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("duration_ms")))
                {
                    interruption_params.m_initial_display.m_duration_ms = atoi(a_doc->getAttributeValue(initialDisplay_node, "duration_ms").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("show_timer")))
                {
                    interruption_params.m_initial_display.m_show_timer = (a_doc->getAttributeValue(initialDisplay_node, "show_timer") == "true");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("x")))
                {
                    interruption_params.m_initial_display.m_x = atoi(a_doc->getAttributeValue(initialDisplay_node, "x").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("y")))
                {
                    interruption_params.m_initial_display.m_y = atoi(a_doc->getAttributeValue(initialDisplay_node, "y").c_str());
                }
            }
        }

        a_task->SetInterruptParams(interruption_params);
    }
}

void
TaskFactory::nbackNodeHandler(const xml::Document*      a_doc,	
                              const xml::DocumentNode*  a_node,
                              TrialTask*                a_task)
{
    const xml::DocumentNode* nbackTask_node = NULL;
    const xml::DocumentNode* initialDisplay_node = NULL;
    const xml::DocumentNode* stimuli_node = NULL;
    const xml::DocumentNode* correct_feedback_node = NULL;
    const xml::DocumentNode* incorrect_feedback_node = NULL;
    const xml::DocumentNode* timer_node = NULL;

    nBackTaskParams nback_params;

    for (std::vector<xml::DocumentNode*>::const_iterator it = a_node->children.begin();
            it != a_node->children.end(); it++)
    {
        if ((*it)->type == "nbackTask")
        {
            nbackTask_node = *it;
            break;
        }
    }
    
    if (nbackTask_node != NULL)
    {
        if (a_doc->attributeExists(nbackTask_node, std::string("start")))
        {
            nback_params.m_start = atoi(a_doc->getAttributeValue(nbackTask_node, "start").c_str());
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("end")))
        {
            nback_params.m_end = atoi(a_doc->getAttributeValue(nbackTask_node, "end").c_str());
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("auto_handoff")))
        {
            if (a_doc->getAttributeValue(nbackTask_node, "auto_handoff") == "true")
            {
                nback_params.m_auto_handoff = true;
            }
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("auto_accept")))
        {
            if (a_doc->getAttributeValue(nbackTask_node, "auto_accept") == "true")
            {
                nback_params.m_auto_accept = true;
            }
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("background_colour")))
        {
            nback_params.m_background_colour = a_doc->getAttributeValue(nbackTask_node, "background_colour");
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("response_key")))
        {
            nback_params.m_response_key = a_doc->getAttributeValue(nbackTask_node, "response_key");
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("n_factor")))
        {
            nback_params.m_n_factor = atoi(a_doc->getAttributeValue(nbackTask_node, "n_factor").c_str());
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("reptition_probability")))
        {
            nback_params.m_repetition_probability = atoi(a_doc->getAttributeValue(nbackTask_node, "reptition_probability").c_str());
        }

        if (a_doc->attributeExists(nbackTask_node, std::string("show_task")))
        {
            nback_params.m_show_task = a_doc->getAttributeValue(nbackTask_node, "show_task") == "true";
        }

        for (std::vector<xml::DocumentNode*>::const_iterator it = nbackTask_node->children.begin();
            it != nbackTask_node->children.end(); it++)
        {
            if ((*it)->type == "initial_display")
            {
                initialDisplay_node = *it;
                
                if (a_doc->attributeExists(initialDisplay_node, std::string("symbol")))
                {
                    nback_params.m_initial_display.m_symbol = a_doc->getAttributeValue(initialDisplay_node, "symbol");
                }
                
                if (a_doc->attributeExists(initialDisplay_node, std::string("font")))
                {
                    nback_params.m_initial_display.m_font = a_doc->getAttributeValue(initialDisplay_node, "font");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("font_size")))
                {
                    nback_params.m_initial_display.m_font_size = atoi(a_doc->getAttributeValue(initialDisplay_node, "font_size").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("font_colour")))
                {
                    nback_params.m_initial_display.m_font_colour = a_doc->getAttributeValue(initialDisplay_node, "font_colour");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("duration_ms")))
                {
                    nback_params.m_initial_display.m_duration_ms = atoi(a_doc->getAttributeValue(initialDisplay_node, "duration_ms").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("show_timer")))
                {
                    nback_params.m_initial_display.m_show_timer = (a_doc->getAttributeValue(initialDisplay_node, "show_timer") == "true");
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("x")))
                {
                    nback_params.m_initial_display.m_x = atoi(a_doc->getAttributeValue(initialDisplay_node, "x").c_str());
                }

                if (a_doc->attributeExists(initialDisplay_node, std::string("y")))
                {
                    nback_params.m_initial_display.m_y = atoi(a_doc->getAttributeValue(initialDisplay_node, "y").c_str());
                }
            }
            
            if ((*it)->type == "stimuli")
            {
                stimuli_node = *it;
                
                if (a_doc->attributeExists(stimuli_node, std::string("font")))
                {
                    nback_params.m_stimuli.m_font = a_doc->getAttributeValue(stimuli_node, "font");
                }

                if (a_doc->attributeExists(stimuli_node, std::string("font_size")))
                {
                    nback_params.m_stimuli.m_font_size = atoi(a_doc->getAttributeValue(stimuli_node, "font_size").c_str());
                }
                
                if (a_doc->attributeExists(stimuli_node, std::string("font_colour")))
                {
                    nback_params.m_stimuli.m_font_colour = a_doc->getAttributeValue(stimuli_node, "font_colour");
                }

                if (a_doc->attributeExists(stimuli_node, std::string("display_duration_ms")))
                {
                    nback_params.m_stimuli.m_display_duration_ms = atoi(a_doc->getAttributeValue(stimuli_node, "display_duration_ms").c_str());
                }

                if (a_doc->attributeExists(stimuli_node, std::string("show_timer")))
                {
                    nback_params.m_stimuli.m_show_timer = (a_doc->getAttributeValue(stimuli_node, "show_timer") == "true");
                }

                if (a_doc->attributeExists(stimuli_node, std::string("x")))
                {
                    nback_params.m_stimuli.m_x = atoi(a_doc->getAttributeValue(stimuli_node, "x").c_str());
                }

                if (a_doc->attributeExists(stimuli_node, std::string("y")))
                {
                    nback_params.m_stimuli.m_y = atoi(a_doc->getAttributeValue(stimuli_node, "y").c_str());
                }
            }
            
            if ((*it)->type == "correct_feedback")
            {
                correct_feedback_node = *it;

                if (a_doc->attributeExists(correct_feedback_node, std::string("symbol")))
                {
                    nback_params.m_correct_feedback.m_symbol = a_doc->getAttributeValue(correct_feedback_node, "symbol");
                }

                if (a_doc->attributeExists(correct_feedback_node, std::string("colour")))
                {
                    nback_params.m_correct_feedback.m_colour = a_doc->getAttributeValue(correct_feedback_node, "colour");
                }

                if (a_doc->attributeExists(correct_feedback_node, std::string("width")))
                {
                    nback_params.m_correct_feedback.m_width = atoi(a_doc->getAttributeValue(correct_feedback_node, "width").c_str());
                }

                if (a_doc->attributeExists(correct_feedback_node, std::string("height")))
                {
                    nback_params.m_correct_feedback.m_height = atoi(a_doc->getAttributeValue(correct_feedback_node, "height").c_str());
                }

                if (a_doc->attributeExists(correct_feedback_node, std::string("x")))
                {
                    nback_params.m_correct_feedback.m_x = atoi(a_doc->getAttributeValue(correct_feedback_node, "x").c_str());
                }

                if (a_doc->attributeExists(correct_feedback_node, std::string("y")))
                {
                    nback_params.m_correct_feedback.m_y = atoi(a_doc->getAttributeValue(correct_feedback_node, "y").c_str());
                }
            }

            if ((*it)->type == "incorrect_feedback")
            {
                incorrect_feedback_node = *it;

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("symbol")))
                {
                    nback_params.m_incorrect_feedback.m_symbol = a_doc->getAttributeValue(incorrect_feedback_node, "symbol");
                }

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("colour")))
                {
                    nback_params.m_incorrect_feedback.m_colour = a_doc->getAttributeValue(incorrect_feedback_node, "colour");
                }

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("width")))
                {
                    nback_params.m_incorrect_feedback.m_width = atoi(a_doc->getAttributeValue(incorrect_feedback_node, "width").c_str());
                }

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("height")))
                {
                    nback_params.m_incorrect_feedback.m_height = atoi(a_doc->getAttributeValue(incorrect_feedback_node, "height").c_str());
                }

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("x")))
                {
                    nback_params.m_incorrect_feedback.m_x = atoi(a_doc->getAttributeValue(incorrect_feedback_node, "x").c_str());
                }

                if (a_doc->attributeExists(incorrect_feedback_node, std::string("y")))
                {
                    nback_params.m_incorrect_feedback.m_y = atoi(a_doc->getAttributeValue(incorrect_feedback_node, "y").c_str());
                }
            }

            if ((*it)->type == "timer")
            {
                timer_node = *it;

                if (a_doc->attributeExists(timer_node, std::string("font")))
                {
                    nback_params.m_timer.m_font = a_doc->getAttributeValue(timer_node, "font");
                }

                if (a_doc->attributeExists(timer_node, std::string("font_size")))
                {
                    nback_params.m_timer.m_font_size = atoi(a_doc->getAttributeValue(timer_node, "font_size").c_str());
                }

                if (a_doc->attributeExists(timer_node, std::string("font_colour")))
                {
                    nback_params.m_timer.m_font_colour = a_doc->getAttributeValue(timer_node, "font_colour");
                }

                if (a_doc->attributeExists(timer_node, std::string("x")))
                {
                    nback_params.m_timer.m_x = atoi(a_doc->getAttributeValue(timer_node, "x").c_str());
                }

                if (a_doc->attributeExists(timer_node, std::string("y")))
                {
                    nback_params.m_timer.m_y = atoi(a_doc->getAttributeValue(timer_node, "y").c_str());
                }
            }
        }

        a_task->SetnBackTaskParams(nback_params);
    }
}
//------------------------------------------------------------------------------
