

V := 0
M_0 := @
M_1 :=
M = $(M_$(V))

all: test.elf

clean:
	rm *.o test.elf || true

CXXFLAGS := -std=gnu++11
CPPFLAGS := -I../lib/pb -I.. -D_UNITTEST -O0 -ggdb

%.cpp.o: %.cpp
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.cpp) -o $@ $<

%.cpp.o: ../%.cpp
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.cpp) -o $@ $<

%.c.o: ../lib/pb/%.c
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.c) -o $@ $<

%.c.o: ../%.c
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.c) -o $@ $<

test.elf: UnitTests.cpp.o MemoryStream.cpp.o ProtocolWrapper.cpp.o TestSensor.cpp.o SensorHandler.cpp.o SensorDetector.cpp.o pb_common.c.o pb_encode.c.o pb_decode.c.o proto.c.o compat.cpp.o
	@echo "Linking" `basename $@` "<" $?
	$M$(CXX) $? -o $@ -lgcc -lcpptest

run: test.elf
	@echo "Running tests.."
	$M./test.elf || ( rm test && false )
