SRCDIR 		:= src
EXAMPLEDIR	:= examples
LIBDIR		:= lib
BINDIR		:= bin
INCLUDEDIR	:= include
INSTALLHDR	:= /usr/include
INSTALLLIB	:= /usr/lib

SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
EXAMPLE_SRCS	:= $(wildcard $(EXAMPLEDIR)/*.cpp)
EXAMPLE_HDRS	:= $(wildcard $(EXAMPLEDIR)/*.h)
HDRS		:= $(wildcard $(INCLUDEDIR)/*.h)

CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto

BINS  		:= $(EXAMPLE_SRCS:$(EXAMPLEDIR)/%.cpp=$(BINDIR)/%)

FFT_FLAG	?= -lffts

.PHONY: all

all: build

fmt:
	@$(foreach file,$(SRCS) $(HDRS) $(EXAMPLE_SRCS) $(EXAMPLE_HDRS),clang-format -i $(file);)

build: directories
	$(CXX) -shared -o $(LIBDIR)/libpitch_detection.so $(FFT_FLAG) $(CXX_FLAGS) $(SRCS) -I$(INCLUDEDIR)

directories:
	@mkdir -p $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(LIBDIR) $(BINDIR)

install: build
	cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	cp $(LIBDIR)/libpitch_detection.so $(INSTALLLIB)

examples: build
examples: $(BINS)

$(BINDIR)/%: $(EXAMPLEDIR)/%.cpp
	$(CXX) $< $(LIBDIR)/libpitch_detection.so $(CXX_FLAGS) -o $@ $(FFT_FLAG) -I$(INCLUDEDIR) -lgflags

.PHONY: clean
