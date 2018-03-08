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
INSTALLHDR	:= /usr/local/include
INSTALLLIB	:= /usr/local/lib

.PHONY: all

all: build

libxcorr:
	@cd libxcorr && mkdir -p build && cd build && cmake .. && make && sudo make install

lint:
	@$(foreach file,$(SRCS) $(HDRS),clang-format -i $(file);)

build: pitch_detection.so libxcorr

pitch_detection.so: directories $(OBJS)
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lxcorr $(CXX_FLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

install: build
	@sudo cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	@sudo cp $(LIBDIR)/pitch_detection.so $(INSTALLLIB)

example: sinewave stdin

sinewave: pitch_detection.so $(EXAMPLEDIR)/sinewave.cpp

stdin: pitch_detection.so $(EXAMPLEDIR)/stdin.cpp

$(EXAMPLEDIR)/*.cpp: $(LIBDIR)/pitch_detection.so
	$(CXX) $@ $^ $(CXX_FLAGS) -o $(BINDIR)/$(basename $(notdir $@)) -I$(INCLUDEDIR) -lgflags

.PHONY: libxcorr clean
