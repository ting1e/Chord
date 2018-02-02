#pragma once
#include <iostream>
#include <map>
#include "KeyNode.h"
#include "SHA1.h"

class ChordNode
{
public:
	ChordNode::ChordNode(std::string ip);
	

	void ReFreshFinTab();

	void SetSuccessor(class ChordNode *suc) { successor = suc; }
	void SetPredecessor(class ChordNode *pre) { predecessor = pre; }
	void __SecretSetKey(std::string key) { sha1_key = key; }

	bool AddKeyNode(std::string key, class KeyNode* node);
	bool DeleteKeyNode(std::string key);

	bool UpdateKeyNode(std::string key,std::wstring value);
	std::wstring LookUpKeyNode(std::string key);

	std::map<std::string, class KeyNode*> GetHashKeyNodeList(){return key_node_list; }

	std::string GetHashKey() { return sha1_key; }
	std::string GetNodeIp() { return node_ip; }
	std::string GetNodePort() { return node_port; }
	class ChordNode *GetSuccessor() { return successor; }
	class ChordNode *GetPredecessor() { return predecessor; }
	class ChordNode **GetFingerTable() { return finger_table; }
	~ChordNode();

private:
	SHA1 Sha1;
	std::string node_ip;
	std::string node_port;
	std::string sha1_key;
	class ChordNode *successor;
	class ChordNode *predecessor;
	class ChordNode *finger_table[HASH_BIT] = {0};
	std::map<std::string, class KeyNode*> key_node_list;
};

