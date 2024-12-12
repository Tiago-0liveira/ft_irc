NAME = ircserv
CC = @c++
CFLAGS = -Wall -Wextra -Werror -g -std=c++98 $(INCLUDES)

includefolder = include/
INCLUDES = -I $(includefolder)

SRC_FOLDER = src/
OBJ_DIR = obj/

SRC_ROOT_FILES =  errors.cpp  main.cpp  misc.cpp parsing.cpp\
				 $(addprefix cmd/, \
					invite.cpp  kick.cpp  nick.cpp    pass.cpp  quit.cpp   user.cpp \
					join.cpp    mode.cpp  notice.cpp  part.cpp  ping.cpp  privmsg.cpp  topic.cpp  who.cpp \
					ignore.cpp \
				 )
				 
CLASSES = Server.cpp Client.cpp Channel.cpp

INCLUDE_HEADERS = $(addprefix $(includefolder), \
	$(CLASSES:%.cpp=%.hpp) \
)

FILES = \
	$(SRC_ROOT_FILES) \
	$(addprefix classes/, $(CLASSES))

OBJS = $(addprefix $(OBJ_DIR), $(FILES:%.cpp=%.o))


# Reset
Color_Off='\033[0m'       # Text Reset

IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
ICyan='\033[0;96m'        # Cyan

MSG0 = @echo [$(ICyan)$(NAME)$(Color_Off)] Compiled $(IGreen)$<$(Color_Off)
MSG1 = @echo "\n"[$(ICyan)$(NAME)$(Color_Off)] Compiled ${IGreen} Successfully ✔︎${Color_Off}
MSG2 = @echo ${IYellow}"Cleaned Successfully ✔︎"${Color_Off}
MSG3 = @echo ${ICyan}"Cleaned ${NAME} Successfully ✔︎"${Color_Off}

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	$(MSG1)

$(OBJS): $(OBJ_DIR)%.o: $(SRC_FOLDER)%.cpp $(INCLUDE_HEADERS)
	@mkdir -p $(OBJ_DIR) $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<
	$(MSG0)

clean:
	@/bin/rm -rf $(OBJ_DIR)
	$(MSG2)

fclean: clean
	@/bin/rm -rf $(NAME) ${OBJ_DIR}
	$(MSG3)

run: all
	@./$(NAME)

re: fclean all
