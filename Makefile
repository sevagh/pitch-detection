CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto -shared -lffts -Iinclude -fopenmp -lmlpack

all: lib

lib:
	@mkdir -p lib
	$(CXX) $(CXX_FLAGS) -o lib/libpitch_detection.so \
		src/hmm.cpp \
		src/pyin.cpp \
		src/yin.cpp \
		src/mpm.cpp \
		src/autocorrelation.cpp \
		src/parabolic_interpolation.cpp

install: lib
	cp include/pitch_detection.h /usr/include
	cp lib/libpitch_detection.so /usr/lib

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf lib


.PHONY: clean fmt install
