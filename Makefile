SRCDIR 		:= src
SAMPLESDIR	:= samples
SRCS		:= $(wildcard $(SRCDIR)/*.cpp)
OBJDIR	 	:= obj
LIBDIR		:= lib
INCLUDEDIR	:= include
BINDIR		:= bin
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC
OBJS  		:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all

all: directories $(OBJS) pitch_detection.so

pitch_detection.so:
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lxcorr

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

sinewave: $(SAMPLESDIR)/sinewave.cpp

$(SAMPLESDIR)/*.cpp: $(LIBDIR)/pitch_detection.so
	$(CXX) $@ $^ $(CXX_FLAGS) -o $(BINDIR)/$(basename $(notdir $@)) -I$(INCLUDEDIR) -lgflags
