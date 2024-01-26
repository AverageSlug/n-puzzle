#include "npuzzle.hpp"

int								manhattan_distance(t_node a, int *solution, int x)
{
	int		f = 0;
	for (int i = 0; i < x * x; i++)
		for (int j = 0; j < x * x; j++)
			if (a.n[i] == solution[j])
				f += abs((i / x) - (j / x)) + abs((i % x) - (j % x));
	return (f);
}

void							set_solution(int n, int *solution, int *qn, int x)
{
	if (n == -1)
		for (int i = 0; i < x; i++)
			solution[i] = qn[i];
	else if (n == 3)
	{
		solution[0]=1;
		solution[1]=2;
		solution[2]=3;
		solution[3]=8;
		solution[4]=0;
		solution[5]=4;
		solution[6]=7;
		solution[7]=6;
		solution[8]=5;
	}
}

int								check_if_equal(int *n, std::vector<t_node*> list, int x)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		int		j = 0;
		while (n[j] == list[i]->n[j] && j < x)
			j++;
		if (j == x)
			return (1);
	}
	return (0);
}

t_node							*duplicate(t_node og, int x)
{
	t_node	*tmp;
	tmp = new t_node;
	tmp->f = og.f;
	tmp->g = og.g;
	tmp->h = og.h;
	tmp->n = new int[x];
	for (int i = 0; i < x; i++)
		tmp->n[i] = og.n[i];
	tmp->parent = og.parent;
	return (tmp);
}

void							find_path(int x, int *t)
{
	std::vector<t_node*>				open;
	std::vector<t_node*>				closed;
	t_node							*q;
	int								s, i, j[4] = {1, 1, 1, 1};

	int		*solution = new int[x * x];
	set_solution(x, solution, NULL, 0);

	t_node	*first;
	first = new t_node;
	first->parent = NULL;
	first->n = new int[x * x];
	set_solution(-1, first->n, t, x * x);
	first->g = 0;
	first->h = manhattan_distance(*first, solution, x);
	first->f = first->h;
	open.push_back(first);
	while (open.size())
	{
		std::vector<t_node*>::iterator	current_smallest_f;
		std::vector<t_node*>::iterator	it;
		current_smallest_f = it = open.begin();
		it++;
		while (it != open.end())
		{
			if ((*it)->f < (*current_smallest_f)->f)
				current_smallest_f = it;
			it++;
		}
		q = *current_smallest_f;
		open.erase(current_smallest_f);
		closed.push_back(q);
		s = 4;
		i = 0;
		while (q->n[i])
			i++;
		if (!(i / 3))
		{
			s--;
			j[0] = 0;
		}
		if (!(i % 3))
		{
			s--;
			j[1] = 0;
		}
		if (i / 3 == x - 1)
		{
			s--;
			j[2] = 0;
		}
		if (i % 3 == x - 1)
		{
			s--;
			j[3] = 0;
		}
		while (s)
		{
			t_node	*successor;
			successor = new t_node;
			successor->parent = q;
			successor->g = q->g + 1;
			successor->n = new int[x * x];
			set_solution(-1, successor->n, q->n, x * x);
			int		k = 0;
			while (!j[k])
				k++;
			j[k] = 0;
			if (!k)
			{
				successor->n[i] = successor->n[i - x];
				successor->n[i - x] = 0;
			}
			else if (k == 1)
			{
				successor->n[i] = successor->n[i - 1];
				successor->n[i - 1] = 0;
			}
			else if (k == 2)
			{
				successor->n[i] = successor->n[i + x];
				successor->n[i + x] = 0;
			}
			else
			{
				successor->n[i] = successor->n[i + 1];
				successor->n[i + 1] = 0;
			}
			successor->h = manhattan_distance(*successor, solution, x);
			successor->f = successor->g + successor->h;
			if (!successor->h)
			{
				std::stack<int*>	tmp;
				t_node				*current = successor;
				while (current->parent)
				{
					tmp.push(current->n);
					current = current->parent;
				}
				std::cout << "Final solution: " << std::endl;
				int					*asdf;
				while (tmp.size())
				{
					asdf = tmp.top();
					for (int y = 0; y < x; y++)
					{
						for (int z = 0; z < x; z++)
							std::cout << asdf[x * y + z] << " ";
						std::cout << std::endl;
					}
					std::cout << std::endl;
					tmp.pop();
				}
				std::cout << std::endl << "Total moves: " << successor->g << std::endl;
				return ;
			}
			if (check_if_equal(successor->n, open, x * x))
			{
				delete [] successor->n;
				delete successor;
				s--;
				continue ;
			}
			if (check_if_equal(successor->n, closed, x * x))
			{
				delete [] successor->n;
				delete successor;
				s--;
				continue ;
			}
			open.push_back(duplicate(*successor, x * x));
			delete [] successor->n;
			delete successor;
			s--;
		}
	}
	std::cout << "No solution found" << std::endl;
	open.clear();
	closed.clear();
	delete [] solution;
}

int								main(int argc, char **argv)
{
	std::fstream	file;
	std::string		str;
	int				x = 0, i = 0;
	int				*t;

	if (argc > 1)
	{
		if (!argv[1])
		{
			std::cout << "Invalid arguments" << std::endl;
			return (1);
		}
		file.open(argv[1], std::ios::in);
		if (!file)
		{
			std::cout << "Error opening file" << std::endl;
			return (1);
		}
		getline(file, str);
		while (file)
		{
			if (str.length() && str[0] != '#')
			{
				if (!x)
				{
					int		pos = 0;
					while (str[0] == ' ')
						str.erase(0);
					pos = str.find(" ", pos);
					x = atoi(str.substr(0, pos).c_str());
					if (x < 3 || x > 3)
					{
						file.close();
						std::cout << "Invalid size. Available sizes: 3" << std::endl;
						return (1);
					}
					t = new int[x*x];
				}
				else
				{
					while (str.length())
					{
						int		pos = 0;
						while (str[0] == ' ')
							str.erase(0, 1);
						if (str[0] == '#')
						{
							str.clear();
							break ;
						}
						if (str.size() && i == x * x)
						{
							delete [] t;
							file.close();
							std::cout << "Invalid file format" << std::endl;
							return (1);
						}
						pos = str.find(' ', pos);
						t[i] = atoi(str.substr(0, pos).c_str());
						str.erase(0, pos);
						i++;
					}
				}
			}
			getline(file, str);
		}
		file.close();
	}
	else
	{
		std::cout << "idk" << std::endl;//
		return (1);
	}
	find_path(x, t);
	delete [] t;
	return (0);
}