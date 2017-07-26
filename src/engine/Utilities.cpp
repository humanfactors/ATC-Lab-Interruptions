// Utilities.cpp: implementation of the Utilities class.
//
//////////////////////////////////////////////////////////////////////

#include "Utilities.h"
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace Utils;

Utilities* Utilities::_instance = 0;

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}

Utilities* Utilities::instance()
{
	if (!_instance)
	{
		_instance = new Utilities();
	}

	return _instance;
}

void Utilities::destroy()
{
	if (_instance)
	{
		delete _instance;
		_instance = 0;
	}
}

std::auto_ptr<QDialog> Utilities::CreateDialogBox(const std::string& caption, const std::string& message)
{
	std::auto_ptr<QDialog> dlg(new QDialog(0, 0, 0, Qt::WStyle_Customize| Qt::WStyle_NormalBorder| Qt::WStyle_Title));

	dlg->setCaption(caption.c_str());
	dlg->setFont(QFont( "Courier New", 12, QFont::Normal));
	dlg->resize(0, 0);

	QPushButton* ok_button = new QPushButton(QString("OK"), dynamic_cast<QWidget*>(dlg.get()), "OK");

	QGridLayout* mainmgr = new QGridLayout( 3, 3, 5 );
	mainmgr->addWidget(new QLabel(message.c_str(), dlg.get()), 0, 0);

	QBoxLayout* btnmgr = new QHBoxLayout();
	btnmgr->addStretch();
	btnmgr->addWidget(ok_button);

	QBoxLayout* vmgr = new QVBoxLayout(dlg.get(), 10, 3 );
	vmgr->addLayout(mainmgr);
	vmgr->addLayout(btnmgr);

	dlg.get()->connect(ok_button, SIGNAL(clicked()), dlg.get(), SLOT(accept()));

	return dlg;
}

void 
Utilities::Tokenize(const std::string&			str,
					std::vector<std::string>&	tokens,
                     const std::string&			delimiters)
{
	std::string s = str;
	size_t startPos = 0;
	size_t endPos = 0;

	while (endPos != std::string::npos)
	{
		startPos = 0;
		endPos = s.find(delimiters);

		if (endPos != std::string::npos)
		{
			tokens.push_back(s.substr(startPos, endPos));
			s = s.substr(endPos + delimiters.size(), str.size());
		}
		else
		{
			tokens.push_back(s);
			s = "";
		}
	}
}

std::string
Utilities::GetDisplayText(std::vector<std::string>& a_displayText)
{
	std::string displayText;

	for (int i = 0; i < a_displayText.size(); i++)
	{
		displayText += a_displayText[i];
			
		if (i != a_displayText.size() - 1)
		{
			displayText += '\n';
		}
	}

	return displayText;
}

void
Utilities::TrimString(std::string& a_str)
{
	// Trim Both leading and trailing spaces  
	// Find the first character position after excluding leading blank spaces  
	size_t startpos = a_str.find_first_not_of(" "); 

	// Find the first character position from reverse af  
	size_t endpos = a_str.find_last_not_of(" "); 

	// if all spaces or empty return an empty string  
	if ((startpos != std::string::npos) && (endpos != std::string::npos))  
	{
		a_str = a_str.substr(startpos, endpos - startpos + 1);
	}
}

void
Utilities::ToUpper(std::string& a_str)
{
	for (unsigned int i = 0; i < a_str.length(); i++)
	{
		a_str[i] = toupper(a_str[i]);
	}
}

void
Utilities::RemoveExcessChars(std::string& a_str, const char* a_char)
{
	std::vector<std::string> tokens;
	std::string str;

	Tokenize(a_str, tokens, std::string(a_char));

	for (std::vector<std::string>::iterator it = tokens.begin();
			it != tokens.end(); it++)
	{
		if (*it != std::string(""))
		{
			str += *it + std::string(a_char);
		}
	}

	TrimString(str);

	a_str = str;
}

QColor
Utilities::GetQColorFromString(std::string a_colour)
{
    QColor colour = Qt::white;

    if (a_colour == "black")
    {
        colour = Qt::black;
    }
    else if (a_colour == "white")
    {
        colour = Qt::white;
    }
    else if (a_colour == "blue")
    {
        colour = Qt::blue;
    }
    else if (a_colour == "red")
    {
        colour = Qt::red;
    }
    else if (a_colour == "green")
    {
        colour = Qt::green;
    }

    return colour;
}