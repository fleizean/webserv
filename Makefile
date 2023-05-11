# COMMON =======================================================================
NAME = webserv
#===============================================================================

# SOURCES ======================================================================
SOURCES_FOLDER = sources/
SOURCES =	main.cpp \
			Error.cpp \
			Config.cpp \
			UtilsParse.cpp \
			Location.cpp \
			ConfigMembers.cpp \
			Server.cpp
#===============================================================================

# INCLUDES =====================================================================
INCLUDES_FOLDER = includes/
INCLUDES = 	Error.hpp \
			Config.hpp \
			Server.hpp \
			ConfigMembers.hpp \
			Location.hpp

INCLUDES_PREFIXED = $(addprefix $(INCLUDES_FOLDER), $(INCLUDES))
#===============================================================================

# OBJECTS ======================================================================
OBJECTS_FOLDER = objects/

OBJECT = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(OBJECT))
#===============================================================================

# FLAGS ========================================================================
FLAGS = -Wall -Wextra -Werror -std=c++98# -g3 -fsanitize=address
#===============================================================================

DEBUG_VALUE = 0

$(OBJECTS_FOLDER)%.o :	$(SOURCES_FOLDER)%.cpp $(INCLUDES_PREFIXED)
	@mkdir -p $(OBJECTS_FOLDER)
	@echo "Compiling : $<"
	@clang++ $(FLAGS) -D DEBUG_ACTIVE=$(DEBUG_VALUE) -c $< -o $@

$(NAME): $(OBJECTS)
	@echo "Create    : $(NAME)"
	@clang++ $(FLAGS) $(OBJECTS) -o $(NAME)

OBJECTS_FOLDER            =   .cache_exists

all: $(NAME)
$(OBJECTS_FOLDER):
            @mkdir -p $(OBJECTS_FOLDER)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_ENTITY)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_PARSER)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_FILE)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_SERVER)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_REQUEST)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_RESPONSE)
            @mkdir -p $(OBJECTS_FOLDER)$(SRC_CGI)
clean:
	@rm -rf $(OBJECTS_FOLDER)

fclean: clean
	@rm -rf $(NAME)

re: fclean all