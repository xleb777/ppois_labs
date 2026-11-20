CXX ?= g++
GCOV ?= gcov
BUILD ?= build/default
ifeq ($(OS),Windows_NT)
EXEEXT ?= .exe
endif
EXEEXT ?=
CPPFLAGS += -Iinclude
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic -Werror
CATCH_LIBS := $(shell pkg-config --libs catch2-with-main 2>/dev/null)

LIB_OBJECTS := $(addprefix $(BUILD)/,src/vector3d.o src/set.o src/menu.o)
MAIN_OBJECT := $(BUILD)/src/main.o
TEST_OBJECTS := $(addprefix $(BUILD)/,tests/test_main.o tests/vector_tests.o tests/set_tests.o tests/menu_tests.o)
OBJECTS := $(LIB_OBJECTS) $(MAIN_OBJECT) $(TEST_OBJECTS)

.PHONY: all test coverage lint docs release clean
all: $(BUILD)/lab1$(EXEEXT)

$(BUILD)/lab1$(EXEEXT): $(LIB_OBJECTS) $(MAIN_OBJECT)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD)/lab1-tests$(EXEEXT): $(LIB_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(CATCH_LIBS) -o $@

$(BUILD)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

test: $(BUILD)/lab1-tests$(EXEEXT)
	$<

coverage:
	mkdir -p build/coverage/report
	find build/coverage -name '*.gcda' -delete
	$(MAKE) BUILD=build/coverage CXXFLAGS='$(CXXFLAGS) -O0 -g --coverage' LDFLAGS='--coverage' all test
	printf '0\n' | build/coverage/lab1$(EXEEXT) >/dev/null
	gcovr --gcov-executable "$(GCOV)" --root . --filter 'src/|include/' --fail-under-line 90.01 \
		--txt build/coverage/report/summary.txt --html-details build/coverage/report/index.html build/coverage
	cat build/coverage/report/summary.txt

lint:
	cppcheck --std=c++17 --enable=warning,style,performance,portability --error-exitcode=1 -Iinclude src

docs:
	mkdir -p build/docs
	doxygen Doxyfile

release:
	$(MAKE) BUILD=build/release CXXFLAGS='$(CXXFLAGS) -O2' LDFLAGS='-static' all

clean:
	rm -rf build
