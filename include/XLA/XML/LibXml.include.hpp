#ifndef XLA_LIBXML_SOURCE_INCLUDE
#define XLA_LIBXML_SOURCE_INCLUDE

namespace XLA
{
	XmlNode * XmlMemoryPool::CloneNode(const XmlNode *source, XmlNode *result )
	{
		// Prepare result node
		if (result)
		{
			result->RemoveAllAttributes();
			result->RemoveAll();
			result->SetType(source->GetType());
		}
		else
			result = AllocateNode(source->GetType());

		// Clone name and value
		result->NodeName(source->NodeName(), source->NodeNameSize());
		result->NodeValue(source->NodeValue(), source->NodeValueSize());

		// Clone child nodes and attributes
		for (XmlNode *child = source->GetFirst(); child; child = child->Next())
			result->Append(CloneNode(child));
		for (XmlAttribute *attr = source->GetFirstAttribute(); attr; attr = attr->GetNext())
			result->AppendAttribute(AllocAttribute(attr->NodeName(), attr->NodeValue(), attr->NodeNameSize(), attr->NodeValueSize()));

		return result;
	}

	Char* XmlMemoryPool::AllocateString(const Char *source, Size size)
	{
		assert(source || size);     // Either source or size (or both) must be specified
		if (size == 0)
			size = XmlUnit::GetSize(source) + 1;
		Char *result = static_cast<Char *>(AllocateAligned( (size + 1) * sizeof(Char)));
		if (source)
		{
			for (Size i = 0; i < size; ++i)
				result[i] = source[i];
			result[size] = 0;
		}
		return result;
	}

	XmlAttribute* XmlMemoryPool::AllocAttribute(const Char *name, const Char *value , 
		Size name_size , Size value_size )
	{
		void *memory = AllocateAligned(sizeof(XmlAttribute));
		XmlAttribute *attribute = new(memory) XmlAttribute;
		if (name)
		{
			if (name_size > 0)
				attribute->NodeName(name, name_size);
			else
				attribute->NodeName(name);
		}
		if (value)
		{
			if (value_size > 0)
				attribute->NodeValue(value, value_size);
			else
				attribute->NodeValue(value);
		}
		return attribute;
	}


	XmlNode* XmlMemoryPool::AllocateNode(ENodeType type, 
		const Char *name , const Char *value, 
		Size name_size , Size value_size )
	{
		void *memory = AllocateAligned(sizeof(XmlNode));
		XmlNode *node = new(memory) XmlNode(type);
		if (name)
		{
			if (name_size > 0)
				node->NodeName(name, name_size);
			else
				node->NodeName(name);
		}
		if (value)
		{
			if (value_size > 0)
				node->NodeValue(value, value_size);
			else
				node->NodeValue(value);
		}
		return node;
	}

	XmlDocument* XmlAttribute::GetDocument() const
	{
		if (XmlNode *node = this->GetParent())
		{
			while (node->GetParent())
				node = node->GetParent();
			return node->GetType() == XmlNodeDocument ? static_cast<XmlDocument*>(node) : 0;
		}
		else
			return 0;
	}

	CString XmlNodeEnumator::GetName() const
	{
		return _current->GetName();
	}

	CString XmlNodeEnumator::GetValue() const
	{
		return _current->GetValue();
	}

	ENodeType XmlNodeEnumator::GetType() const
	{
		return _current->GetType();
	}

	XmlNodeEnumator XmlNode::GetEnumator( CBool& rescursive ) const
	{
		return XmlNodeEnumator( _first , rescursive );
	}

	Bool XmlNodeEnumator::DataElement() const
	{
		return _current->DataElement();
	}
}

#endif