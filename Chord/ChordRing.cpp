#include "stdafx.h"
#include "ChordRing.h"


ChordRing::ChordRing()
{
	InitRing();
	Join("123123122");
	Join("221");
	Join("21");
	Join("212221");
	
	Insert("2", L"43003");
	Insert("3", L"43113");
	Insert("4", L"4111133");
	Insert("5", L"454433");
	Insert("6", L"ÄãºÃ");
	Insert("7", L"437773");

	Join("1");
	Join("11");
	Join("111");
	Join("11111");
	Join("111111");
	Join("1000");
	Join("10000");

}


ChordRing::~ChordRing()
{
	delete father_node1;
	delete father_node2;
}

ChordNode * ChordRing::FindSuccessor(std::string key) //for join node
{
	class ChordNode *dest_node = this->father_node1;
	//	int i = HASH_BIT - 1;
	while (!(key.compare(dest_node->GetHashKey()) == 1 &&
		key.compare(dest_node->GetSuccessor()->GetHashKey()) == -1))
	{
		std::map<std::string, class ChordNode*>finger_table;
		std::map<std::string, class ChordNode*>::iterator table_node;
		//use map sort function to find successor

		for (int i = 0; i < HASH_BIT; i++)
		{
			finger_table[(dest_node->GetFingerTable()[i])->GetHashKey()] = dest_node->GetFingerTable()[i];
		}
		if (finger_table.count(key) == 1) //the key already existed
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
	//for insert key and route
	class ChordNode *dest_node = this->father_node1;
	node_route.push_back(dest_node->GetPredecessor());
	//	int i = HASH_BIT - 1;
	while (!(key.compare(dest_node->GetHashKey()) == 1 &&
		key.compare(dest_node->GetSuccessor()->GetHashKey()) == -1))
	{
		std::map<std::string, class ChordNode*>finger_table;
		std::map<std::string, class ChordNode*>::iterator table_node;
		for (int i = 0; i < HASH_BIT; i++)
		{
			finger_table[(dest_node->GetFingerTable()[i])->GetHashKey()] = dest_node->GetFingerTable()[i];
		}
		if (finger_table.count(key) == 1)
		{    //hash_key == hash_node and directly return
			table_node = finger_table.find(key);
			dest_node = table_node->second;
			node_route.push_back(dest_node);
			node_route.push_back(dest_node->GetSuccessor());
			return dest_node->GetSuccessor();
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
{		// Course of it is a stand-alone version chord ,
		//so I use this way to refresh rather than sending msg.
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
	class ChordNode *successor_node = FindSuccessor(join_node->GetHashKey());
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
	std::map<std::string, class KeyNode*> key_list = successor_node->GetHashKeyNodeList();
	std::map<std::string, class KeyNode*>::iterator key_node;
	if (key_list.size() != 0)
	{
		key_node = key_list.begin();
		while(key_node != key_list.end())
		{
			if (join_node->GetHashKey().compare(key_node->first) != 1)
				break;
			join_node->AddKeyNode(key_node->first, key_node->second);
			successor_node->DeleteKeyNode(key_node->first);
			key_node++;
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
	if (dest_node == NULL)
	{ //almost impossible
		key[HASH_BIT / 4 - 1] -= 1;
		dest_node = FindSuccessor(key); 
		dest_node = dest_node->GetSuccessor();
	}  
	key = sha1.Encode(ip);
	if (dest_node->GetHashKey() == key)
	{
		std::map<std::string, class KeyNode*> node_list = dest_node->GetHashKeyNodeList();
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
	std::vector<ChordNode *> node_route;
	KeyNode *temp_key_node = new KeyNode(key, Value);
	class ChordNode *dest_node = FindSuccessor(temp_key_node->GetHashKey(), node_route);
	if (!dest_node->AddKeyNode(temp_key_node->GetHashKey(), temp_key_node))
	{
		delete temp_key_node;
		return false;
	}
	return true;
}

std::wstring ChordRing::Lookup(std::string _key,std::vector<ChordNode *>&node_route)
{
	std::string key = sha1.Encode(_key);
	node_route.erase(node_route.begin(), node_route.end());
	class ChordNode *dest_node = FindSuccessor(key, node_route);
	return dest_node->LookUpKeyNode(key);

}

bool ChordRing::Update(std::string key, std::wstring Value)
{
	std::vector<ChordNode *> node_route;
	std::string sha1_key = sha1.Encode(key);
	class ChordNode *dest_node = FindSuccessor(sha1_key, node_route);
	if (dest_node->UpdateKeyNode(sha1_key, Value))
		return true;
	else
		return false;
}
