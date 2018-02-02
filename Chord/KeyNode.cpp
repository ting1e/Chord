#include "stdafx.h"
#include "KeyNode.h"


KeyNode::KeyNode()
{
}


KeyNode::KeyNode(std::string key, std::wstring value)
{
	this->key = key;
	this->sha1_key = sha1.Encode(key);
	this->value = value;
}

KeyNode::~KeyNode()
{
}
