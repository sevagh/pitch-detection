CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto -shared

all: lib

lib:
	@mkdir -p lib
	$(CXX) $(CXX_FLAGS) -o lib/libpitch_detection.so -lffts -Iinclude \
		src/pyin.cpp \
		src/yin.cpp \
		src/mpm.cpp \
		src/autocorrelation.cpp \
		src/parabolic_interpolation.cpp

install: lib
	cp -r include/ /usr/include/pitch_detection
	cp lib/libpitch_detection.so /usr/lib

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf lib


.PHONY: clean fmt install
