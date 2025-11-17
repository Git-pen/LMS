CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = library_system
SRCDIR = src
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/entities/Book.cpp \
          $(SRCDIR)/entities/User.cpp \
          $(SRCDIR)/entities/Transaction.cpp \
          $(SRCDIR)/management/LibraryManager.cpp \
          $(SRCDIR)/management/AuthManager.cpp \
          $(SRCDIR)/utils/BookBST.cpp \
          $(SRCDIR)/utils/UserHashMap.cpp \
          $(SRCDIR)/utils/TransactionList.cpp \
          $(SRCDIR)/utils/SearchEngine.cpp \
          $(SRCDIR)/utils/FileHandler.cpp

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Create object directory structure
$(shell mkdir -p $(OBJDIR)/entities $(OBJDIR)/management $(OBJDIR)/utils)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Clean complete!"

# Clean and rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
