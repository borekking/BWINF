#include <vector>
#include <set>
#include "matrix_bool.h"

#ifndef _MAX_CLIQUES_
#define _MAX_CLIQUES_

// 1. simple all max cliques algorithm 
std::set<std::set<int>> max_cliques(int n, matrix_bool &mat);


#endif