# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files
SOURCES = main.cpp matrix.cpp multithreading.cpp

# Output executable name
TARGET = matrix_multiplication

# PAPI library flag
LIBS = -lpapi

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

# Clean up the build
clean:
	rm -f $(TARGET) *.o
