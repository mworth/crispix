#ifndef XmlFile_H
#define XmlFile_H
// XmlFile.h: interface for the XmlFile class.
//
// Adapted from http://www.codeproject.com/cpp/xmlite.asp.
// On 2004-02-09 Cho, Kyung-Min gave us permission to use and modify this 
// library.
//
// XmlFile : XML Lite Parser Library
// by bro ( Cho,Kyung Min: bro@shinbiro.com ) 2002-10-30
// History.
// 2002-10-29 : First Coded. Parsing XMLElelement and Attributes.
//              get xml parsed string ( looks good )
// 2002-10-30 : Get Node Functions, error handling ( not completed )
// 2002-12-06 : Helper Funtion string to long
// 2002-12-12 : Entity Helper Support
// 2003-04-08 : Close, 
// 2003-07=23 : add property escape_value. (now no escape on default)
//              fix escape functions
//////////////////////////////////////////////////////////////////////

#include <vector>
struct DateTime;
class RageFile;

struct XAttr;
typedef std::vector<XAttr*> XAttrs;
struct XNode;
typedef std::vector<XNode*> XNodes;

// Entity Encode/Decode Support
struct XENTITY
{
	char entity;					// entity ( & " ' < > )
	char ref[10];					// entity reference ( &amp; &quot; etc )
	int ref_len;					// entity reference length
};

struct XENTITYS : public std::vector<XENTITY>
{
	XENTITY *GetEntity( int entity );
	XENTITY *GetEntity( char* entity );	
	int GetEntityCount( const char* str );
	int Ref2Entity( const char* estr, char* str, int strlen );
	int Entity2Ref( const char* str, char* estr, int estrlen );
	CString Ref2Entity( const char* estr );
	CString Entity2Ref( const char* str );	

	XENTITYS(){};
	XENTITYS( XENTITY *entities, int count );
};
extern XENTITYS entityDefault;
CString XRef2Entity( const char* estr );
CString XEntity2Ref( const char* str );	

enum PCODE
{
	PIE_PARSE_WELFORMED	= 0,
	PIE_ALONE_NOT_CLOSED,
	PIE_NOT_CLOSED,
	PIE_NOT_NESTED,
	PIE_ATTR_NO_VALUE,
	PIE_READ_ERROR
};

// Parse info.
struct PARSEINFO
{
	bool		trim_value;			// [set] do trim when parse?
	bool		entity_value;		// [set] do convert from reference to entity? ( &lt; -> < )
	XENTITYS	*entitys;			// [set] entity table for entity decode
	char		escape_value;		// [set] escape value (default '\\')

	char*		xml;				// [get] xml source
	bool		erorr_occur;		// [get] is occurance of error?
	char*		error_pointer;		// [get] error position of xml source
	PCODE		error_code;			// [get] error code
	CString		error_string;		// [get] error string

	PARSEINFO() { trim_value = true; entity_value = true; entitys = &entityDefault; xml = NULL; erorr_occur = false; error_pointer = NULL; error_code = PIE_PARSE_WELFORMED; escape_value = 0; }
};
extern PARSEINFO piDefault;

// display optional environment
struct DISP_OPT
{
	bool newline;			// newline when new tag
	bool reference_value;	// do convert from entity to reference ( < -> &lt; )
	XENTITYS	*entitys;	// entity table for entity encode
	CString stylesheet;		// empty string = no stylesheet
	bool write_tabs;		// if false, don't write tab indent characters

	int tab_base;			// internal usage
	DISP_OPT()
	{
		newline = true;
		reference_value = true;
		entitys = &entityDefault;
		stylesheet = "";
		write_tabs = true;
		tab_base = 0;
	}
};
extern DISP_OPT optDefault;

// XAttr : Attribute Implementation
struct XAttr
{
	CString name;
	CString	value;
	void GetValue(CString &out) const;
	void GetValue(int &out) const;
	void GetValue(float &out) const;
	void GetValue(bool &out) const;
	void GetValue(unsigned &out) const;
	void GetValue(DateTime &out) const;
	
	XNode*	parent;

	bool GetXML( RageFile &f, DISP_OPT *opt = &optDefault );
};

