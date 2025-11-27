TARGET := arkanoid_m3
SRC_DIR := Logica
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
INCLUDE_DIRS := Cabeçalho

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra $(foreach dir,$(INCLUDE_DIRS),-I$(dir))
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
