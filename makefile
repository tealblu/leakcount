CC=clang
CFLAGS= -Wall -g
FILES= leakcount memory_shim.so sctracer

all: $(FILES)

leakcount: leakcount.c 
	$(CC) $(CFLAGS) -o leakcount leakcount.c

memory_shim.so:  memory_shim.c 
	$(CC) $(CFLAGS) -fPIC -shared -o memory_shim.so memory_shim.c -ldl

sctracer: sctracer.c
	$(CC) $(CFLAGS) -o sctracer sctracer.c

clean:
	rm $(FILES) 

tar:
	tar -czvf project1.tgz leakcount.c memory_shim.c sctracer.c README