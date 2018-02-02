#include "stdafx.h"
#include "ChordRing.h"


ChordRing::ChordRing()
{
	InitRing();
	Join("123123122");
	Join("221");
	Join("21");
	Join("212221");
	Insert("3", L"333");
	Insert("4", L"433");
	Insert("5", L"533");
	Insert("6", L"ÄãºÃ");
	Insert("7", L"733");
	Insert("8",	L"833");
	for (int i = 10; i < 150; i++)
	{
		Join(i + "0");
	}
}


ChordRing::~ChordRing()
{
	delete father_node1;
	delete father_node2;
}

ChordNode * ChordRing::FindSuccessor(std::string key)
{
	class ChordNode *dest_node = this->father_node1;
	//	int i = HASH_BIT - 1;
	while (!(key.compare(dest_node->GetKey()) == 1 &&
		key.compare(dest_node->GetSuccessor()->GetKey()) == -1))
	{
		std::map<std::string, class ChordNode*>finger_table;
		std::map<std::string, class ChordNode*>::iterator table_node;
		for (int i = 0; i < HASH_BIT; i++)
		{
			finger_table[(dest_node->GetFingerTable()[i])->GetKey()] = dest_node->GetFingerTable()[i];
		}
		if (finger_table.count(key) == 1)
		{
			return nullptr;
		}
		else
		{
			finger_table[key] = 0;
			table_node = finger_table.find(key);
			table_node--;
			dest_node = table_node->second;
		}
	}
	return dest_node->GetSuccessor();
}

ChordNode * ChordRing::FindSuccessor(std::string key, std::vector<ChordNode *>&node_route)
{
	class ChordNode *dest_node = this->father_node1;
	node_route.push_back(dest_node->GetPredecessor());
	//	int i = HASH_BIT - 1;
	while (!(key.compare(dest_node->GetKey()) == 1 &&
		key.compare(dest_node->GetSuccessor()->GetKey()) == -1))
	{
		std::map<std::string, class ChordNode*>finger_table;
		std::map<std::string, class ChordNode*>::iterator table_node;
		for (int i = 0; i < HASH_BIT; i++)
		{
			finger_table[(dest_node->GetFingerTable()[i])->GetKey()] = dest_node->GetFingerTable()[i];
		}
		if (finger_table.count(key) == 1)
		{
			return nullptr;
		}
		else
		{
			finger_table[key] = 0;
			table_node = finger_table.find(key);
			table_node--;
			dest_node = table_node->second;
			node_route.push_back(dest_node);
		}
	}
	node_route.push_back(dest_node->GetSuccessor());
	return dest_node->GetSuccessor();
}

void ChordRing::ReFreshAllFinTab()
{
	ChordNode *temp_node = father_node1->GetSuccessor();
	father_node1->ReFreshFinTab();
	while (temp_node != father_node1)
	{
		temp_node->ReFreshFinTab();
		temp_node = temp_node->GetSuccessor();
	}
}

void ChordRing::InitRing()
{
	father_node1->__SecretSetKey("0000000000000000000000000000000000000000");
	father_node2->__SecretSetKey("ffffffffffffffffffffffffffffffffffffffff");

	father_node1->SetPredecessor(father_node2);
	father_node1->SetSuccessor(father_node2);

	father_node2->SetPredecessor(father_node1);
	father_node2->SetSuccessor(father_node1);

	ReFreshAllFinTab();
}

bool ChordRing::Join(std::string ip)
{
	class ChordNode *join_node = new ChordNode(ip);
	class ChordNode *successor_node = FindSuccessor(join_node->GetKey());
	if (successor_node == NULL)
	{
	//	std::cout << "This key is already in the ring.\n";
		delete join_node;
		return false;
	}
	join_node->SetSuccessor(successor_node);
	join_node->SetPredecessor(successor_node->GetPredecessor());
	successor_node->GetPredecessor()->SetSuccessor(join_node);
	successor_node->SetPredecessor(join_node);
	std::map<std::string, class KeyNode*> map_node = successor_node->GetKeyNodeList();
	std::map<std::string, class KeyNode*>::iterator map_key_node;
	if (successor_node->GetKeyNodeList().size() != 0)
	{
		map_key_node = map_node.begin();
		while(map_key_node != map_node.end())
		{
			if (join_node->GetKey().compare(map_key_node->first) != -1)
				break;
			join_node->AddKeyNode(map_key_node->first, map_key_node->second);
			successor_node->DeleteKeyNode(map_key_node->first);
			map_key_node++;
		}
	}
	ReFreshAllFinTab();
	return true;
}

bool ChordRing::Leave(std::string ip)
{
	std::string key = sha1.Encode(ip);
	key[HASH_BIT/4-1] -= 1;
	class ChordNode *dest_node = FindSuccessor(key);
	key = sha1.Encode(ip);
	if (dest_node->GetKey() == key)
	{
		std::map<std::string, class KeyNode*> node_list = dest_node->GetKeyNodeList();
		std::map<std::string, class KeyNode*>::iterator map_key_node;
		map_key_node = node_list.begin();
		while(map_key_node != node_list.end())
		{
			dest_node->GetSuccessor()->AddKeyNode(map_key_node->first, map_key_node->second);
			map_key_node++;
		}
		dest_node->GetPredecessor()->SetSuccessor(dest_node->GetSuccessor());
		dest_node->GetSuccessor()->SetPredecessor(dest_node->GetPredecessor());
		delete dest_node;
		ReFreshAllFinTab();
		return true;
	}
	return false;
	
}

bool ChordRing::Insert(std::string key, std::wstring Value)
{
	KeyNode *temp_key_node = new KeyNode(key, Value);
	class ChordNode *dest_node = FindSuccessor(temp_key_node->GetKey());
	if (!dest_node->AddKeyNode(temp_key_node->GetKey(), temp_key_node))
	{
		delete temp_key_node;
		return false;
	}
	return true;
}

std::wstring ChordRing::Lookup(std::string _key,std::vector<ChordNode *>&node_route)
{
	std::string key = sha1.Encode(_key);
	class ChordNode *dest_node = FindSuccessor(key, node_route);
	if ((dest_node->LookUpKeyNode(key)).size()!=0)
	{
		return dest_node->LookUpKeyNode(key);
	}
	else
	{
		return std::wstring();
	}
}

bool ChordRing::Update(std::string key, std::wstring Value)
{
	std::string Key = sha1.Encode(key);
	class ChordNode *dest_node = FindSuccessor(Key);
	if (dest_node->UpdateKeyNode(Key, Value))
		return true;
	else
		return false;
}
