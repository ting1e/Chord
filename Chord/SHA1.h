#pragma once
/*
sha1.hpp - header of
============
SHA-1 in C++
============
100% Public Domain.
Original C Code
-- Steve Reid <steve@edmweb.com>
Small changes to fit into bglibs
-- Bruce Guenter <bruce@untroubled.org>
Translation to simpler C++ Code
-- Volker Grabsch <vog@notjusthosting.com>
Safety fixes
-- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#ifndef SHA1_HPP
#define SHA1_HPP
#define HASH_BIT 160

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

class SHA1
{
public:
	SHA1();
	std::string Encode(const std::string &s);
	std::string Encode(std::istream &is);

private:
	void update(const std::string &s);
	void update(std::istream &is);
	std::string final();
	static std::string from_file(const std::string &filename);

	uint32_t digest[5];
	std::string buffer;
	uint64_t transforms;
};


#endif /* SHA1_HPP */