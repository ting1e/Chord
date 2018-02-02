#pragma once
#include <iostream>
#include "SHA1.h"
#include "ChordNode.h"
#include <map>
#include <vector>  

class ChordRing
{
public:
	ChordRing();
	~ChordRing();
	bool Join(std::string ip);
	bool Leave(std::string ip);
	bool Insert(std::string key, std::wstring Value);
	std::wstring Lookup(std::string key, std::vector<ChordNode *>&node_route);
	bool Update(std::string key, std::wstring Value);
	ChordNode * GetFatherNode() { return father_node1; }

private:
	SHA1 sha1;
	ChordNode *father_node1 = new ChordNode("Ancestor");
	ChordNode *father_node2 = new ChordNode("Ancestor");
	ChordNode *FindSuccessor(std::string key);
	ChordNode *FindSuccessor(std::string key,std::vector<ChordNode *>&);
	void ReFreshAllFinTab();
	void InitRing();
};

