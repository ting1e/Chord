#pragma once
//没啥好说的。

#include <iostream>
#include "SHA1.h"

class KeyNode
{
public:
	KeyNode();
	KeyNode(std::string key,std::wstring value);
	std::string GetHashKey() { return sha1_key; }
	std::string GetKey() { return key; }
	std::wstring GetValue() { return value; }
	void UpdateValue(std::wstring Value) { this->value = Value; }
	~KeyNode();
private:
	std::string sha1_key;
	std::string key;
	std::wstring value;
	SHA1 sha1;
};

