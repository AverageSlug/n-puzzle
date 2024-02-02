#include "npuzzle.hpp"

int								one_way_distance(t_node a, int *solution, int x)
{
	int		f[x * x];
	for (int i = 0; i < x * x; i++)
		f[i] = 0;
	for (int i = 0; i < x * x; i++)
		for (int j = 0; j < x * x; j++)
			if (a.n[i] == solution[j] && a.n[i])
			{
				f[a.n[i]] += abs((i / x) - (j / x)) + abs((i % x) - (j % x));
				if (i == j)
					continue ;
				else if (i / x == j / x)
				{
					int		m = j - i;
					for (int l = 0; l < x - 1; l++)
					{
						for (int k = 0; k < x * x; k++)
							if (a.n[i + m] == solution[k] && a.n[i + m] && ((m > 0 && i / x == k / x && k <= j && k > i) || (m < 0 && i / x == k / x && k >= j && k < i)))
								f[a.n[i]]++;
						if (m > 0)
							m--;
						else
							m++;
					}
				}
				else if (i % x == j % x)
				{
					int		m = (j / 4) - (i / 4);
					for (int l = 0; l < x - 1; l++)
					{
						for (int k = 0; k < x * x; k++)
							if (a.n[i + m] == solution[k] && a.n[i + m] && ((m > 0 && i % x == k % x && k <= j && k > i) || (m < 0 && i % x == k % x && k >= j && k < i)))
								f[a.n[i]]++;
						if (m > 0)
							m -= x;
						else
							m += x;
					}
				}
			}
	int		total = 0;
	for (int i = 0; i < x * x; i++)
		total += f[i];
	return (total);
}

int								manhattan_distance(t_node a, int *solution, int x)
{
	int		f = 0;
	for (int i = 0; i < x * x; i++)
		for (int j = 0; j < x * x; j++)
			if (a.n[i] == solution[j] && a.n[i])
				f += abs((i / x) - (j / x)) + abs((i % x) - (j % x));
	return (f);
}

int								hamming_distance(t_node a, int *solution, int x)
{
	int f = 0;
	for (int i = 0; i < x * x; i++)
		if (a.n[i] != solution[i] && a.n[i])
			f++;
	return (f);
}

void							set_solution(int n, int *solution, int *qn, int x)
{
	int		t = 1, y = x, c = -1;
	if (n == -1)
		for (int i = 0; i < x * x; i++)
			solution[i] = qn[i];
	else
	{
		for (int v = 0; v < x * 2 - 1; v++)
		{
			if (t == x * x)
				t = 0;
			if (!(v % 4))
			{
				for (int i = 0; i < y; i++)
				{
					c++;
					solution[c] = t;
					t++;
				}
				y--;
			}
			else if (v % 4 == 2)
			{
				for (int i = y; i; i--)
				{
					c--;
					solution[c] = t;
					t++;
				}
				y--;
			}
			else if (v % 4 == 1)
				for (int i = 0; i < y; i++)
				{
					c += x;
					solution[c] = t;
					t++;
				}
			else
				for (int i = y; i; i--)
				{
					c -= x;
					solution[c] = t;
					t++;
				}
		}
	}
}

int								check_if_equal(int *n, std::vector<t_node*> list, int x)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		int		j = 0;
		while (j < x && n[j] == list[i]->n[j])
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

void							find_path(int x, int *t, std::string heuristic)
{
	std::vector<t_node*>	open;
	std::vector<t_node*>	closed;
	t_node					*q;
	int						s, h, i, j[4];
	size_t					c_in_time = 0, c_in_size = 0;

	t_heuristic				to_use[3];
	to_use[0].name = "manhattan";
	to_use[0].c = &manhattan_distance;
	to_use[1].name = "hamming";
	to_use[1].c = &hamming_distance;
	to_use[2].name = "one_way";
	to_use[2].c = &one_way_distance;

	for (h = 0; h < 2; h++)
		if (heuristic == to_use[h].name)
			break ;

	int		*solution = new int[x * x];
	set_solution(0, solution, NULL, x);

	int qwer;
	for (qwer = 0; qwer < x * x; qwer++)
		if (t[qwer] != solution[qwer])
			break ;
	if (qwer == x * x)
	{
		delete [] solution;
		std::cout << "Final solution: " << std::endl;
		for (int asdf = 0; asdf < x; asdf++)
		{
			for (int zxcv = 0; zxcv < x; zxcv++)
				std::cout << t[asdf * x + zxcv] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl << "Total moves: 0" << std::endl;
		std::cout << "Complexity in time: 0" << std::endl;
		std::cout << "Complexity in size: 0" << std::endl;
		return ;
	}

	t_node	*first;
	first = new t_node;
	first->parent = NULL;
	first->n = new int[x * x];
	set_solution(-1, first->n, t, x);
	first->g = 0;
	first->h = (*to_use[h].c)(*first, solution, x);
	first->f = first->h;
	open.push_back(first);
	while (open.size())
	{
		j[0] = 1;
		j[1] = 1;
		j[2] = 1;
		j[3] = 1;
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
		c_in_time++;
		closed.push_back(q);
		s = 4;
		i = 0;
		while (q->n[i])
			i++;
		if (!(i / x))
		{
			s--;
			j[0] = 0;
		}
		if (!(i % x))
		{
			s--;
			j[1] = 0;
		}
		if (i / x == x - 1)
		{
			s--;
			j[2] = 0;
		}
		if (i % x == x - 1)
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
			set_solution(-1, successor->n, q->n, x);
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
			successor->h = (*to_use[h].c)(*successor, solution, x);
			successor->f = successor->g + successor->h;
			if (c_in_size < open.size() + closed.size() + 1)
				c_in_size = open.size() + closed.size() + 1;
			if (!successor->h)
			{
				std::stack<int*>	tmp;
				t_node				*current = successor;
				while (current->parent)
				{
					tmp.push(current->n);
					current = current->parent;
				}
				tmp.push(current->n);
				std::cout << "Final solution: " << std::endl;
				int					*solutionfinale;
				while (tmp.size())
				{
					solutionfinale = tmp.top();
					for (int y = 0; y < x; y++)
					{
						for (int z = 0; z < x; z++)
							std::cout << solutionfinale[x * y + z] << " ";
						std::cout << std::endl;
					}
					std::cout << std::endl;
					delete [] solutionfinale;
					tmp.pop();
				}
				std::cout << std::endl << "Total moves: " << successor->g << std::endl;
				std::cout << "Complexity in time: " << c_in_time << std::endl;
				std::cout << "Complexity in size: " << c_in_size << std::endl;
				open.clear();
				closed.clear();
				delete [] solution;
				return ;
			}
			if (check_if_equal(successor->n, open, x * x) || check_if_equal(successor->n, closed, x * x))
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
	std::string		str, heuristic;
	int				x = 0, i = 0;
	int				*t;

	if (argc > 2)
	{
		if (!argv[1] || !argv[2])
		{
			std::cout << "Invalid arguments" << std::endl;
			return (1);
		}
		heuristic = argv[1];
		if (!(heuristic == "manhattan") && !(heuristic == "hamming") && !(heuristic == "one_way"))
		{
			std::cout << "Invalid heuristic. Available: manhattan, hamming, one_way" << std::endl;
			return (1);
		}
		file.open(argv[2], std::ios::in);
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
					if (x < 3)
					{
						file.close();
						std::cout << "Invalid input file" << std::endl;
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
	find_path(x, t, heuristic);
	delete [] t;
	return (0);
}