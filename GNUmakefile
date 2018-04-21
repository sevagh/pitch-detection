SRCDIR 		:= src
EXAMPLEDIR	:= example
OBJDIR	 	:= obj
LIBDIR		:= lib
BINDIR		:= bin
INCLUDEDIR	:= include
INSTALLHDR	:= /usr/include
INSTALLLIB	:= /usr/lib

SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
EXAMPLES	:= $(wildcard $(EXAMPLEDIR)/*.cpp)
HDRS		:= $(wildcard $(INCLUDEDIR)/*.h)

CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto

OBJS  		:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
BINS  		:= $(EXAMPLES:$(EXAMPLEDIR)/%.cpp=$(BINDIR)/%)

.PHONY: all

all: build

lint:
	@$(foreach file,$(SRCS) $(HDRS),clang-format -i $(file);)

build: libpitch_detection.so

libpitch_detection.so: directories $(OBJS)
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lfftw3 $(CXX_FLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

install: build
	cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	cp $(LIBDIR)/libpitch_detection.so $(INSTALLLIB)

examples: directories build $(BINS)

$(BINDIR)/%: $(EXAMPLEDIR)/%.cpp
	$(CXX) $< $(LIBDIR)/libpitch_detection.so $(CXX_FLAGS) -o $@ -lfftw3 -I$(INCLUDEDIR)

.PHONY: libxcorr clean
