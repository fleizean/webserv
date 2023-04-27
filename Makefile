NAME		=	webserv

DIRSRC		=	sources
OBJD		=	obj
INCLUDE		=	includes

INCLUDEF	=	$(INCLUDE)/color.hpp		\
				$(INCLUDE)/Error.hpp		\
				$(INCLUDE)/WebServer.hpp

SRC			=	main.cpp			\
				Error.cpp			\
				WebServer.cpp		\
				utils_parse.cpp

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(OBJ:%=$(OBJD)/%)

CFLAGS		=	-Wall -Wextra -Werror -std=c++98 # -g3 -fsanitize=address -Ofast
CC			=	clang++
RM			=	rm -rf

DEBUG		=	0


$(NAME)		:	$(OBJD) $(OBJS) $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJD)		:
				@mkdir $(OBJD)

$(OBJD)/%.o	:	$(DIRSRC)/%.cpp $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) $(CFLAGS) -o $@ -c $<

all			:	$(NAME)

clean		:
				$(RM) $(OBJS)
				$(RM) $(OBJS) $(INCLUDE)/*.gch
				$(RM) $(OBJD)

fclean		:	clean

				$(RM) $(NAME)

%:
	@:

re			:	fclean all

.PHONY		:	all clean re fclean
