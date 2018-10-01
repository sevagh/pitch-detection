SRCDIR 		:= src
EXAMPLEDIR	:= example
LIBDIR		:= lib
BINDIR		:= bin
INCLUDEDIR	:= include
INSTALLHDR	:= /usr/include
INSTALLLIB	:= /usr/lib

SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
EXAMPLES	:= $(wildcard $(EXAMPLEDIR)/*.cpp)
HDRS		:= $(wildcard $(INCLUDEDIR)/*.h)

CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto

BINS  		:= $(EXAMPLES:$(EXAMPLEDIR)/%.cpp=$(BINDIR)/%)

FFT_FLAG	?= -lffts

.PHONY: all

all: build examples

lint:
	@$(foreach file,$(SRCS) $(HDRS),clang-format -i $(file);)

build: directories
	$(CXX) -shared -o $(LIBDIR)/libpitch_detection.so $(FFT_FLAG) $(CXX_FLAGS) $(SRCS) -I$(INCLUDEDIR)

directories:
	@mkdir -p $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(LIBDIR) $(BINDIR)

install: build
	cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	cp $(LIBDIR)/libpitch_detection.so $(INSTALLLIB)

examples: build directories $(BINS)

$(BINDIR)/%: $(EXAMPLEDIR)/%.cpp
	$(CXX) $< $(LIBDIR)/libpitch_detection.so $(CXX_FLAGS) -o $@ $(FFT_FLAG) -I$(INCLUDEDIR) -lgflags

.PHONY: libxcorr clean
