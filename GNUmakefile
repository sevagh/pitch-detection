SRCDIR 		:= src
EXAMPLEDIR	:= example
SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
OBJDIR	 	:= obj
LIBDIR		:= lib
INCLUDEDIR	:= include
HDRS		:= $(wildcard $(INCLUDEDIR)/*.h)
BINDIR		:= bin
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto
OBJS  		:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
INSTALLHDR	:= /usr/include
INSTALLLIB	:= /usr/lib

.PHONY: all

all: build install

libxcorr:
	@cd libxcorr && mkdir -p build && cd build && cmake .. && make && sudo make install

lint:
	@$(foreach file,$(SRCS) $(HDRS),clang-format -i $(file);)

build: libpitch_detection.so libxcorr

libpitch_detection.so: directories $(OBJS)
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lxcorr $(CXX_FLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

install: build
	sudo cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	sudo cp $(LIBDIR)/libpitch_detection.so $(INSTALLLIB)

example: stdin

stdin:
	$(CXX) $(EXAMPLEDIR)/stdin.cpp $(CXX_FLAGS) -o $(BINDIR)/$(basename $(notdir $@)) -lgflags -lpitch_detection

.PHONY: libxcorr clean
