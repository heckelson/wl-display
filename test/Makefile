SOURCES=test_main.cpp
TARGET=../build/test/test_main

LINKS=-lssl -lcrypto
OPENSSL_DIR = /usr/include

all:
	mkdir -p ../build/test/
	g++ $(SOURCES) -o $(TARGET) $(LINKS) && $(TARGET)

clean:
	rm -f $(TARGET)
