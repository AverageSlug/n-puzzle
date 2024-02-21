#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <stack>
#include <random>

typedef struct	s_node {
	int				*n;
	int				g;
	int				h;
	int				f;
	int				dir;
	struct s_node	*parent;
}				t_node;

typedef struct	s_heuristic
{
	std::string		name;
	int				(*c)(t_node a, int *solution, int x);
}				t_heuristic;
