
# This is the main compiler
# CXX := g++
# CXX := clang --analyze # and comment out the linker last line for sanity
SRCDIR := ./
INCDIR := include
BUILDDIR := build
BINDIR := bin
TARGET := fast-gpio
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CXXFLAGS := -g # -Wall
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
OMEGA_VERSION := "2"
override CXXFLAGS += -D OMEGA_VERSION=$(OMEGA_VERSION)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"; $(CXX) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo " $(CXX) $(CXXFLAGS) -I include/ -c -o $@ $<"; $(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r *.o $(TARGET)"; $(RM) -r *.o $(TARGET)

bla:
	@echo "$(BLA)"

# Tests
tester:
	$(CXX) $(CXXFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
#ticket:
#  $(CXX) $(CXXFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
