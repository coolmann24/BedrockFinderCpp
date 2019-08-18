//have a string for each bedrock gen code and select which to append
#include <string>

//formation is array of ints, repeating every 4 as (x, y, z, type=[0,1])

std::string kernel =
"kernel void hasFormation(global const int* precomps, global const int* formation, global const int* formation_size, global int* results_ind, global int* results)" 
"{"
"	int found = 1;"
"	int i = 0;"
"	for(i=0; i < *formation_size; i++)"
"	{"
"		if(getBedrock(get_global_id(0) + formation[i*4], get_global_id(1) + formation[i*4 + 1], get_global_id(2) + formation[i*4 + 2]) != formation[i*4 + 3])"//redefine getBedrock for each context
"		{"
"			found = 0;"
"			break;"
"		}"
"	}"
"	if(found)"
"	{"
"		int ind = atomic_add(result_ind, 1);"
"		results[ind*3] = get_global_id(0);"
"		results[ind*3 + 1] = get_global_id(1);"
"		results[ind*3 + 2] = get_global_id(2);"
"	}"
"}"

