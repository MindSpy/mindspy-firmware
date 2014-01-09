

default:
	protoc --plugin=/home/pborky/tmp/nanopb/generator/protoc-gen-nanopb --nanopb_out=. -o registers_pb regs.proto
	protoc  --python_out=. regs.proto
	mv regs.pb.h regs_pb.h
	mv regs.pb.c regs_pb.c
	sed -i 's/regs[.]pb[.]h/regs_pb.h/' regs_pb.c
