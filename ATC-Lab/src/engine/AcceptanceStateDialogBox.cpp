// AcceptanceStateDialogBox.cpp: implementation of the AcceptanceStateDialogBox class.
//
//////////////////////////////////////////////////////////////////////

#include "AcceptanceStateDialogBox.h"
#include "logging/ExcelLogger.h"

using namespace atc;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int DEFAULT_SIZE_X = 0;
const int DEFAULT_SIZE_Y = 0;

AcceptanceStateDialogBox::AcceptanceStateDialogBox(QWidget* w, const char* n, AircraftAgent* aircraft)
	: QDialog(w, n, 0, WStyle_Customize | WStyle_NoBorder), 
		_aircraft(aircraft)
{
	resize(DEFAULT_SIZE_X, DEFAULT_SIZE_Y);

	_textbox = new QTextEdit();

	QBoxLayout *l = new QVBoxLayout(dynamic_cast<QWidget*>(this));
	l->addWidget(_textbox);
}

AcceptanceStateDialogBox::~AcceptanceStateDialogBox()
{
	//ExcelLogging::ExcelLogger::instance()->LogFlashDetailsKeysPressed(_aircraft->callsign(), _keys_pressed);
	delete _textbox;
}

void AcceptanceStateDialogBox::keyPressEvent(QKeyEvent* e) 
{
	try
	{
		std::stringstream key;
		key << (char) e->key();
		
		if (_keys_pressed == "")
		{
			std::stringstream acceptance_key;
			acceptance_key << (char) AC_ACCEPT_KEY_DEFAULT;
			
			_keys_pressed = acceptance_key.str() + std::string(" ");
		}

		_keys_pressed += key.str() + std::string(" ");

		if (_aircraft->FlashParams().flash_item == FLASH_ITEM_SI)
		{
			switch (_aircraft->UserInputState())
			{
			case USER_INPUT_STATE_DEFAULT:
				if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				break;
			case USER_INPUT_STATE_COORDINATION_REQUEST:
				if (e->key() == _aircraft->CoordinationRequestKey())
				{
					done(Accepted);
				}
				else if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				/*
				else if (e->key() == ACCEPT_STATE_KEY_SI_DEFAULT)
				{
					done(Rejected);
				}
				*/
				break;
			}
		}
		else if (_aircraft->FlashParams().flash_item == FLASH_ITEM_FL)
		{
			switch (_aircraft->UserInputState())
			{
			case USER_INPUT_STATE_DEFAULT:
				if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				break;
			case USER_INPUT_STATE_COORDINATION_REQUEST:
				if (e->key() == _aircraft->CoordinationRequestKey())
				{
					done(Accepted);
				}
				else if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				/*
				else if (e->key() == ACCEPT_STATE_KEY_FL_DEFAULT)
				{
					done(Rejected);
				}
				*/
				break;
			}
		}
		else if (_aircraft->FlashParams().flash_item == FLASH_ITEM_CS)
		{
			switch (_aircraft->UserInputState())
			{
			case USER_INPUT_STATE_DEFAULT:
				if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				break;
			case USER_INPUT_STATE_COORDINATION_REQUEST:
				if (e->key() == _aircraft->CoordinationRequestKey())
				{
					done(Accepted);
				}
				else if (e->key() == AC_ACCEPT_KEY_DEFAULT)
				{
					done(Accepted);
				}
				/*
				else if (e->key() == ACCEPT_STATE_KEY_CS_DEFAULT)
				{
					done(Rejected);
				}
				*/
				break;
			}
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling AcceptanceStateDialogBox::keyPressEvent");
	}
}
