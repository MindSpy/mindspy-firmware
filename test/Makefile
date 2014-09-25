

V := 0
M_0 := @
M_1 :=
M = $(M_$(V))

all: test

clean:
	rm *.o test || true

CPPFLAGS := -I../lib/pb

%.cpp.o: %.cpp
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.cpp) -o $@ $<

TestSensor.cpp.o: ../TestSensor.cpp
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.cpp) -o $@ $<

SensorHandler.cpp.o: ../SensorHandler.cpp
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.cpp) -o $@ $<

%.c.o: ../lib/pb/%.c
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.c) -o $@ $<

proto.c.o: ../proto.c
	@echo "Compiling <" `basename $<`
	$Mmkdir -p $(dir $@)
	$M$(COMPILE.c) -o $@ $<

test: test.cpp.o SensorHandler.cpp.o pb_common.c.o pb_encode.c.o pb_decode.c.o proto.c.o
	@echo "Linking" `basename $@` "<" $<
	$M$(CXX) $< -o $@ -lgcc -lcpptest
	@echo "Running tests.."
	$M./test || rm test && false
