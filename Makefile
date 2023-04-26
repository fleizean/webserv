
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#              #
#    Updated: 2021/10/27 12:54:48 by hthomas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

DIRSRC		=	sources
OBJD		=	obj
INCLUDE		=	includes

INCLUDEF	=	$(INCLUDE)/color.hpp		\
				$(INCLUDE)/error.hpp		\
				$(INCLUDE)/webserv.hpp

SRC			=	main.cpp			\
				error.cpp

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
				$(CC) -I ./$(INCLUDE) $(CFLAGS) -D DEBUG_ACTIVE=$(DEBUG) -o $@ -c $<

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
