#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <stack>

typedef struct	s_node {
	int				*n;
	int				g;
	int				h;
	int				f;
	struct s_node	*parent;
}				t_node;
