#pragma once
#include <iostream>
#include <set>
#include <tuple>
#include "PrecomputedRandAdvance.h"
#include "BedrockGen.h"
#include "BedrockSearch.h"

int main()
{
	std::set<std::tuple<int, int, int, FormType>> bedrocks;

	bedrocks.insert(std::make_tuple(0, 0, 0, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(0, 0, 1, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(0, 0, 2, FormType::BEDROCK));

	bedrocks.insert(std::make_tuple(1, 0, 0, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(1, 0, 1, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(1, 0, 2, FormType::BEDROCK));

	bedrocks.insert(std::make_tuple(2, 0, 0, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(2, 0, 1, FormType::BEDROCK));
	bedrocks.insert(std::make_tuple(2, 0, 2, FormType::BEDROCK));
	//check accuracy of threaded search

	threadedSearch(std::make_tuple(-10000, 10000, -10000, 10000, 4, 4), bedrocks, bedrockOverworld114, 1);


	return 0;
}

