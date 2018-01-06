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

all: pitch_detection.so

lint:
	@$(foreach file,$(SRCS) $(HDRS),clang-format -i $(file);)

pitch_detection.so: directories $(OBJS)
	$(CXX) $(OBJS) -shared -o $(LIBDIR)/$@ -lxcorr $(CXX_FLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< $(CXX_FLAGS) -I$(INCLUDEDIR) -o $@

directories:
	@mkdir -p $(OBJDIR) $(LIBDIR) $(BINDIR)

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

src_clean:
	-find . -type f -name '*~' -exec rm -rf {} \;

install:
	@cp $(INCLUDEDIR)/pitch_detection.h $(INSTALLHDR)
	@cp $(LIBDIR)/pitch_detection.so $(INSTALLLIB)

example: sinewave stdin

sinewave: pitch_detection.so $(EXAMPLEDIR)/sinewave.cpp

stdin: pitch_detection.so $(EXAMPLEDIR)/stdin.cpp

$(EXAMPLEDIR)/*.cpp: $(LIBDIR)/pitch_detection.so
	$(CXX) $@ $^ $(CXX_FLAGS) -o $(BINDIR)/$(basename $(notdir $@)) -I$(INCLUDEDIR) -lgflags
