

V := 0
M_0 := @
M_1 :=
M = $(M_$(V))

all: test server

clean:
	rm *.o test server || true

CXXFLAGS := -std=gnu++11
CPPFLAGS := -I../lib/pb -I.. -D_UNITTEST -O0 -ggdb

Proto.cpp: ../lib/proto/regs.proto
	$M$(MAKE) -C ../lib/proto Proto.cpp
	$Mmv ../lib/proto/Proto.[ch]pp ./

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

test: test.cpp.o SensorTest.cpp.o ProtocolTest.cpp.o FileStream.cpp.o \
      ProtocolWrapper.cpp.o SensorHandler.cpp.o TestSensors.cpp.o \
      TestSensor.cpp.o compat.cpp.o Proto.cpp.o Subprocess.cpp.o TestServer.cpp.o \
      compat.cpp.o pb_common.c.o pb_encode.c.o pb_decode.c.o proto.c.o Proto.cpp.o
	@echo "Linking" `basename $@` "<" $?
	$M$(CXX) $? -o $@ -lgcc -lcpptest -lprotobuf

server: server.cpp.o TestServer.cpp.o FileStream.cpp.o ProtocolWrapper.cpp.o \
      TestSensor.cpp.o SensorHandler.cpp.o TestSensors.cpp.o \
      compat.cpp.o pb_common.c.o pb_encode.c.o pb_decode.c.o proto.c.o Proto.cpp.o
	@echo "Linking" `basename $@` "<" $?
	$M$(CXX) $? -o $@ -lgcc -lprotobuf

run: test
	@echo "Running tests.."
	$M./test || ( rm test && false )
