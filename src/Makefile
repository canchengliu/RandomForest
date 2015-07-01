CC = mpic++
CFLAGS=-c -w -O2
SOURCES=DecisionTree.cpp RandomForest.cpp rfmpi.cpp main.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=rfmpi

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *o rfmpi
	rm -rf decisionTreeOutput.txt
