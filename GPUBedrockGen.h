
//have a string for each bedrock gen code and select which to append
#include <string>

//formation is array of ints, repeating every 4 as (x, y, z, type=[0,1])

std::string kernel =
"kernel void hasFormation(global const long* a, global const long* b, global const int* formation, global const int* formation_size, global int* results_ind, global const int* results_max, global int* results, global const int* coord_offsets)"//max size results
"{"
"	if(*results_ind >= *results_max)"
"		return;"
"	int i = 0;"
"	for(i=0; i < *formation_size; i++)"
"	{"
"		if(getBedrock(get_global_id(0) + coord_offsets[0] + formation[i*4], get_global_id(1) + coord_offsets[1] + formation[i*4 + 1], get_global_id(2) + coord_offsets[2] + formation[i*4 + 2], a, b) != formation[i*4 + 3])"//redefine getBedrock for each context
"		{"
"			return;"
"		}"
"	}"
"	int ind = atomic_add(results_ind, 1);"
"	if(ind < *results_max)"
"	{"
"		results[ind*3] = get_global_id(0) + coord_offsets[0];"
"		results[ind*3 + 1] = get_global_id(1) + coord_offsets[1];"
"		results[ind*3 + 2] = get_global_id(2) + coord_offsets[2];"
"	}"
"}"
" ";

std::string utility =
"inline long rawSeedFromChunk(int x, int z)"
"{"
"	return (((long)x * (long)341873128712 + (long)z * (long)132897987541) ^ (long)0x5DEECE66D) & ((((long)1 << 48) - 1));"
"}"
" "
"inline int rand5(long raw_seed, long a, long b)"
"{"
"	return (int)((((raw_seed * a + b) & (((long)1 << 48) - 1)) >> 17) % ((long)5));"
"}"
" "
"inline int precompChunkIndCalcNormal(int x, int y, int z, int nether)"
"{"
"	return ((z * 16 + x) * (nether == 1 ? 8 : 4) + ((nether == 1 ? 7 : 3) - y));"
"}"
" "
"inline void precompChunkIndCalc113(int* inds, int* count, int x, int y, int z, int nether)"
"{"
"	inds[*count] = ((z * 17 + x) * (nether == 1 ? 8 : 4) + ((nether == 1 ? 7 : 3) - y));"
"	(*count)++;"
"	if (x == 0)"
"	{"
"		inds[*count] = ((z * 17 + 16) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));"
"		(*count)++;"
"	}"
"	if (z == 0)"
"	{"
"		inds[*count] = ((16 * 17 + x) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));"
"		(*count)++;"
"	}"
"	if (x == 0 && z == 0)"
"	{"
"		inds[*count] = ((16 * 17 + 16) * (nether ? 8 : 4) + ((nether ? 7 : 3) - y));"
"		(*count)++;"
"	}"
"}"
" ";

std::string OW112_114 =
"inline int getBedrock(int x, int y, int z, global const long* a, global const long* b)"
"{"
"	if (y == 0) return 1;"
"	if (y < 0 || y > 4) return 0;"
"	int precomp_ind = precompChunkIndCalcNormal(x & 15, y - 1, z & 15, 0);"
"	return (rand5(rawSeedFromChunk(x >> 4, z >> 4), a[precomp_ind], b[precomp_ind]) >= y)? 1 : 0;"
"}"
" ";

std::string OW113 =
"inline int getBedrock(int x, int y, int z, global const long* a, global const long* b)"
"{"
"	if (y == 0) return 1;"
"	if (y < 0 || y > 4) return 0;"
"	int inds[4];"
"	int count = 0;"
"	precompChunkIndCalc113(inds, &count, x & 15, y - 1, z & 15, 0);"
"	int found = 0;"
"	for (int i = 0; i < count; i++)"
"		found |= (rand5(rawSeedFromChunk(x >> 4, z >> 4), a[inds[i]], b[inds[i]]) >= y)?1:0;"
"	return found;"
"}"
" ";

std::string N113 =
"inline int getBedrock(int x, int y, int z, global const long* a, global const long* b)"
"{"
"	if (y == 0 || y == 127) return 1;"
"	if (y < 0 || (y > 4 && y < 123) || y > 127) return 0;"
"	int inds[4];"
"	int count = 0;"
"	precompChunkIndCalc113(inds, &count, x & 15, y > 4 ? y - 119 : y - 1, z & 15, 1);"
"	int found = 0;"
"	for (int i = 0; i < count; i++)"
"		found |= (y > 4 ? ((4 - rand5(rawSeedFromChunk(x >> 4, z >> 4), a[inds[i]], b[inds[i]]) <= y - 123)?1:0) :"
"		((rand5(rawSeedFromChunk(x >> 4, z >> 4), a[inds[i]], b[inds[i]]) >= y)?1:0));"
"	return found;"
"}"
" ";

std::string N114 =
"inline int getBedrock(int x, int y, int z, global const long* a, global const long* b)"
"{"
"	if (y == 0 || y == 127) return 1;"
"	if (y < 0 || (y > 4 && y < 123) || y > 127) return 0;"
"	int precomp_ind = precompChunkIndCalcNormal(x & 15, y > 4 ? y - 119 : y - 1, z & 15, 1);"
"	return y > 4 ? (4 - rand5(rawSeedFromChunk(x >> 4, z >> 4), a[precomp_ind], b[precomp_ind]) <= y - 123)?1:0 :"
"		(rand5(rawSeedFromChunk(x >> 4, z >> 4), a[precomp_ind], b[precomp_ind]) >= y)?1:0;"
"}"
" ";
