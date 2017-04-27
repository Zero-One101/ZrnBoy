#include "Z80.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>

Z80::Z80()
{
}


bool Z80::LoadGame(std::string const &gameName)
{
	std::ifstream input(gameName, std::ios::binary);
	input >> std::noskipws;
	std::istream_iterator<char> start(input), end;
	std::vector<char> buffer(start, end);

	for (auto& c : buffer)
	{
		printf(&c);
	}

	return true;
}

Z80::~Z80()
{
}
