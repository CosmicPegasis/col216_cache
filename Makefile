CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic

SRCDIR := src
BUILDDIR := build
INCLUDE := include
TARGET := cacheSim

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -I$(INCLUDE) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CXX) -I$(INCLUDE) $(CXXFLAGS) -c -o $@ $<

clean:
	@$(RM) -r $(BUILDDIR)/* $(TARGET)


format:
	clang-format -i **/*.cpp **/*.h

.PHONY: clean format
