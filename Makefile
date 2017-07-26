SRCDIR 		:= src
SAMPLESDIR	:= samples
SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
OBJDIR	 	:= obj
LIBDIR		:= lib
INCLUDEDIR	:= include
BINDIR		:= bin
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals
OBJS  		:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
INSTALLHDR	:= /usr/local/include
INSTALLLIB	:= /usr/local/lib

.PHONY: all

all: pitch_detection.so

pitch_detection.so: directories $(OBJS)
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lfftw3

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

src_clean:
	-find . -type f -name '*~' -exec rm -rf {} \;

install:
	-cp $(INCLUDEDIR)/pitch_detection.hpp $(INSTALLHDR)
	-cp $(LIBDIR)/pitch_detection.so $(INSTALLLIB)


sinewave: pitch_detection.so $(SAMPLESDIR)/sinewave.cpp

$(SAMPLESDIR)/*.cpp: $(LIBDIR)/pitch_detection.so
	$(CXX) $@ $^ $(CXX_FLAGS) -o $(BINDIR)/$(basename $(notdir $@)) -I$(INCLUDEDIR) -lgflags
