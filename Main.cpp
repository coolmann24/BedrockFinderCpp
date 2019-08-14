#pragma once
#include <iostream>
#include <set>
#include <tuple>
#include <array>
#include "PrecomputedRandAdvance.h"

inline bool bedrockOverworld112(int x, int y, int z);
bool bedrockOverworld113(int x, int y, int z);
bool bedrockNether113(int x, int y, int z);
bool bedrockOverworld114(int x, int y, int z);
bool bedrockNether114(int x, int y, int z);

int main()
{
	std::set<std::tuple<int, int, int>> bedrocks;

	bedrocks.insert(std::make_tuple(0, 0, 0));
	bedrocks.insert(std::make_tuple(0, 0, 1));
	bedrocks.insert(std::make_tuple(0, 0, 2));

	bedrocks.insert(std::make_tuple(1, 0, 0));
	bedrocks.insert(std::make_tuple(1, 0, 1));
	bedrocks.insert(std::make_tuple(1, 0, 2));

	bedrocks.insert(std::make_tuple(2, 0, 0));
	bedrocks.insert(std::make_tuple(2, 0, 1));
	bedrocks.insert(std::make_tuple(2, 0, 2));

	/*bedrocks.insert(std::make_tuple(0, 0, 0));

	bedrocks.insert(std::make_tuple(0, 0, 1));
	bedrocks.insert(std::make_tuple(0, 0, 2));
	bedrocks.insert(std::make_tuple(-1, 0, 2));
	//bedrocks.insert(std::make_tuple(2, 0, 2));

	bedrocks.insert(std::make_tuple(0, 0, -1));
	bedrocks.insert(std::make_tuple(0, 0, -2));
	bedrocks.insert(std::make_tuple(1, 0, -2));
	//bedrocks.insert(std::make_tuple(-2, 0, -2));

	bedrocks.insert(std::make_tuple(1, 0, 0));
	bedrocks.insert(std::make_tuple(2, 0, 0));
	bedrocks.insert(std::make_tuple(2, 0, 1));
	//bedrocks.insert(std::make_tuple(2, 0, -2));

	bedrocks.insert(std::make_tuple(-1, 0, 0));
	bedrocks.insert(std::make_tuple(-2, 0, 0));
	bedrocks.insert(std::make_tuple(-2, 0, -1));
	//bedrocks.insert(std::make_tuple(-2, 0, 2));*/

	int minx = -1000;
	int minz = -1000;

	int maxx = 1000;
	int maxz = 1000;

	for(int x = minx; x <= maxx; x++)
		for (int z = minz; z <= maxz; z++)
		{
			bool found = true;

			for (const auto& point : bedrocks)
			{
				found = bedrockOverworld114(std::get<0>(point) + x, 4, std::get<2>(point) + z);
				if (!found)break;
			}

			if (found)std::cout << "X: " << x << " Z: " << z << std::endl;
		}

	return 0;
}

inline int64_t rawSeedFromChunk(int x, int z) //chunk coordinates
{
	return (((int64_t)x * (int64_t)341873128712 + (int64_t)z * (int64_t)132897987541) ^ (int64_t)0x5DEECE66D) & ((((int64_t)1 << 48) - 1));
}

inline int rand5(int64_t raw_seed, int64_t a, int64_t b)
{
	return (int)((((raw_seed * a + b) & (((int64_t)1 << 48) - 1)) >> 17) % ((int64_t)5));
}

inline int precompChunkIndCalcNormal(int x, int y, int z, bool nether)// y 123->126 must be 4->7
{
	return ((z * 16 + x) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));
}

inline void precompChunkIndCalc113(int * inds, int & count, int x, int y, int z, bool nether)// y 123->126 must be 4->7
{
	inds[count] = ((z * 17 + x) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));
	count++;
	if (x == 0)
	{
		inds[count] = ((z * 17 + 16) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));
		count++;
	}
	if (z == 0)
	{
		inds[count] = ((16 * 17 + x) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));
		count++;
	}
	if (x == 0 && z == 0)
	{
		inds[count] = ((16 * 17 + 16) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));
		count++;
	}	
}

inline bool bedrockOverworld112(int x, int y, int z)
{
	if (y == 0) return true;
	if (y < 0 || y > 4) return false;

	int precomp_ind = precompChunkIndCalcNormal(x & 15, y - 1, z & 15, false);

	return rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_OW_112[precomp_ind], B_OW_112[precomp_ind]) >= y;
}

bool bedrockOverworld113(int x, int y, int z)
{
	if (y == 0) return true;
	if (y < 0 || y > 4) return false;

	int inds[4];
	int count = 0;
	precompChunkIndCalc113(inds, count, x & 15, y - 1, z & 15, false);

	bool found = false;

	for(int i = 0; i < count; i++)
		found = found || rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_OW_113_114[inds[i]], B_OW_113_114[inds[i]]) >= y;
	return found;
}

bool bedrockOverworld114(int x, int y, int z)
{
	if (y == 0) return true;
	if (y < 0 || y > 4) return false;

	int precomp_ind = precompChunkIndCalcNormal(x & 15, y - 1, z & 15, false);

	return rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_OW_113_114[precomp_ind], B_OW_113_114[precomp_ind]) >= y;
}

bool bedrockNether113(int x, int y, int z)
{
	if (y == 0 || y == 127) return true;
	if (y < 0 || (y > 4 && y < 123) || y > 127) return false;

	int inds[4];
	int count = 0;
	precompChunkIndCalc113(inds, count, x & 15, y > 4 ? y - 119 : y - 1, z & 15, true);

	bool found = false;

	for(int i = 0; i < count; i++)
		found = found || (y > 4 ? (4 - rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_N_113_114[inds[i]], B_N_113_114[inds[i]]) <= y - 123) :
		(rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_N_113_114[inds[i]], B_N_113_114[inds[i]]) >= y));

	return found;
}

inline bool bedrockNether114(int x, int y, int z)
{
	if (y == 0 || y == 127) return true;
	if (y < 0 || (y > 4 && y < 123) || y > 127) return false;

	int precomp_ind = precompChunkIndCalcNormal(x & 15, y > 4 ? y - 119 : y - 1, z & 15, true);

	return y > 4 ? 4 - rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_N_113_114[precomp_ind], B_N_113_114[precomp_ind]) <= y - 123 :
		rand5(rawSeedFromChunk(x < 0 ? -1 + ((x + 1) / 16) : x / 16, z < 0 ? -1 + ((z + 1) / 16) : z / 16), A_N_113_114[precomp_ind], B_N_113_114[precomp_ind]) >= y;
}