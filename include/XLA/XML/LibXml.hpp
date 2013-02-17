#ifndef RAPIDXML_HPP_INCLUDED
#define RAPIDXML_HPP_INCLUDED


#if !defined(RAPIDXML_NO_STDLIB)
#include <cstdlib>      // For Size
#include <cassert>      // For assert
#include <new>          // For placement new
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127)   // Conditional expression is constant
#endif


#include <XLA/TypeDefine.hpp>
#include <XLA/IO/StreamReader.hpp>
#include <XLA/Exception.hpp>

#define RAPIDXML_NO_EXCEPTIONS

#if defined(RAPIDXML_NO_EXCEPTIONS)

namespace XLA
{
	class XmlException
		: public RunTimeException
	{
	public:
		XLA_REGEDIT_CLASS( XmlException );

		XmlException()
		{

		}
	};
}

#define XLA_XML_THROWEXP(what, where) { ExpThrow( XmlException() ); }

namespace XLA
{
	inline void parse_error_handler(const char *what, void *where)
	{
	}




}

#else


#endif

///////////////////////////////////////////////////////////////////////////
// Pool sizes

#ifndef RAPIDXML_STATIC_POOL_SIZE
// Size of static memory block of memory_pool.
// Define RAPIDXML_STATIC_POOL_SIZE before including rapidxml.hpp if you want to override the default value.
// No dynamic memory allocations are performed by memory_pool until static memory is exhausted.
#define RAPIDXML_STATIC_POOL_SIZE (64 * 1024)
#endif

#ifndef RAPIDXML_DYNAMIC_POOL_SIZE
// Size of dynamic memory block of memory_pool.
// Define RAPIDXML_DYNAMIC_POOL_SIZE before including rapidxml.hpp if you want to override the default value.
// After the static block is exhausted, dynamic blocks with approximately this size are allocated by memory_pool.
#define RAPIDXML_DYNAMIC_POOL_SIZE (64 * 1024)
#endif

#ifndef RAPIDXML_ALIGNMENT
// Memory allocation alignment.
// Define RAPIDXML_ALIGNMENT before including rapidxml.hpp if you want to override the default value, which is the size of pointer.
// All memory allocations for nodes, attributes and strings will be aligned to this value.
// This must be a power of 2 and at least 1, otherwise memory_pool will not work.
#define RAPIDXML_ALIGNMENT sizeof(void *)
#endif

namespace XLA
{

	class XmlNode;
	class XmlAttribute;
	class XmlDocument;

	typedef const XmlNode CXmlNode;
	typedef const XmlAttribute CXmlAttribute;


	enum ENodeType
	{
		XmlNodeDocument,      //!< A document node. Name and value are empty.
		XmlNodeElement,       //!< An element node. Name contains element name. Value contains text of first data node.
		XmlNodeData,          //!< A data node. Name is empty. Value contains data text.
		XmlNodeCData,         //!< A CDATA node. Name is empty. Value contains data text.
		XmlNodeComment,       //!< A comment node. Name is empty. Value contains comment text.
		XmlNodeDeclaration,   //!< A declaration node. Name and value are empty. Declaration parameters (version, encoding and standalone) are in node attributes.
		XmlNodeDoctype,       //!< A DOCTYPE node. Name is empty. Value contains DOCTYPE text.
		XmlNodePI,             //!< A PI node. Name contains target. Value contains instructions.
		XmlDataElement
	};


	const int parse_no_data_nodes = 0x1;            
	const int parse_no_element_values = 0x2;
	const int parse_no_string_terminators = 0x4;
	const int parse_no_entity_translation = 0x8;
	const int parse_no_utf8 = 0x10;
	const int parse_declaration_node = 0x20;
	const int parse_comment_nodes = 0x40;
	const int parse_doctype_node = 0x80;
	const int parse_pi_nodes = 0x100;
	const int parse_validate_closing_tags = 0x200;
	const int parse_trim_whitespace = 0x400;
	const int parse_normalize_whitespace = 0x800;
	const int parse_default = 0;
	const int parse_non_destructive = parse_no_string_terminators | parse_no_entity_translation;
	const int parse_fastest = parse_non_destructive | parse_no_data_nodes;
	const int parse_full = parse_declaration_node | parse_comment_nodes | parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;


	namespace XmlUnit
	{

		template<int Dummy>
		struct XmlByteTable
		{
			static const Byte lookup_whitespace[256];              // Whitespace table
			static const Byte lookup_node_name[256];               // Node name table
			static const Byte lookup_text[256];                    // Text table
			static const Byte lookup_text_pure_no_ws[256];         // Text table
			static const Byte lookup_text_pure_with_ws[256];       // Text table
			static const Byte lookup_attribute_name[256];          // Attribute name table
			static const Byte lookup_attribute_data_1[256];        // Attribute data table with single quote
			static const Byte lookup_attribute_data_1_pure[256];   // Attribute data table with single quote
			static const Byte lookup_attribute_data_2[256];        // Attribute data table with double quotes
			static const Byte lookup_attribute_data_2_pure[256];   // Attribute data table with double quotes
			static const Byte lookup_digits[256];                  // Digits
			static const Byte lookup_upcase[256];                  // To uppercase conversion table for ASCII characters
		};


		inline Size GetSize(const Char *p)
		{
			const Char *tmp = p;
			while (*tmp) 
				++tmp;
			return tmp - p;
		}

		inline bool StringCompare(const Char *p1, Size size1, const Char *p2, Size size2, bool case_sensitive)
		{
			if (size1 != size2)
				return false;
			if (case_sensitive)
			{
				for (const Char *end = p1 + size1; p1 < end; ++p1, ++p2)
					if (*p1 != *p2)
						return false;
			}
			else
			{
				for (const Char *end = p1 + size1; p1 < end; ++p1, ++p2)
					if (XmlByteTable<0>::lookup_upcase[static_cast<Byte>(*p1)] != XmlByteTable<0>::lookup_upcase[static_cast<Byte>(*p2)])
						return false;
			}
			return true;
		}
	}



	class XmlMemoryPool
	{

	public:

		typedef void *(alloc_func)(Size);       // Type of user-defined function used to allocate memory
		typedef void (free_func)(void *);              // Type of user-defined function used to free memory

