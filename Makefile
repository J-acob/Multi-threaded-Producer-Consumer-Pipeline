# the compiler to use
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -pthread
  
#files to link:
LFLAGS = -lcs50
  
# the name to use for both the target source file, and the output file:
TARGET = line_processor
  
build/$(TARGET): src/$(TARGET).c build/
	$(CC) $(CFLAGS) -o build/$(TARGET) src/$(TARGET).c

build/:
	mkdir build -p

clean:
	rm -rf build/$(TARGET)
	rm -rf data/output/output1.txt
	rm -rf data/output/output2.txt
	rm -rf data/output/output3.txt

data/output: data
	mkdir data/output -p

data/output/output1.txt: data/output
	touch data/output/output1.txt
data/output/output2.txt: data/output
	touch data/output/output2.txt
data/output/output3.txt: data/output
	touch data/output/output3.txt

test1: build/$(TARGET) data/output data/output/output1.txt
	build/$(TARGET) < data/input/input1.txt > data/output/output1.txt

test2: build/$(TARGET) data/output data/output/output2.txt
	build/$(TARGET) < data/input/input2.txt > data/output/output2.txt

test3: build/$(TARGET) data/output data/output/output3.txt
	build/$(TARGET) < data/input/input3.txt > data/output/output3.txt