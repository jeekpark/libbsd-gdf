NAME				:=	../../../lib/libbsd-gdf-network.dylib
CXX					:=	c++
CXXFLAGS			:=	-Wall -Wextra -Werror -std=c++98 -I../../../include
LDFLAGS				:=	-dynamiclib -install_name '@rpath/libbsd-gdf-network.dylib' -L../../../lib -Wl,-rpath,../../../lib
LDLIBS				:=	-lbsd-gdf-logger

FILE_DIR			:=	./
FILE_NAME			:=	Network.cpp

FILE_SRCS 			:=	$(addprefix $(FILE_DIR), $(FILE_NAME))
FILE_OBJS			:=	$(FILE_SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(FILE_OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

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
