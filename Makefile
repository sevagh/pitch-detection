SRCDIR 		:= src
SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
BUILDDIR 	:= obj
TARGETDIR	:= lib
INCLUDEDIR	:= include
CXX_FLAGS 	:=-ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC
OBJS 		:= ${SRCS:cpp=o} 

.PHONY: all

all: directories $(OBJS)

%.o: %.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $(BUILDDIR)/$(notdir $@)

directories:
	@mkdir -p $(BUILDDIR) $(TARGETDIR)

clean:
	-rm -rf $(BUILDDIR) $(TARGETDIR)

#set (LIBS "-lxcorr")
