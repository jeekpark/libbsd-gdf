NAME				:=	../../../lib/libbsd-gdf-display.dylib
CXX					:=	c++
CXXFLAGS			:=	-Wall -Wextra -Werror -std=c++98 -I../../../include
LDFLAGS				:=	-dynamiclib -install_name '@rpath/libbsd-gdf-display.dylib'


FILE_DIR			:=	./
FILE_NAME			:=	DisplayConsole.cpp			\
						DisplayBuffer.cpp

FILE_SRCS 			:=	$(addprefix $(FILE_DIR), $(FILE_NAME))
FILE_OBJS			:=	$(FILE_SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(FILE_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) $(FILE_OBJS)

fclean : clean
	$(RM) $(NAME)

re :
	make fclean
	make all

.PHONY: all clean fclean re