		XmlMemoryPool()
			: m_alloc_func(0)
			, m_free_func(0)
		{
			Initalize();
		}

		~XmlMemoryPool()
		{
			Uninitalize();
		}

		XmlNode *AllocateNode(ENodeType type, 
			const Char *name = 0, const Char *value = 0, 
			Size name_size = 0, Size value_size = 0);

		XmlAttribute *AllocAttribute(const Char *name = 0, const Char *value = 0, 
			Size name_size = 0, Size value_size = 0);

		Char *AllocateString(const Char *source = 0, Size size = 0);

		XmlNode *CloneNode(const XmlNode *source, XmlNode *result = 0);

		void Uninitalize()
		{
			while (m_begin != m_static_memory)
			{
				char *previous_begin = reinterpret_cast<header *>(MemoryAlign(m_begin))->previous_begin;
				if (m_free_func)
					m_free_func(m_begin);
				else
					delete[] m_begin;
				m_begin = previous_begin;
			}
			Initalize();
		}

		void SetAllocate(alloc_func *af, free_func *ff)
		{
			assert(m_begin == m_static_memory && m_ptr == MemoryAlign(m_begin));    // Verify that no memory is allocated yet
			m_alloc_func = af;
			m_free_func = ff;
		}

	private:

		struct header
		{
			char *previous_begin;
		};

		void Initalize()
		{
			m_begin = m_static_memory;
			m_ptr = MemoryAlign(m_begin);
			m_end = m_static_memory + sizeof(m_static_memory);
		}

		char *MemoryAlign(char *ptr)
		{
			Size alignment = ((RAPIDXML_ALIGNMENT - (Size(ptr) & (RAPIDXML_ALIGNMENT - 1))) & (RAPIDXML_ALIGNMENT - 1));
			return ptr + alignment;
		}

		char *AllocateData(Size size)
		{
			// Allocate
			void *memory;   
			if (m_alloc_func)   // Allocate memory using either user-specified allocation function or global operator new[]
			{
				memory = m_alloc_func(size);
				assert(memory); // Allocator is not allowed to return 0, on failure it must either throw, stop the program or use longjmp
			}
			else
			{
				memory = new char[size];
#ifdef RAPIDXML_NO_EXCEPTIONS
				if (!memory)            // If exceptions are disabled, verify memory allocation, because new will not be able to throw bad_alloc
					XLA_XML_THROWEXP("out of memory", 0);
#endif
			}
			return static_cast<char *>(memory);
		}

		void *AllocateAligned(Size size)
		{
			// Calculate aligned pointer
			char *result = MemoryAlign(m_ptr);

			// If not enough memory left in current pool, allocate a new pool
			if (result + size > m_end)
			{
				// Calculate required pool size (may be bigger than RAPIDXML_DYNAMIC_POOL_SIZE)
				Size pool_size = RAPIDXML_DYNAMIC_POOL_SIZE;
				if (pool_size < size)
					pool_size = size;

				// Allocate
				Size alloc_size = sizeof(header) + (2 * RAPIDXML_ALIGNMENT - 2) + pool_size;     // 2 alignments required in worst case: one for header, one for actual allocation
				char *raw_memory = AllocateData(alloc_size);

				// Setup new pool in allocated memory
				char *pool = MemoryAlign(raw_memory);
				header *new_header = reinterpret_cast<header *>(pool);
				new_header->previous_begin = m_begin;
				m_begin = raw_memory;
				m_ptr = pool + sizeof(header);
				m_end = raw_memory + alloc_size;

				// Calculate aligned pointer again using new pool
				result = MemoryAlign(m_ptr);
			}

			// Update pool and return aligned pointer
			m_ptr = result + size;
			return result;
		}

		char *m_begin;                                      // Start of raw memory making up current pool
		char *m_ptr;                                        // First free byte in current pool
		char *m_end;                                        // One past last available byte in current pool
		char m_static_memory[RAPIDXML_STATIC_POOL_SIZE];    // Static raw memory
		alloc_func *m_alloc_func;                           // Allocator function, or 0 if default is to be used
		free_func *m_free_func;                             // Free function, or 0 if default is to be used
	};



	class XmlData
	{

	public:


		XmlData()
			: _name(0)
			, _value(0)
			, _parent(0)
		{
		}

		Char *NodeName() const
		{
			return _name ? _name : GetNullString();
		}


		Size NodeNameSize() const
		{
			return _name ? _nameSize : 0;
		}

		Char *NodeValue() const
		{
			return _value ? _value : GetNullString();
		}


		Size NodeValueSize() const
		{
			return _value ? _valueSize : 0;
		}

		String GetValue() const
		{
			return String( NodeValue() , NodeValueSize() );
		}

		void NodeName(const Char *name, Size size)
		{
			_name = const_cast<Char *>(name);
			_nameSize = size;
		}

		void NodeName(const Char *name)
		{
			this->NodeName(name, XmlUnit::GetSize(name));
		}

		void NodeValue(const Char *value, Size size)
		{
			_value = const_cast<Char *>(value);
			_valueSize = size;
		}

		void NodeValue(const Char *value)
		{
			this->NodeValue(value, XmlUnit::GetSize(value));
		}

		XmlNode *GetParent() const
		{
			return _parent;
		}

	protected:

		static Char *GetNullString()
		{
			static Char zero = Char('\0');
			return &zero;
		}

		Char*    _name;                         // Name of node, or 0 if no name
		Char*    _value;                        // Value of node, or 0 if no value
		Size     _nameSize;            // Length of node name, or undefined of no name
		Size     _valueSize;           // Length of node value, or undefined if no value
		XmlNode* _parent;             // Pointer to parent node, or 0 if none

	};

	class XmlAttribute: public XmlData
	{

		friend class XmlNode;

	public:

		XmlAttribute()
		{
		}

		XmlDocument *GetDocument() const;

