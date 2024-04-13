CXX := g++
CXXFLAGS := -std=c++14 -Wall -Wextra -pedantic

SRCDIR := src
BUILDDIR := build
LIBDIR := lib
TESTDIR := test
INCLUDE := include
TARGET := cacheSim
TEST_TARGET := tests

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
TESTS := $(wildcard $(TESTDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TEST_SRC_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDIR)/%,$(TESTS:.$(SRCEXT)=.o))
TEST_OBJECTS := $(filter-out  $(BUILDDIR)/main.o, $(OBJECTS)) $(TEST_SRC_OBJECTS) 

TEST_FLAGS := -I$(INCLUDE) -L$(LIBDIR)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -I$(INCLUDE) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	@mkdir -p $(@D)
	$(CXX) -I$(INCLUDE) $(CXXFLAGS) -c -o $@ $<

clean:
	@$(RM) -r $(BUILDDIR)/* $(TARGET)
	@$(RM) test/*.o
	@$(RM) tests

format:
	clang-format -i **/*.cpp **/*.h

$(TESTDIR)/%.o: $(TESTDIR)/%.$(SRCEXT) 
	$(CXX) -I$(INCLUDE) -c -o $@ $<

tests: $(TEST_OBJECTS)
	g++ $(TESTFLAGS) $(TEST_OBJECTS) -lgtest_main -lgtest -o $(TEST_TARGET)
	./tests

.PHONY: clean format tests
