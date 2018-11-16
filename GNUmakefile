CXX		?= gcc

INSTALLHDR	:= /usr/include
INSTALLLIB	:= /usr/lib

LIBDIR		:= lib
BINDIR		:= bin

PITCH_SRCDIR 		:= src
PITCH_INCLUDEDIR	:= include
PITCH_SRCS		:= $(wildcard $(PITCH_SRCDIR)/*.cpp)
PITCH_HDRS		:= $(wildcard $(PITCH_INCLUDEDIR)/*.h)

UTILDIR		:= util
UTIL_SRCS	:= $(wildcard $(UTILDIR)/*.cpp)
UTIL_HDRS	:= $(wildcard $(UTILDIR)/*.h)

TESTDIR		:= test
TEST_SRCS	:= $(wildcard $(TESTDIR)/*.cpp)
TEST_HDRS	:= $(wildcard $(TESTDIR)/*.h)

EXAMPLEDIR	:= example
EXAMPLE_SRCS	:= $(wildcard $(EXAMPLEDIR)/*.cpp)
EXAMPLE_HDRS	:= $(wildcard $(EXAMPLEDIR)/*.h)

CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto

BINS  		:= $(EXAMPLE_SRCS:$(EXAMPLEDIR)/%.cpp=$(BINDIR)/%)

FFT_FLAG	?= -lffts

.PHONY: all

all: build

fmt:
	@$(foreach file,$(PITCH_SRCS) $(PITCH_HDRS) $(EXAMPLE_SRCS) $(EXAMPLE_HDRS) $(UTIL_SRCS) $(UTIL_HDRS) $(TEST_SRCS) $(TEST_HDRS),clang-format -i $(file);)

build: directories
	$(CXX) -shared -o $(LIBDIR)/libpitch_detection.so $(FFT_FLAG) $(CXX_FLAGS) $(PITCH_SRCS) -I$(PITCH_INCLUDEDIR)

util: directories
	$(CXX) -shared -o $(LIBDIR)/util.so $(CXX_FLAGS) $(UTIL_SRCS) -I$(UTILDIR)

directories:
	@mkdir -p $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(LIBDIR) $(BINDIR)

install: build
	cp $(PITCH_INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	cp $(LIBDIR)/libpitch_detection.so $(INSTALLLIB)

test: build
test: util
test:
	$(CXX) $(CXX_FLAGS) $(LIBDIR)/*.so $(TEST_SRCS) -o $(BINDIR)/tests -I$(PITCH_INCLUDEDIR) -I$(UTILDIR) -lpthread -lgtest $(FFT_FLAG)

example: build
example: util
example: $(BINS)

$(BINDIR)/%: $(EXAMPLEDIR)/%.cpp
	$(CXX) $< $(LIBDIR)/*.so $(CXX_FLAGS) -o $@ $(FFT_FLAG) -I$(PITCH_INCLUDEDIR) -I$(UTILDIR) -lgflags

.PHONY: clean