		XmlAttribute *Previous(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlAttribute *attribute = _prev; attribute; attribute = attribute->_prev)
					if (XmlUnit::StringCompare(attribute->NodeName(), attribute->NodeNameSize(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return this->_parent ? _prev : 0;
		}

		XmlAttribute *GetNext(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlAttribute *attribute = _next; attribute; attribute = attribute->_next)
					if (XmlUnit::StringCompare(attribute->NodeName(), attribute->NodeNameSize(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return this->_parent ? _next : 0;
		}

	private:

		XmlAttribute *_prev;
		XmlAttribute *_next;

	};

	class XmlNodeEnumator;

	class XmlNode: public XmlData
	{

	public:

		XmlNode(ENodeType type)
			: _type(type)
			, _first(0)
			, _firstAttribute(0)
		{
		}

		XmlNodeEnumator GetEnumator( CBool& recursive = true ) const;

		ENodeType GetType() const
		{
			return _type;
		}

		Bool DataElement() const
		{
			if( _type == XmlNodeElement )
			{
				if( _first == 0 )
					return true;
				if( _first == _last )
				{
					if( _first->GetType() != XmlNodeElement )
						return true;
				}
			}

			return false;
		}

		String GetName() const
		{
			ENodeType type = GetType();
			if( type == XmlNodeCData || type == XmlNodeData  )
			{
				XmlNode* xmlNode = GetParent();
				return xmlNode==0?String():xmlNode->GetName();
			}
			return String( NodeName() , NodeNameSize() );
		}

		XmlDocument *GetDocument() const
		{
			XmlNode *node = const_cast<XmlNode *>(this);
			while (node->GetParent())
				node = node->GetParent();
			return node->GetType() == XmlNodeDocument ? (XmlDocument*)(node) : 0;
		}
	
		XmlNode* GetFirst(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlNode *child = _first; child; child = child->Next())
					if (XmlUnit::StringCompare(child->NodeName(), child->NodeNameSize(), name, name_size, case_sensitive))
						return child;
				return 0;
			}
			else
				return _first;
		}

			
		XmlNode * GetLast(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			assert(_first);  // Cannot query for last child if node has no children
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlNode *child = _last; child; child = child->GetPrevious())
					if (XmlUnit::StringCompare(child->NodeName(), child->NodeNameSize(), name, name_size, case_sensitive))
						return child;
				return 0;
			}
			else
				return _last;
		}

