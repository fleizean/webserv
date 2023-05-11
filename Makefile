NAME		:= webserv

SRCS_LIST	= \
			main.cpp \
			Config/Config.cpp \
			Config/ConfigMembers.cpp \
			Config/Location.cpp \
			Config/Server.cpp \
			ErrorHandle/Error.cpp \
			Server/Cluster.cpp \
			Utils/UtilsParse.cpp

SRCS_FOLDER	= sources

SRCS		= $(addprefix ${SRCS_FOLDER}/, ${SRCS_LIST})

OBJS_FOLDER	= objects

OBJS		= $(SRCS:$(SRCS_FOLDER)/%.cpp=$(OBJS_FOLDER)/%.o)

INCLUDES	= -I includes

CC			= clang++
CFLAGS		= -Wall -Wextra -Werror -std=c++98
RM			= rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(OBJS_FOLDER)/%.o: $(SRCS_FOLDER)/%.cpp
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
		${RM} -rf $(OBJS_FOLDER)

fclean:		clean
		${RM} ${NAME}

re:			fclean all

.PHONY:		all fclean clean re
