# project name
TARGET   = rotatingcube

# compiler flags
CXX      = g++
CXXFLAGS = -std=c++11

# file directories
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
INCDIR   = include

# files
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# default make command - will compile program with $(CXXFLAGS)
#						 and the -O3 optomization
#						 Also turns off debugging
release: CXXFLAGS += -O3 -DNDEBUG
release: $(BINDIR)/$(TARGET)

# make debug - will compile program with $(CXXFLAGS) and the -g flag
#              also defines DEBUG so that "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG -gdwarf-3
debug: clean $(BINDIR)/$(TARGET)

# compiles executable into the $(BINDIR) folder
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)
	@echo "Linking Complete!"

# individual object compliation
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" Successfully!"

# make clean - removes executable as well as all .o files in $(OBJDIR)
clean:
	@rm -f $(BINDIR)/$(TARGET) $(OBJECTS)
	@echo "Cleanup Complete!"
