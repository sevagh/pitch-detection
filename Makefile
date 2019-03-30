CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++2a -fPIC -fext-numeric-literals -flto -shared -lffts -Iinclude -lblas -llapack
CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++2a -fPIC -shared -lffts -Iinclude -ggdb -lblas -llapack -fno-omit-frame-pointer

all: lib

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: lib

lib:
	@mkdir -p lib
	$(CXX) $(CXX_FLAGS) -o lib/libpitch_detection.so \
		src/yin.cpp \
		src/mpm.cpp \
		src/swipe.cpp \
		src/autocorrelation.cpp \
		src/parabolic_interpolation.cpp \
		src/capi.cpp

install: lib
	cp -r include/pitch_detection /usr/local/include
	cp lib/libpitch_detection.so /usr/local/lib

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf lib


.PHONY: clean fmt install