// XMLNode structure
struct XNode
{
	// name and value
	CString name;
	CString	value;
	void GetValue(CString &out) const;
	void GetValue(int &out) const;
	void GetValue(float &out) const;
	void GetValue(bool &out) const;
	void GetValue(unsigned &out) const;
	void GetValue(DateTime &out) const;
	void SetValue(int v);
	void SetValue(float v);
	void SetValue(bool v);
	void SetValue(unsigned v);
	void SetValue(const DateTime &v);

	// internal variables
	XNode	*parent;		// parent node
	XNodes	childs;		// child node
	XAttrs	attrs;		// attributes

	// Load/Save XML
	char*	Load( const char* pszXml, PARSEINFO *pi = &piDefault );
	char*	LoadAttributes( const char* pszAttrs, PARSEINFO *pi = &piDefault );
	bool GetXML( RageFile &f, DISP_OPT *opt = &optDefault );

	bool LoadFromFile( CString sFile, PARSEINFO *pi = &piDefault );
	bool SaveToFile( CString sFile, DISP_OPT *opt = &optDefault );

	// in own attribute list
	const XAttr *GetAttr( const char* attrname ) const; 
	XAttr *GetAttr( const char* attrname ); 
	const char*	GetAttrValue( const char* attrname ); 
	bool GetAttrValue(const char* name,CString &out) const	{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	bool GetAttrValue(const char* name,int &out) const		{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	bool GetAttrValue(const char* name,float &out) const	{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	bool GetAttrValue(const char* name,bool &out) const		{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	bool GetAttrValue(const char* name,unsigned &out) const	{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	bool GetAttrValue(const char* name,DateTime &out) const	{ const XAttr* pAttr=GetAttr(name); if(pAttr==NULL) return false; pAttr->GetValue(out); return true; }
	XAttrs	GetAttrs( const char* name ); 

	// in one level child nodes
	const XNode *GetChild( const char* name ) const; 
	XNode *GetChild( const char* name ); 
	const char*	GetChildValue( const char* name ); 
	bool GetChildValue(const char* name,CString &out) const	{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	bool GetChildValue(const char* name,int &out) const		{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	bool GetChildValue(const char* name,float &out) const	{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	bool GetChildValue(const char* name,bool &out) const	{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	bool GetChildValue(const char* name,unsigned &out) const{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	bool GetChildValue(const char* name,DateTime &out) const{ const XNode* pChild=GetChild(name); if(pChild==NULL) return false; pChild->GetValue(out); return true; }
	XNodes	GetChilds( const char* name ); 
	XNodes	GetChilds(); 

	XAttr *GetChildAttr( const char* name, const char* attrname );
	const char* GetChildAttrValue( const char* name, const char* attrname );
	
	// modify DOM 
	int		GetChildCount();
	XNode *GetChild( int i );
	XNodes::iterator GetChildIterator( XNode *node );
	XNode *CreateNode( const char* name = NULL, const char* value = NULL );
	XNode	*AppendChild( const char* name = NULL, const char* value = NULL );
	XNode	*AppendChild( const char* name, float value );
	XNode	*AppendChild( const char* name, int value );
	XNode	*AppendChild( const char* name, unsigned value );
	XNode	*AppendChild( const char* name, const DateTime &value );
	XNode	*AppendChild( XNode *node );
	bool	RemoveChild( XNode *node );
	XNode *DetachChild( XNode *node );


	XAttr *GetAttr( int i );
	XAttrs::iterator GetAttrIterator( XAttr *node );
	XAttr *CreateAttr( const char* anem = NULL, const char* value = NULL );
	XAttr *AppendAttr( const char* name = NULL, const char* value = NULL );
	XAttr *AppendAttr( const char* name, float value );
	XAttr *AppendAttr( const char* name, int value );
	XAttr *AppendAttr( const char* name, unsigned value );
	XAttr *AppendAttr( const char* name, const DateTime &value );
	XAttr	*AppendAttr( XAttr *attr );
	bool	RemoveAttr( XAttr *attr );
	XAttr *DetachAttr( XAttr *attr );

	// operator overloads
	XNode* operator [] ( int i ) { return GetChild(i); }

	XNode() { parent = NULL; }
	~XNode();

	void Close();
};

// Helper Funtion
inline long XStr2Int( const char* str, long default_value = 0 )
{
	return str ? atol(str) : default_value;
}

bool XIsEmptyString( const char* str );

#endif
