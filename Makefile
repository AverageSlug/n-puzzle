NAME		=	$(shell basename $(CURDIR))

CXX			=	clang++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -O3

SRCS		=	$(wildcard *.cpp)

OBJS		=	$(SRCS:.cpp=.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean	:
			rm -f $(OBJS) tmp_npuzzle_*

fclean	:	clean
			rm -f $(NAME)

re		:	fclean all
