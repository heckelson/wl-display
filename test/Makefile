BUILD=../build/test
TARGET=../build/test/test_main

OPTIONS=-g3 -O0 -std=c++17 -pedantic -Wall -Wextra -Wunreachable-code -fdiagnostics-all-candidates
INCLUDES=-I../include -I../src
LINKS=-lssl -lcrypto
OPENSSL_DIR = /usr/include

all: test_main

test_main: \
	$(BUILD)/wl.o \
	$(BUILD)/TestNetworkMgr/TestNetworkMgr.o \
	$(BUILD)/AppMain/AppMain.o \
	builddir

	g++ test_main.cpp \
    $(BUILD)/wl.o \
	$(BUILD)/TestNetworkMgr/TestNetworkMgr.o \
	$(BUILD)/AppMain/AppMain.o \
	-o $(TARGET) $(OPTIONS) $(INCLUDES) $(LINKS)

	printf "\n\n================== Running test_main: ==================\n"

	$(TARGET)

$(BUILD)/wl.o: builddir
	g++ ../src/wl/wl.cpp -c -o $(BUILD)/wl.o $(OPTIONS) $(INCLUDES)

$(BUILD)/TestNetworkMgr/TestNetworkMgr.o: builddir
	mkdir -p $(BUILD)/TestNetworkMgr/
	g++ TestNetworkMgr/TestNetworkMgr.cpp -c -o $(BUILD)/TestNetworkMgr/TestNetworkMgr.o $(OPTIONS) $(INCLUDES)

$(BUILD)/AppMain/AppMain.o: builddir
	mkdir -p $(BUILD)/AppMain/
	g++ ../src/AppMain/AppMain.cpp -c -o $(BUILD)/AppMain/AppMain.o $(OPTIONS) $(INCLUDES)

builddir:
	mkdir -p ../build/test/

memtest: test_main TestNetworkMgr
	valgrind --leak-check=yes $(TARGET)

clean:
	rm -rf $(BUILD)
