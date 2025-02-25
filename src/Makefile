# Параметры
BUILD_DIR = ./build
FFMPEG_LIB = ./ffmpeg/lib
APP_NAME = 3DViewer

# Параметры для вызова cmake
CMAKE := cmake
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release
BUILD_DIR_TEST = ./build-test

GCP_FLAGS = -std=c++17 -Wall -Wextra -Werror -ld64
COVERAGE_FLAGS=-fprofile-arcs -ftest-coverage
TEST_DIR = ./tests
PARSER = ./model/parser
AFFINE = ./model/affineTransformations
CONTROLLER = ./controller
MODEL = ./model
INCLUDES = -I$(MODEL) -I$(SRC_DIRS) -I$(CONTROLLER) -I$(PARSER) -I$(AFFINE) 

SRC_DIRS = .
OBJ_DIR = ./objs
TEST_DIR = ./tests
TEST_OBJ_DIR = ./tests/objs
BUILD_PATH = gcov_report/
REPORT_PATH = $(BUILD_PATH)report/
COVERAGE_FLAGS=-fprofile-arcs -ftest-coverage


all: install

install: $(BUILD_DIR) $(FFMPEG_LIB)
	cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) ../ && $(MAKE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(FFMPEG_LIB):
	mkdir -p $(FFMPEG_LIB)

start:
	./build/3DViewer

dvi: clean
	open dvi/Documentation.html

dist: install
	tar -cvzf $(APP_NAME).tgz build
	rm -rf build

unistall:
	rm -rf $(BUILD_DIR)

test: $(BUILD_DIR_TEST)
	g++ $(GCP_FLAGS) $(COVERAGE_FLAGS) $(INCLUDES) $(TEST_DIR)/*.cc $(PARSER)/*.cc $(AFFINE)/*.cc -o $(BUILD_DIR_TEST)/test -lgtest
	./$(BUILD_DIR_TEST)/test


$(BUILD_DIR_TEST):
	mkdir -p $(BUILD_DIR_TEST)

gcov_report: test
	lcov -t "test" -o $(BUILD_DIR_TEST)test.info -c -d . --no-external --ignore-errors mismatch,mismatch,inconsistent,version
	genhtml -o report $(BUILD_DIR_TEST)test.info
	open ./report/index.html


clean:
	rm -rf $(BUILD_DIR)
	rm -rf 	build-desktop-Desktop_x86_darwin_generic_mach_o_64bit-Debug
	rm -rf view/build
	rm ffmpeg/lib/*
	rm -rf $(BUILD_DIR_TEST)
	rm -rf report


style_test:
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc *.h
	clang-format -n ./*.h ./*.cc
	clang-format -n ./*/*.h ./*/*.cc
	clang-format -n ./*/*/*.h ./*/*/*.cc ./*/*/*.glsl
	rm .clang-format

format_style:
	cp ../materials/linters/.clang-format .
	clang-format -i *.cc *.h
	clang-format -i ./*.h ./*.cc
	clang-format -i ./*/*.h ./*/*.cc
	clang-format -i ./*/*/*.h ./*/*/*.cc ./*/*/*.glsl
	rm .clang-format
