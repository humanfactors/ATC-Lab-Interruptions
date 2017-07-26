// Utilities.h: interface for the Utilities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILITIES_H__9F152151_4F91_40E9_B168_8E77FA147DD5__INCLUDED_)
#define AFX_UTILITIES_H__9F152151_4F91_40E9_B168_8E77FA147DD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdexcept>
#include <qdialog.h>
#include <vector>
#include <string>

namespace Utils
{
	class Utilities  
	{
	public:
		static Utilities* instance();

		static void destroy();

		static std::auto_ptr<QDialog> CreateDialogBox(
			const std::string& caption, 
			const std::string& message);

		static void Tokenize(const std::string& str,
								std::vector<std::string>& tokens,
								const std::string& delimiters = "\n");
		
		static std::string GetDisplayText(std::vector<std::string>& a_displayText);

		static void TrimString(std::string& a_str);

		static void ToUpper(std::string& a_str);

		static void RemoveExcessChars(std::string& a_str, const char* a_char);

        static QColor GetQColorFromString(std::string a_colour);

	private:
		Utilities();
		~Utilities();

		static Utilities* _instance;
	};
};
#endif // !defined(AFX_UTILITIES_H__9F152151_4F91_40E9_B168_8E77FA147DD5__INCLUDED_)
