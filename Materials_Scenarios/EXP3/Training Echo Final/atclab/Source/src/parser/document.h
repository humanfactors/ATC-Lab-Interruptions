/**
 * @file:       document.h
 *
 *              XML Utilities
 *              Memory representation of XML script.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/08/04
 *
 * $Id: document.h,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __DOCUMENT_HDR__
#define __DOCUMENT_HDR__

#ifdef WIN32
	// disable 'identifier' truncation warnings
	#pragma warning(disable:4786)
#endif


#include <map>
#include <stdexcept>
#include <string>
#include <vector>


namespace xml{

	/*!
	 * \struct document_error
	 */
	struct document_error : public std::runtime_error{
		document_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class xml::NodeHandler
	 */
	class NodeHandler;

	
	/*!
	 * \struct DocumentNode
	 * 
	 * Note:
	 * - children implemented as vector since order is important 
	 *   (xml sequence)
	 * - tried multimap which gives good lookup but has issues
	 *   + the returning of items with same key in insertion order is 
	 *     NON standard
	 * - considered map<name, vector>
	 *   + overcomes multimap same key elements order issues
	 *   + sometimes (eg. sector descriptors) different types may need
	 *     to be accessed in insertion order.
	 */
	struct DocumentNode	{

		DocumentNode( const std::string & );
		~DocumentNode();

		std::string type;
		std::string idx;
		std::map<std::string, std::string> attributes;
		std::string content;
		DocumentNode* parent;
		std::vector< DocumentNode* > children;
	};


	/*!
	 * \class Document document.h
	 */
	class Document
	{
	public:

		Document();
		virtual ~Document();

	public:

		virtual void createChild(const std::string&);

	public:
		
		void ascendParent();

		void setReference(const std::string&);
		void setIDRef(const std::string&);
		
		void addAttribute(const std::string&, const std::string&);
		void setContent(const std::string &);

		void setWarning(){ ++_errors; }
		void setError(){ ++_errors; }
		void setFatal(){ ++_errors; }

		bool is_valid()const{ return _valid; }

	public:

		void process_children(
			const xml::DocumentNode*,
			xml::NodeHandler&
		)const;

		// lookup functions
		//
		// - could move these to DocNode *BUT* trying
		//   to keep these light. Also 'getReferencedNode'
		//   better in doc class

		const xml::DocumentNode* root()const{ return _root; }
		const xml::DocumentNode* current()const{ return _current; }

		const xml::DocumentNode* getReferencedNode(
			const std::string&
		)const;

		bool exists(
			const xml::DocumentNode*, const std::string&
		)const;

		const xml::DocumentNode* getDescendant(
			const xml::DocumentNode*, const std::string&
		)const;

		bool attributeExists(
			const xml::DocumentNode*, const std::string&
		)const;

		std::string getAttributeValue(
			const xml::DocumentNode*, const std::string&
		)const;

		std::string getStringContent(
			const xml::DocumentNode*, const std::string& = ""
		)const;

	public: // virtual

		virtual bool validate();

		virtual int getIntegerContent(
			const xml::DocumentNode*, const std::string& = ""
		)const;

		virtual double getDecimalContent(
			const xml::DocumentNode*, const std::string& =""
		)const;

	protected:

		int _errors;
		bool _valid;

		DocumentNode *_root;
		DocumentNode *_current;
		
		std::map<std::string, DocumentNode*> _references;
	};


	/*!
	 * \class xml::has_type
	 */
	class has_type
	{
	public:

		has_type(const std::string& type):_type(type){}
		~has_type(){}

		bool operator()(const xml::DocumentNode* node){
			return _type == node->type;
		}

	private:

		std::string _type;

	};


	/*!
	 * \struct pact::DocumentView
	 */
	struct DocumentView{
		virtual ~DocumentView(){}
	};


	/*!
	 * \class xml::NodeHandler
	 */
	class NodeHandler
	{
	public:

		NodeHandler(const xml::Document* doc, xml::DocumentView* view)
			:_doc(doc), _view(view){}
		virtual ~NodeHandler(){}

		virtual void operator()(const xml::DocumentNode*) = 0;

	protected:

		const xml::Document* _doc;
		xml::DocumentView* _view;

	};


	/*!
	 * \typedef 
	 */
	typedef std::vector<DocumentNode*> DocNodes;
	typedef xml::DocNodes::const_iterator ChildCIt;

	typedef std::map<std::string, std::string> AttributeMap;
	typedef AttributeMap::const_iterator AttribMapCIt;

	typedef std::map<std::string, DocumentNode*> ReferenceMap;
	typedef ReferenceMap::const_iterator RefMapCIt;


	
}; 

#endif 