		XmlNode *GetPrevious(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			assert(this->_parent);     // Cannot query for siblings if node has no parent
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlNode *sibling = _prev; sibling; sibling = sibling->_prev)
					if (XmlUnit::StringCompare(sibling->NodeName(), sibling->NodeNameSize(), name, name_size, case_sensitive))
						return sibling;
				return 0;
			}
			else
				return _prev;
		}

		XmlNode *Next(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			assert(this->_parent);     // Cannot query for siblings if node has no parent
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlNode *sibling = _next; sibling; sibling = sibling->_next)
					if (XmlUnit::StringCompare(sibling->NodeName(), sibling->NodeNameSize(), name, name_size, case_sensitive))
						return sibling;
				return 0;
			}
			else
				return _next;
		}

		XmlAttribute* GetFirstAttribute(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlAttribute *attribute = _firstAttribute; attribute; attribute = attribute->_next)
					if (XmlUnit::StringCompare(attribute->NodeName(), attribute->NodeNameSize(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return _firstAttribute;
		}

		XmlAttribute* GetLastAttribute(const Char *name = 0, Size name_size = 0, bool case_sensitive = true) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = XmlUnit::GetSize(name);
				for (XmlAttribute *attribute = lastAttribute; attribute; attribute = attribute->_prev)
					if (XmlUnit::StringCompare(attribute->NodeName(), attribute->NodeNameSize(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return _firstAttribute ? lastAttribute : 0;
		}

		void SetType(ENodeType type)
		{
			_type = type;
		}

		void Prepend(XmlNode *child)
		{
			assert(child && !child->GetParent() && child->GetType() != XmlNodeDocument);
			if (GetFirst())
			{
				child->_next = _first;
				_first->_prev = child;
			}
			else
			{
				child->_next = 0;
				_last = child;
			}
			_first = child;
			child->_parent = this;
			child->_prev = 0;
		}

		void Append(XmlNode *child)
		{
			assert(child && !child->GetParent() && child->GetType() != XmlNodeDocument);
			if (GetFirst())
			{
				child->_prev = _last;
				_last->_next = child;
			}
			else
			{
				child->_prev = 0;
				_first = child;
			}
			_last = child;
			child->_parent = this;
			child->_next = 0;
		}

		void Insert(XmlNode *where, XmlNode *child)
		{
			assert(!where || where->GetParent() == this);
			assert(child && !child->GetParent() && child->GetType() != XmlNodeDocument);
			if (where == _first)
				Prepend(child);
			else if (where == 0)
				Append(child);
			else
			{
				child->_prev = where->_prev;
				child->_next = where;
				where->_prev->_next = child;
				where->_prev = child;
				child->_parent = this;
			}
		}

		void RemoveFirst()
		{
			assert(GetFirst());
			XmlNode *child = _first;
			_first = child->_next;
			if (child->_next)
				child->_next->_prev = 0;
			else
				_last = 0;
			child->_parent = 0;
		}

		void RemoveLast()
		{
			assert(GetFirst());
			XmlNode *child = _last;
			if (child->_prev)
			{
				_last = child->_prev;
				child->_prev->_next = 0;
			}
			else
				_first = 0;
			child->_parent = 0;
		}

		void Remove(XmlNode *where)
		{
			assert(where && where->GetParent() == this);
			assert(GetFirst());
			if (where == _first)
				RemoveFirst();
			else if (where == _last)
				RemoveLast();
			else
			{
				where->_prev->_next = where->_next;
				where->_next->_prev = where->_prev;
				where->_parent = 0;
			}
		}

		void RemoveAll()
		{
			for (XmlNode *node = GetFirst(); node; node = node->_next)
				node->_parent = 0;
			_first = 0;
		}

		void PrependAttribute(XmlAttribute *attribute)
		{
			assert(attribute && !attribute->GetParent());
			if (GetFirstAttribute())
			{
				attribute->_next = _firstAttribute;
				_firstAttribute->_prev = attribute;
			}
			else
			{
				attribute->_next = 0;
				lastAttribute = attribute;
			}
			_firstAttribute = attribute;
			attribute->_parent = this;
			attribute->_prev = 0;
		}

		void AppendAttribute(XmlAttribute *attribute)
		{
			assert(attribute && !attribute->GetParent());
			if (GetFirstAttribute())
			{
				attribute->_prev = lastAttribute;
				lastAttribute->_next = attribute;
			}
			else
			{
				attribute->_prev = 0;
				_firstAttribute = attribute;
			}
			lastAttribute = attribute;
			attribute->_parent = this;
			attribute->_next = 0;
		}

	
		void InsertAttribute(XmlAttribute *where, XmlAttribute *attribute)
		{
			assert(!where || where->GetParent() == this);
			assert(attribute && !attribute->GetParent());
			if (where == _firstAttribute)
				PrependAttribute(attribute);
			else if (where == 0)
				AppendAttribute(attribute);
			else
			{
				attribute->_prev = where->_prev;
				attribute->_next = where;
				where->_prev->_next = attribute;
				where->_prev = attribute;
				attribute->_parent = this;
			}
		}


		void RemoveFirstAttribute()
		{
			assert(GetFirstAttribute());
			XmlAttribute *attribute = _firstAttribute;
			if (attribute->_next)
			{
				attribute->_next->_prev = 0;
			}
			else
				lastAttribute = 0;
			attribute->_parent = 0;
			_firstAttribute = attribute->_next;
		}


		void RemoveLastAttribute()
		{
			assert(GetFirstAttribute());
			XmlAttribute *attribute = lastAttribute;
			if (attribute->_prev)
			{
				attribute->_prev->_next = 0;
				lastAttribute = attribute->_prev;
			}
			else
				_firstAttribute = 0;
			attribute->_parent = 0;
		}

		void RemoveAttribute(XmlAttribute *where)
		{
			assert(GetFirstAttribute() && where->GetParent() == this);
			if (where == _firstAttribute)
				RemoveFirstAttribute();
			else if (where == lastAttribute)
				RemoveLastAttribute();
			else
			{
				where->_prev->_next = where->_next;
				where->_next->_prev = where->_prev;
				where->_parent = 0;
			}
		}


		void RemoveAllAttributes()
		{
			for (XmlAttribute *attribute = GetFirstAttribute(); attribute; attribute = attribute->_next)
				attribute->_parent = 0;
			_firstAttribute = 0;
		}

	private:


		XmlNode(const XmlNode &);
		void operator =(const XmlNode &);


		ENodeType _type;              
		XmlNode * _first;             
		XmlNode * _last;              
		XmlAttribute *_firstAttribute;  
		XmlAttribute *lastAttribute;   
		XmlNode *_prev;           
		XmlNode *_next;          

	};

	class XmlNodeEnumator
	{
		CXmlNode* _token;
		CXmlNode* _current;
		Bool      _recursive;
		UInt32    _deep;

		Bool SignalMoveNext()
		{
			_current = _token;
			if( _token == 0 )
				return false;
			_token = _token->Next();
			return true;
		}

		Bool RecursiveMoveNext()
		{

			if( _token == 0 )
				return false;
			_current = _token;

			CXmlNode* nodeNext = _token;

			if( _token->GetType() == XmlNodeElement )
			{
				nodeNext = _token->GetFirst();
				if( nodeNext != 0 )
				{
					_token = nodeNext;
					_deep  = _deep + 1;
					return true;
				}
			}

			nodeNext = _token->Next();

			while( nodeNext == 0 && _deep > 0 )
			{
				nodeNext = _token->GetParent();
				_deep--;
				if( nodeNext != 0 )
					nodeNext = nodeNext->Next();

				if( nodeNext == 0 )
					continue;

				_token = nodeNext;
			}

			_token = nodeNext;

			
			return true;
		}
	public:
		XmlNodeEnumator( CXmlNode* token , CBool& recursive )
			: _token( token )
			, _current(token)
			, _recursive( recursive )
			,_deep(0)
		{

		}

		Bool MoveNext()
		{
			if( !_recursive )
				return SignalMoveNext();
			return RecursiveMoveNext();
		}

		CXmlNode* GetCurrent() const
		{
			return _current;
		}

		CString GetName() const;
		CString GetValue() const;
		ENodeType GetType() const;
		Bool DataElement() const;
	};





	class XmlDocument: 
		public XmlNode, 
		public XmlMemoryPool,
		public Object
	{

	public:
		XLA_REGEDIT_CLASS( XmlDocument );

		void LoadFromFile( CString& name )
		{
			StreamReader xmlReader( name , Encoding::UTF8() , true );

			String xmlData = xmlReader.ReadToEnd();

			xmlReader.Close();

			Char* xmlString = AllocateString( xmlData.c_str() , xmlData.size() );

			LocalParse<0>( xmlString );
		}

		XmlDocument()
			: XmlNode(XmlNodeDocument)
		{
		}

		template<int Flags>
		void LocalParse(Char *text)
		{
			assert(text);

			// Remove current contents
			this->RemoveAll();
			this->RemoveAllAttributes();

			// Parse BOM, if any
			LocalParseBOM<Flags>(text);

			// Parse children
			while (1)
			{
				// Skip whitespace before node
				Skip<whitespace_pred, Flags>(text);
				if (*text == 0)
					break;

				// Parse and append new child
				if (*text == Char('<'))
				{
					++text;     // Skip '<'
					if (XmlNode *node = LocalParseNode<Flags>(text))
						this->Append(node);
				}
				else
					XLA_XML_THROWEXP("expected <", text);
			}

		}


		void Uninitalize()
		{
			this->RemoveAll();
			this->RemoveAllAttributes();
			XmlMemoryPool::Uninitalize();
		}

	private:


		struct whitespace_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_whitespace[static_cast<Byte>(ch)];
			}
		};

	
		struct node_name_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_node_name[static_cast<Byte>(ch)];
			}
		};

	
		struct attribute_name_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_attribute_name[static_cast<Byte>(ch)];
			}
		};

	
		struct text_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_text[static_cast<Byte>(ch)];
			}
		};

	
		struct text_pure_no_ws_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_text_pure_no_ws[static_cast<Byte>(ch)];
			}
		};

	
		struct text_pure_with_ws_pred
		{
			static Byte test(Char ch)
			{
				return XmlUnit::XmlByteTable<0>::lookup_text_pure_with_ws[static_cast<Byte>(ch)];
			}
		};

		template<Char Quote>
		struct attribute_value_pred
		{
			static Byte test(Char ch)
			{
				if (Quote == Char('\''))
					return XmlUnit::XmlByteTable<0>::lookup_attribute_data_1[static_cast<Byte>(ch)];
				if (Quote == Char('\"'))
					return XmlUnit::XmlByteTable<0>::lookup_attribute_data_2[static_cast<Byte>(ch)];
				return 0;       // Should never be executed, to avoid warnings on Comeau
			}
		};

		
		template<Char Quote>
		struct attribute_value_pure_pred
		{
			static Byte test(Char ch)
			{
				if (Quote == Char('\''))
					return XmlUnit::XmlByteTable<0>::lookup_attribute_data_1_pure[static_cast<Byte>(ch)];
				if (Quote == Char('\"'))
					return XmlUnit::XmlByteTable<0>::lookup_attribute_data_2_pure[static_cast<Byte>(ch)];
				return 0;       // Should never be executed, to avoid warnings on Comeau
			}
		};

		
		template<int Flags>
		static void LocalInsertCodedCharacter(Char *&text, unsigned long code)
		{
			if (Flags & parse_no_utf8)
			{
				// Insert 8-bit ASCII character
				// Todo: possibly verify that code is less than 256 and use replacement char otherwise?
				text[0] = static_cast<Byte>(code);
				text += 1;
			}
			else
			{
				// Insert UTF8 sequence
				if (code < 0x80)    // 1 byte sequence
				{
					text[0] = static_cast<Byte>(code);
					text += 1;
				}
				else if (code < 0x800)  // 2 byte sequence
				{
					text[1] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[0] = static_cast<Byte>(code | 0xC0);
					text += 2;
				}
				else if (code < 0x10000)    // 3 byte sequence
				{
					text[2] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[1] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[0] = static_cast<Byte>(code | 0xE0);
					text += 3;
				}
				else if (code < 0x110000)   // 4 byte sequence
				{
					text[3] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[2] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[1] = static_cast<Byte>((code | 0x80) & 0xBF); code >>= 6;
					text[0] = static_cast<Byte>(code | 0xF0);
					text += 4;
				}
				else    // Invalid, only codes up to 0x10FFFF are allowed in Unicode
				{
					XLA_XML_THROWEXP("invalid numeric character entity", text);
				}
			}
		}

		
		template<class StopPred, int Flags>
		static void Skip(Char *&text)
		{
			Char *tmp = text;
			while (StopPred::test(*tmp))
				++tmp;
			text = tmp;
		}

	
		template<class StopPred, class StopPredPure, int Flags>
		static Char *LocalExpandCharacter(Char *&text)
		{
			// If entity translation, whitespace condense and whitespace trimming is disabled, use plain skip
			if (Flags & parse_no_entity_translation && 
				!(Flags & parse_normalize_whitespace) &&
				!(Flags & parse_trim_whitespace))
			{
				Skip<StopPred, Flags>(text);
				return text;
			}

			// Use simple skip until first modification is detected
			Skip<StopPredPure, Flags>(text);

			// Use translation skip
			Char *src = text;
			Char *dest = src;
			while (StopPred::test(*src))
			{
				// If entity translation is enabled    
				if (!(Flags & parse_no_entity_translation))
				{
					// Test if replacement is needed
					if (src[0] == Char('&'))
					{
						switch (src[1])
						{

							// &amp; &apos;
						case Char('a'): 
							if (src[2] == Char('m') && src[3] == Char('p') && src[4] == Char(';'))
							{
								*dest = Char('&');
								++dest;
								src += 5;
								continue;
							}
							if (src[2] == Char('p') && src[3] == Char('o') && src[4] == Char('s') && src[5] == Char(';'))
							{
								*dest = Char('\'');
								++dest;
								src += 6;
								continue;
							}
							break;

							// &quot;
						case Char('q'): 
							if (src[2] == Char('u') && src[3] == Char('o') && src[4] == Char('t') && src[5] == Char(';'))
							{
								*dest = Char('"');
								++dest;
								src += 6;
								continue;
							}
							break;

							// &gt;
						case Char('g'): 
							if (src[2] == Char('t') && src[3] == Char(';'))
							{
								*dest = Char('>');
								++dest;
								src += 4;
								continue;
							}
							break;

							// &lt;
						case Char('l'): 
							if (src[2] == Char('t') && src[3] == Char(';'))
							{
								*dest = Char('<');
								++dest;
								src += 4;
								continue;
							}
							break;

							// &#...; - assumes ASCII
						case Char('#'): 
							if (src[2] == Char('x'))
							{
								unsigned long code = 0;
								src += 3;   // Skip &#x
								while (1)
								{
									Byte digit = XmlUnit::XmlByteTable<0>::lookup_digits[static_cast<Byte>(*src)];
									if (digit == 0xFF)
										break;
									code = code * 16 + digit;
									++src;
								}
								LocalInsertCodedCharacter<Flags>(dest, code);    // Put character in output
							}
							else
							{
								unsigned long code = 0;
								src += 2;   // Skip &#
								while (1)
								{
									Byte digit = XmlUnit::XmlByteTable<0>::lookup_digits[static_cast<Byte>(*src)];
									if (digit == 0xFF)
										break;
									code = code * 10 + digit;
									++src;
								}
								LocalInsertCodedCharacter<Flags>(dest, code);    // Put character in output
							}
							if (*src == Char(';'))
								++src;
							else
								XLA_XML_THROWEXP("expected ;", src);
							continue;

							// Something else
						default:
							// Ignore, just copy '&' verbatim
							break;

						}
					}
				}

				// If whitespace condensing is enabled
				if (Flags & parse_normalize_whitespace)
				{
					// Test if condensing is needed                 
					if (whitespace_pred::test(*src))
					{
						*dest = Char(' '); ++dest;    // Put single space in dest
						++src;                      // Skip first whitespace char
						// Skip remaining whitespace chars
						while (whitespace_pred::test(*src))
							++src;
						continue;
					}
				}

				// No replacement, only copy character
				*dest++ = *src++;

			}

			// Return new end
			text = src;
			return dest;

		}

	
		template<int Flags>
		void LocalParseBOM(Char *&text)
		{
			// UTF-8?
			if (static_cast<Byte>(text[0]) == 0xEF && 
				static_cast<Byte>(text[1]) == 0xBB && 
				static_cast<Byte>(text[2]) == 0xBF)
			{
				text += 3;      // Skup utf-8 bom
			}
		}

	
		template<int Flags>
		XmlNode *LocalParseDeclaration(Char *&text)
		{
			// If parsing of declaration is disabled
			if (!(Flags & parse_declaration_node))
			{
				// Skip until end of declaration
				while (text[0] != Char('?') || text[1] != Char('>'))
				{
					if (!text[0])
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}
				text += 2;    // Skip '?>'
				return 0;
			}

			// Create declaration
			XmlNode *declaration = this->AllocateNode(XmlNodeDeclaration);

			// Skip whitespace before attributes or ?>
			Skip<whitespace_pred, Flags>(text);

			// Parse declaration attributes
			LocalParseAttribute<Flags>(text, declaration);

			// Skip ?>
			if (text[0] != Char('?') || text[1] != Char('>'))
				XLA_XML_THROWEXP("expected ?>", text);
			text += 2;

			return declaration;
		}

		
		template<int Flags>
		XmlNode *LocalParseComment(Char *&text)
		{
			// If parsing of comments is disabled
			if (!(Flags & parse_comment_nodes))
			{
				// Skip until end of comment
				while (text[0] != Char('-') || text[1] != Char('-') || text[2] != Char('>'))
				{
					if (!text[0])
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}
				text += 3;     // Skip '-->'
				return 0;      // Do not produce comment node
			}

			// Remember value start
			Char *value = text;

			// Skip until end of comment
			while (text[0] != Char('-') || text[1] != Char('-') || text[2] != Char('>'))
			{
				if (!text[0])
					XLA_XML_THROWEXP("unexpected end of data", text);
				++text;
			}

			// Create comment node
			XmlNode *comment = this->AllocateNode(XmlNodeComment);
			comment->NodeValue(value, text - value);

			// Place zero terminator after comment value
			if (!(Flags & parse_no_string_terminators))
				*text = Char('\0');

			text += 3;     // Skip '-->'
			return comment;
		}

		
		template<int Flags>
		XmlNode *LocalParseDoctype(Char *&text)
		{
			// Remember value start
			Char *value = text;

			// Skip to >
			while (*text != Char('>'))
			{
				// Determine character type
				switch (*text)
				{

					// If '[' encountered, scan for matching ending ']' using naive algorithm with depth
					// This works for all W3C test files except for 2 most wicked
				case Char('['):
					{
						++text;     // Skip '['
						int depth = 1;
						while (depth > 0)
						{
							switch (*text)
							{
							case Char('['): ++depth; break;
							case Char(']'): --depth; break;
							case 0: XLA_XML_THROWEXP("unexpected end of data", text);
							}
							++text;
						}
						break;
					}

					// Error on end of text
				case Char('\0'):
					XLA_XML_THROWEXP("unexpected end of data", text);

					// Other character, skip it
				default:
					++text;

				}
			}

			
			if (Flags & parse_doctype_node)
			{
				// Create a new doctype node
				XmlNode *doctype = this->AllocateNode(XmlNodeDoctype);
				doctype->NodeValue(value, text - value);

				// Place zero terminator after value
				if (!(Flags & parse_no_string_terminators))
					*text = Char('\0');

				text += 1;      // skip '>'
				return doctype;
			}
			else
			{
				text += 1;      // skip '>'
				return 0;
			}

		}

		
		template<int Flags>
		XmlNode *LocalParsePI(Char *&text)
		{
			// If creation of PI nodes is enabled
			if (Flags & parse_pi_nodes)
			{
				// Create pi node
				XmlNode *pi = this->AllocateNode(XmlNodePI);

				// Extract PI target name
				Char *name = text;
				Skip<node_name_pred, Flags>(text);
				if (text == name)
					XLA_XML_THROWEXP("expected PI target", text);
				pi->NodeName(name, text - name);

				// Skip whitespace between pi target and pi
				Skip<whitespace_pred, Flags>(text);

				// Remember start of pi
				Char *value = text;

				// Skip to '?>'
				while (text[0] != Char('?') || text[1] != Char('>'))
				{
					if (*text == Char('\0'))
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}

				// Set pi value (verbatim, no entity expansion or whitespace normalization)
				pi->NodeValue(value, text - value);     

				// Place zero terminator after name and value
				if (!(Flags & parse_no_string_terminators))
				{
					pi->NodeName()[pi->NodeNameSize()] = Char('\0');
					pi->NodeValue()[pi->NodeValueSize()] = Char('\0');
				}

				text += 2;                          // Skip '?>'
				return pi;
			}
			else
			{
				// Skip to '?>'
				while (text[0] != Char('?') || text[1] != Char('>'))
				{
					if (*text == Char('\0'))
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}
				text += 2;    // Skip '?>'
				return 0;
			}
		}

		
		template<int Flags>
		Char LocalParseAppendData(XmlNode *node, Char *&text, Char *contents_start)
		{
			// Backup to contents start if whitespace trimming is disabled
			if (!(Flags & parse_trim_whitespace))
				text = contents_start;     

			// Skip until end of data
			Char *value = text, *end;
			if (Flags & parse_normalize_whitespace)
				end = LocalExpandCharacter<text_pred, text_pure_with_ws_pred, Flags>(text);   
			else
				end = LocalExpandCharacter<text_pred, text_pure_no_ws_pred, Flags>(text);

			// Trim trailing whitespace if flag is set; leading was already trimmed by whitespace skip after >
			if (Flags & parse_trim_whitespace)
			{
				if (Flags & parse_normalize_whitespace)
				{
					// Whitespace is already condensed to single space characters by skipping function, so just trim 1 char off the end
					if (*(end - 1) == Char(' '))
						--end;
				}
				else
				{
					// Backup until non-whitespace character is found
					while (whitespace_pred::test(*(end - 1)))
						--end;
				}
			}

			// If characters are still left between end and value (this test is only necessary if normalization is enabled)
			// Create new data node
			if (!(Flags & parse_no_data_nodes))
			{
				XmlNode *data = this->AllocateNode(XmlNodeData);
				data->NodeValue(value, end - value);
				node->Append(data);
			}

			// Add data to parent node if no data exists yet
			if (!(Flags & parse_no_element_values)) 
				if (*node->NodeValue() == Char('\0'))
					node->NodeValue(value, end - value);

			// Place zero terminator after value
			if (!(Flags & parse_no_string_terminators))
			{
				Char ch = *text;
				*end = Char('\0');
				return ch;      // Return character that ends data; this is required because zero terminator overwritten it
			}

			// Return character that ends data
			return *text;
		}

		// Parse CDATA
		template<int Flags>
		XmlNode *LocalParseCData(Char *&text)
		{
			// If CDATA is disabled
			if (Flags & parse_no_data_nodes)
			{
				// Skip until end of cdata
				while (text[0] != Char(']') || text[1] != Char(']') || text[2] != Char('>'))
				{
					if (!text[0])
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}
				text += 3;      // Skip ]]>
				return 0;       // Do not produce CDATA node
			}

			// Skip until end of cdata
			Char *value = text;
			while (text[0] != Char(']') || text[1] != Char(']') || text[2] != Char('>'))
			{
				if (!text[0])
					XLA_XML_THROWEXP("unexpected end of data", text);
				++text;
			}

			// Create new cdata node
			XmlNode *cdata = this->AllocateNode(XmlNodeCData);
			cdata->NodeValue(value, text - value);

			// Place zero terminator after value
			if (!(Flags & parse_no_string_terminators))
				*text = Char('\0');

			text += 3;      // Skip ]]>
			return cdata;
		}

		// Parse element node
		template<int Flags>
		XmlNode *LocalParseElement(Char *&text)
		{
			// Create element node
			XmlNode *element = this->AllocateNode(XmlNodeElement);

			// Extract element name
			Char *name = text;
			Skip<node_name_pred, Flags>(text);
			if (text == name)
				XLA_XML_THROWEXP("expected element name", text);
			element->NodeName(name, text - name);

			// Skip whitespace between element name and attributes or >
			Skip<whitespace_pred, Flags>(text);

			// Parse attributes, if any
			LocalParseAttribute<Flags>(text, element);

			// Determine ending type
			if (*text == Char('>'))
			{
				++text;
				LocalParseContents<Flags>(text, element);
			}
			else if (*text == Char('/'))
			{
				++text;
				if (*text != Char('>'))
					XLA_XML_THROWEXP("expected >", text);
				++text;
			}
			else
				XLA_XML_THROWEXP("expected >", text);

			// Place zero terminator after name
			if (!(Flags & parse_no_string_terminators))
				element->NodeName()[element->NodeNameSize()] = Char('\0');

			// Return parsed element
			return element;
		}

		// Determine node type, and parse it
		template<int Flags>
		XmlNode *LocalParseNode(Char *&text)
		{
			// Parse proper node type
			switch (text[0])
			{

				// <...
			default: 
				// Parse and append element node
				return LocalParseElement<Flags>(text);

				// <?...
			case Char('?'): 
				++text;     // Skip ?
				if ((text[0] == Char('x') || text[0] == Char('X')) &&
					(text[1] == Char('m') || text[1] == Char('M')) && 
					(text[2] == Char('l') || text[2] == Char('L')) &&
					whitespace_pred::test(text[3]))
				{
					// '<?xml ' - xml declaration
					text += 4;      // Skip 'xml '
					return LocalParseDeclaration<Flags>(text);
				}
				else
				{
					// Parse PI
					return LocalParsePI<Flags>(text);
				}

				// <!...
			case Char('!'): 

				// Parse proper subset of <! node
				switch (text[1])    
				{

					// <!-
				case Char('-'):
					if (text[2] == Char('-'))
					{
						// '<!--' - xml comment
						text += 3;     // Skip '!--'
						return LocalParseComment<Flags>(text);
					}
					break;

					// <![
				case Char('['):
					if (text[2] == Char('C') && text[3] == Char('D') && text[4] == Char('A') && 
						text[5] == Char('T') && text[6] == Char('A') && text[7] == Char('['))
					{
						// '<![CDATA[' - cdata
						text += 8;     // Skip '![CDATA['
						return LocalParseCData<Flags>(text);
					}
					break;

					// <!D
				case Char('D'):
					if (text[2] == Char('O') && text[3] == Char('C') && text[4] == Char('T') && 
						text[5] == Char('Y') && text[6] == Char('P') && text[7] == Char('E') && 
						whitespace_pred::test(text[8]))
					{
						// '<!DOCTYPE ' - doctype
						text += 9;      // skip '!DOCTYPE '
						return LocalParseDoctype<Flags>(text);
					}

				}   // switch

				// Attempt to skip other, unrecognized node types starting with <!
				++text;     // Skip !
				while (*text != Char('>'))
				{
					if (*text == 0)
						XLA_XML_THROWEXP("unexpected end of data", text);
					++text;
				}
				++text;     // Skip '>'
				return 0;   // No node recognized

			}
		}

		// Parse contents of the node - children, data etc.
		template<int Flags>
		void LocalParseContents(Char *&text, XmlNode *node)
		{
			// For all children and text
			while (1)
			{
				// Skip whitespace between > and node contents
				Char *contents_start = text;      // Store start of node contents before whitespace is skipped
				Skip<whitespace_pred, Flags>(text);
				Char next_char = *text;

				// After data nodes, instead of continuing the loop, control jumps here.
				// This is because zero termination inside parse_and_append_data() function
				// would wreak havoc with the above code.
				// Also, skipping whitespace after data nodes is unnecessary.
after_data_node:    

				// Determine what comes next: node closing, child node, data node, or 0?
				switch (next_char)
				{

					// Node closing or child node
				case Char('<'):
					if (text[1] == Char('/'))
					{
						// Node closing
						text += 2;      // Skip '</'
						if (Flags & parse_validate_closing_tags)
						{
							// Skip and validate closing tag name
							Char *closing_name = text;
							Skip<node_name_pred, Flags>(text);
							if (!XmlUnit::StringCompare(node->NodeName(), node->NodeNameSize(), closing_name, text - closing_name, true))
								XLA_XML_THROWEXP("invalid closing tag name", text);
						}
						else
						{
							// No validation, just skip name
							Skip<node_name_pred, Flags>(text);
						}
						// Skip remaining whitespace after node name
						Skip<whitespace_pred, Flags>(text);
						if (*text != Char('>'))
							XLA_XML_THROWEXP("expected >", text);
						++text;     // Skip '>'
						return;     // Node closed, finished parsing contents
					}
					else
					{
						// Child node
						++text;     // Skip '<'
						if (XmlNode *child = LocalParseNode<Flags>(text))
							node->Append(child);
					}
					break;

					// End of data - error
				case Char('\0'):
					XLA_XML_THROWEXP("unexpected end of data", text);

					// Data node
				default:
					next_char = LocalParseAppendData<Flags>(node, text, contents_start);
					goto after_data_node;   // Bypass regular processing after data nodes

				}
			}
		}

		// Parse XML attributes of the node
		template<int Flags>
		void LocalParseAttribute(Char *&text, XmlNode *node)
		{
			// For all attributes 
			while (attribute_name_pred::test(*text))
			{
				// Extract attribute name
				Char *name = text;
				++text;     // Skip first character of attribute name
				Skip<attribute_name_pred, Flags>(text);
				if (text == name)
					XLA_XML_THROWEXP("expected attribute name", name);

				// Create new attribute
				XmlAttribute *attribute = this->AllocAttribute();
				attribute->NodeName(name, text - name);
				node->AppendAttribute(attribute);

				// Skip whitespace after attribute name
				Skip<whitespace_pred, Flags>(text);

				// Skip =
				if (*text != Char('='))
					XLA_XML_THROWEXP("expected =", text);
				++text;

				// Add terminating zero after name
				if (!(Flags & parse_no_string_terminators))
					attribute->NodeName()[attribute->NodeNameSize()] = 0;

				// Skip whitespace after =
				Skip<whitespace_pred, Flags>(text);

				// Skip quote and remember if it was ' or "
				Char quote = *text;
				if (quote != Char('\'') && quote != Char('"'))
					XLA_XML_THROWEXP("expected ' or \"", text);
				++text;

				// Extract attribute value and expand char refs in it
				Char *value = text, *end;
				const int AttFlags = Flags & ~parse_normalize_whitespace;   // No whitespace normalization in attributes
				if (quote == Char('\''))
					end = LocalExpandCharacter<attribute_value_pred<Char('\'')>, attribute_value_pure_pred<Char('\'')>, AttFlags>(text);
				else
					end = LocalExpandCharacter<attribute_value_pred<Char('"')>, attribute_value_pure_pred<Char('"')>, AttFlags>(text);

				// Set attribute value
				attribute->NodeValue(value, end - value);

				// Make sure that end quote is present
				if (*text != quote)
					XLA_XML_THROWEXP("expected ' or \"", text);
				++text;     // Skip quote

				// Add terminating zero after value
				if (!(Flags & parse_no_string_terminators))
					attribute->NodeValue()[attribute->NodeValueSize()] = 0;

				// Skip whitespace after attribute value
				Skip<whitespace_pred, Flags>(text);
			}
		}

	};

	//! \cond internal
	namespace XmlUnit
	{

		// Whitespace (space \n \r \t)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_whitespace[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  // 0
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 1
			1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 2
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 3
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 4
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 5
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 6
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 7
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 8
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 9
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // A
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // B
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // C
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // D
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // E
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   // F
		};

		// Node name (anything but space \n \r \t / > ? \0)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_node_name[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Text (i.e. PCDATA) (anything but < \0)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_text[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Text (i.e. PCDATA) that does not require processing when ws normalization is disabled 
		// (anything but < \0 &)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_text_pure_no_ws[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Text (i.e. PCDATA) that does not require processing when ws normalizationis is enabled
		// (anything but < \0 & space \n \r \t)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_text_pure_with_ws[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			0,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Attribute name (anything but space \n \r \t / < > = ? ! \0)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_attribute_name[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Attribute data with single quote (anything but ' \0)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_attribute_data_1[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Attribute data with single quote that does not require processing (anything but ' \0 &)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_attribute_data_1_pure[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Attribute data with double quote (anything but " \0)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_attribute_data_2[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Attribute data with double quote that does not require processing (anything but " \0 &)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_attribute_data_2_pure[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
			1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
		};

		// Digits (dec and hex, 255 denotes end of numeric character reference)
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_digits[256] = 
		{
			// 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 0
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 1
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 2
			0,  1,  2,  3,  4,  5,  6,  7,  8,  9,255,255,255,255,255,255,  // 3
			255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,  // 4
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 5
			255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,  // 6
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 7
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 8
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // 9
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // A
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // B
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // C
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // D
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  // E
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255   // F
		};

		// Upper case conversion
		template<int Dummy>
		const Byte XmlByteTable<Dummy>::lookup_upcase[256] = 
		{
			// 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  A   B   C   D   E   F
			0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,   // 0
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,   // 1
			32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,   // 2
			48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,   // 3
			64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,   // 4
			80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,   // 5
			96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,   // 6
			80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123,124,125,126,127,  // 7
			128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,  // 8
			144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,  // 9
			160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,  // A
			176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,  // B
			192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,  // C
			208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,  // D
			224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,  // E
			240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255   // F
		};
	}
	//! \endcond

}

#include <XLA/XML/LibXml.include.hpp>

namespace XLA
{
	void LoadXmlDocument( XmlDocument& xmlDocument , CString& fileName )
	{
		StreamReader streamReader( fileName , Encoding::UTF8() , true );
		
		String xmlString = streamReader.ReadToEnd();

		Char* xmlText = xmlDocument.AllocateString( xmlString.c_str() , xmlString.size() );

		xmlDocument.LocalParse<0>( xmlText );
	}
}

// Undefine internal macros
#undef XLA_XML_THROWEXP

// On MSVC, restore warnings state
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
