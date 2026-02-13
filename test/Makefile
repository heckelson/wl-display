BUILD=../build/test
TARGET=../build/test/test_main

OPTIONS=-g3 -O0 -std=c++17 -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code
LINKS=-lssl -lcrypto
OPENSSL_DIR = /usr/include

all: test_main


test_main: $(BUILD)/wl.o builddir
	g++ test_main.cpp $(BUILD)/wl.o -o $(TARGET) $(OPTIONS) $(LINKS) && \
	    echo "================== Running test_main: ==================" && \
		$(TARGET)

$(BUILD)/wl.o: builddir
	g++ ../src/wl/wl.cpp -c -o $(BUILD)/wl.o $(OPTIONS)

builddir:
	mkdir -p ../build/test/

clean:
	rm -rf $(BUILD)
