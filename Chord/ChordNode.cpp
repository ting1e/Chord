#include "stdafx.h"
#include "ChordNode.h"





ChordNode::ChordNode(std::string ip)
{	
	this->node_ip = ip;
	this->sha1_key = Sha1.Encode(node_ip);
	this->successor = this;
	this->predecessor = this;

}

void ChordNode::ReFreshFinTab()  
{
	for (int i = 0; i < HASH_BIT; i++)  //160times
	{
		std::string val = sha1_key;  
		val[HASH_BIT / 4 - 1 - i / 4] += 1 << (i % 4);  //add 1£¬2£¬4£¬8£¬16£¬32£¬64£¬128¡£¡£¡£
		//	std::cout << val + "\n";
		for (int t = 0; t < HASH_BIT / 4; t++)
		{
			if (val[HASH_BIT / 4 - 1 - t] > '9'&&val[HASH_BIT / 4 - 1 - t] < '@')
			{
				val[HASH_BIT / 4 - 1 - t] = val[HASH_BIT / 4 - 1 - t] - ':' + 'a';
			}
			else if (val[HASH_BIT / 4 - 1 - t] > '?'&&val[HASH_BIT / 4 - 1 - t]<'B')
			{
				val[HASH_BIT / 4 - 1 - t] = val[HASH_BIT / 4 - 1 - t] - 16;
				if ((HASH_BIT / 4 - 2 - t) >= 0)
				{
					val[HASH_BIT / 4 - 2 - t] += 1;
				}
			}
			else if (val[HASH_BIT / 4 - 1 - t]>'f')
			{
				val[HASH_BIT / 4 - 1 - t] = val[HASH_BIT / 4 - 1 - t] - 'g' + '0';
				if ((HASH_BIT / 4 - 2 - t) >= 0)
				{
					val[HASH_BIT / 4 - 2 - t] += 1;
				}
			}
		}   // end add£¬¡£

		class ChordNode *dest_node = this;
		//
		while (!(val.compare(dest_node->GetHashKey()) == 1 &&
			val.compare(dest_node->successor->GetHashKey()) == -1))
		{
			dest_node = dest_node->successor;
			if (dest_node == this) break;
		}		 
		  // find the destinaion
		finger_table[i] = dest_node->successor;

	}
}

bool ChordNode::AddKeyNode(std::string key, KeyNode * node)
{
	if (key_node_list.count(key) == 0)
	{
		this->key_node_list[key] = node;
		return true;
	}
	return false;
}

bool ChordNode::DeleteKeyNode(std::string key)
{
	if (key_node_list.count(key) >=1)
	{
		this->key_node_list.erase(key);
		return true;
	}
	return false;
}

bool ChordNode::UpdateKeyNode(std::string key, std::wstring value)
{
	if (key_node_list.count(key) == 1)
	{
		key_node_list.find(key)->second->UpdateValue(value);
		return true;
	}
	return false;
}

std::wstring ChordNode::LookUpKeyNode(std::string key)
{
	if (key_node_list.count(key) == 1)
		return key_node_list.find(key)->second->GetValue();
	return std::wstring();
}

ChordNode::~ChordNode()
{

}
