

.PHONY: proto

all: clean proto regs_pb.c

clean:
	rm regs_pb.[ch]* || true

proto:
	$(MAKE) -C $@

regs_pb.c:
	mv proto/regs_pb.[hc] ./

