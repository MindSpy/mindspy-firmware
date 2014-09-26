

V := 0
M_0 := @
M_1 :=
M = $(M_$(V))

all: test

clean:
	rm *.o test || true

CPPFLAGS := -I../lib/pb -I.. -D_UNITTEST

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

test: test.cpp.o MemoryStream.cpp.o ProtocolWrapper.cpp.o TestSensor.cpp.o SensorHandler.cpp.o SensorDetector.cpp.o pb_common.c.o pb_encode.c.o pb_decode.c.o proto.c.o compat.cpp.o
	@echo "Linking" `basename $@` "<" $?
	$M$(CXX) $? -o $@ -lgcc -lcpptest
	@echo "Running tests.."
	$M./test || ( rm test && false )
