// AcceptanceStateDialogBox.h: interface for the AcceptanceStateDialogBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ACCEPTANCE_STATE_DIALOG_BOX_HDR__
#define __ACCEPTANCE_STATE_DIALOG_BOX_HDR__

#include "aircraft_agent.h"
#include <qdialog.h>
#include <qtextedit.h>
#include <qlayout.h>

namespace atc
{
	class AcceptanceStateDialogBox : public QDialog  
	{
	public:
		AcceptanceStateDialogBox(QWidget* w, const char* n, AircraftAgent* aircraft);
		virtual ~AcceptanceStateDialogBox();

	protected: 
		virtual void keyPressEvent(QKeyEvent *);

	private:
		QTextEdit* _textbox;
		AircraftAgent* _aircraft;
		std::string _keys_pressed;
	};
};
#endif