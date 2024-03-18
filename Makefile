TARGET				=	TEST

CXX						= c++
CXXFLAGS			=	-I./gdf/include -I./

LDFLAGS				=	-L./gdf/lib -Wl,-rpath,./gdf/lib
LDLIBS				=	-lbsd-gdf-assert -lbsd-gdf-logger

FILE_DIR			:=	./
FILE_NAME			:=	test.cpp

FILE_SRCS 			:=	$(addprefix $(FILE_DIR), $(FILE_NAME))
FILE_OBJS			:=	$(FILE_SRCS:.cpp=.o)

all : $(TARGET)

$(TARGET) : $(FILE_OBJS)
	make all -C gdf
	$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -rf $(FILE_OBJS) $(TARGET)

re : clean
	make all

.PHONY: all clean clangd re
