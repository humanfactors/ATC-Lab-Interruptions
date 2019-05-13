/**
 * @file:       document.cpp
 *
 *              XML Utilities
 *              Memory representation of XML script.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/08/04
 *
 * $Id: document.cpp,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "document.h"
#include <algorithm>
#include <sstream>

#ifdef WIN32
	#include <assert.h>
	#pragma warning(disable:4800) // forcing value(int) to bool
#endif

using namespace xml;

////////////////////////////////////////////////////////////////////////////////
//
// xml::DocumentNode
//
//------------------------------------------------------------------------------
//
//

/**
 *
 */
DocumentNode::DocumentNode( const std::string &type )
	  : type(type) {}

/**
 *
 */
DocumentNode::~DocumentNode() {
	DocNodes::const_iterator node = children.begin();
	for ( ; node != children.end(); ++node ) {
		delete *node;
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// xml::Document
//
//------------------------------------------------------------------------------
//
//


/**
 *
 */
Document::Document()
	:_errors(0)
	,_valid(false)
	,_root(0)
	,_current(0)
	,_references()
{}

/**
 *
 */
Document::~Document() {
	if(_root) delete _root;
}

//------------------------------------------------------------------------------

/**
 *
 */
void Document::createChild(const std::string& type)
{
	DocumentNode *node = new DocumentNode(type);
	if(_current){
		_current->children.push_back(node);
		node->parent = _current;

	}else{
		_root = node;
	}
	
	_current = node;
}

/**
 *
 */
void Document::ascendParent()
{
	if(_current == _root){
	
	}else{
		_current = _current->parent;
	}
}

/**
 *
 */
void Document::setReference(const std::string &idx){
	_current->idx = idx;
	_references.insert(std::make_pair(idx, _current));
}

/**
 *
 */
void Document::setIDRef(const std::string &idx){
	if(!_current->idx.size()) _current->idx = idx;
}

/**
 *
 */
void Document::addAttribute(const std::string &name, const std::string &value){
	_current->attributes.insert(std::make_pair(name, value));
}

/**
 *
 */
void Document::setContent(const std::string &content){
	_current->content = content;
}

//------------------------------------------------------------------------------

/**
 * The document should be validated before use
 */
bool Document::validate(){
	return (_valid = !_errors);
}

//------------------------------------------------------------------------------
// [public] 
//

/**
 * Iterate and process a document node's children
 * 
 * @param node The parent document node
 * @param hdlr The handler for the performing processing
 *
 * pre-condition: a node with refkey exists
 */
void Document::process_children(
	const DocumentNode* node,
	NodeHandler& hdlr
)const{
	for(
		ChildCIt child = node->children.begin();
		child != node->children.end();
		++child
	){
		hdlr(*child);
	}
}

//------------------------------------------------------------------------------
// [public] lookup functions
//

/**
 * Lookup a referenced document node by name
 * 
 * @param refkey unique name for the node (xml type xs:ID)
 * @return xml::DocumentNode*
 *
 * pre-condition: a node with refkey exists
 */
const DocumentNode* Document::getReferencedNode(
	const std::string &refkey
)const{
	RefMapCIt rit = _references.find(refkey);
	assert(rit != _references.end());
	return rit->second;
}

/**
 * Check for existance of child nodes of specific type
 *
 * @param root Document node to query.
 * @param name Child document node name to look for.
 * @return Existance of child in root.
 *
 * pre-condition: root is valid.
 */
bool Document::exists(
	const DocumentNode *root,
	const std::string &name
)const{
	assert(root);
	return root->children.end() != std::find_if(
		root->children.begin(),
		root->children.end(),
		has_type(name)
	);
}

/**
 * Look up document node by path
 *
 * @param root starting node for lookup
 * @param path a space deliminated string of path components
 * @return xml::DocumentNode*
 *
 * pre-condition: root is valid
 * pre-condition: path only describes one path through node tree
 */
const DocumentNode* Document::getDescendant(
	const DocumentNode *root,
	const std::string &path
)const{
	assert(root);

	const DocumentNode *current = root;

	std::istringstream ist(path);
	std::string name;
	
	while(ist >> name){

		int count = std::count_if(
			current->children.begin(),
			current->children.end(),
			has_type(name)
		);
		if(1 != count){
			std::ostringstream oss;
			oss 
				<< "illegal path <" << path	<< ">@<" << name 
				<< ">: returned " << count << " options.";
			throw document_error(oss.str());
		}

		current = *(std::find_if(
			current->children.begin(), 
			current->children.end(), 
			has_type(name)
		));
	}
	return current;
}

/**
 * Look up document node by path
 *
 * @param node node to check
 * @param key name of attribute
 * @return bool
 *
 * pre-condition: node is a valid
 */
bool Document::attributeExists(
	const DocumentNode *node, 
	const std::string &key
)const{
	assert(node);
	return node->attributes.count(key);
}

/**
 * Look up value of a nodes attribute
 *
 * @param node node containing attributes
 * @param key name of attribute
 * @return std::string
 *
 * pre-condition: node is a valid
 *
 * throws experiment_error if attribute does not exist
 */
std::string Document::getAttributeValue(
		const DocumentNode *node
	  , const std::string  &key
)const{
	assert(node);

	AttribMapCIt ait = node->attributes.find(key);
	if ( node->attributes.end() == ait ) {
		std::string msg;
		msg += 
				"illegal attribute <" 
			  + key                + ">@<" 
			  + node->type         + ">@<" 
			  + node->parent->type + ">";
		throw document_error( msg );
	}
	return ait->second;
}

/**
 * Lookup desendant node and return it's (simple) content
 *
 * @param root starting node for lookup
 * @param path a space deliminated string of path components
 * @return std::string
 *
 * pre-condition: root is a valid
 *
 * @sa getIntegerContent(), getDescendant()
 */
std::string Document::getStringContent(
	const DocumentNode *root,
	const std::string& path
)const{
	return getDescendant(root, path)->content;
}

/**
 * Lookup desendant node and return it's (simple) content converted to integer
 *
 * @param root starting node for lookup
 * @param path a space deliminated string of path components
 * @return int
 *
 * pre-condition: root is a valid
 *
 * @sa getStringContent(), getDescendant()
 */
int Document::getIntegerContent(
	const DocumentNode *root,
	const std::string& path
)const{
	std::istringstream ist(getStringContent(root, path));
	int value;
	ist >> value;
	return value;
}

/**
 * Lookup desendant node and return it's (simple) content converted to decimal
 *
 * @param root starting node for lookup
 * @param path a space deliminated string of path components
 * @return double
 *
 * pre-condition: root is a valid
 *
 * @sa getStringContent(), getDescendant()
 */
double Document::getDecimalContent(
	const DocumentNode *root,
	const std::string& path
)const{
	std::istringstream ist(getStringContent(root, path));
	double value;
	ist >> value;
	return value;
}

//------------------------------------------------------------------------------
