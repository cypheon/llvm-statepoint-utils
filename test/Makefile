CC := cc
OPT_CC := ${OPT_FLAG} -DPRINT_STUFF -fPIC
OPT_LLC := -relocation-model=pic
LD_FLAGS := -lm

all: statepoints a.out

a.out: fib.o driver.o shim.o ../dist/llvm-statepoint-tablegen.a
	$(CC) $(OPT_CC) $^ $(LD_FLAGS)

fib.o: fib.ll
	llc $(OPT_LLC) fib.ll -o fib.s
	perl -i -pe "s/__LLVM_StackMaps:/.globl __LLVM_StackMaps\n__LLVM_StackMaps:/" fib.s
	$(CC) $(OPT_CC) -c fib.s -o fib.o

shim.o: shim.S
	cc -c -o $@ $^

driver.o: driver.c
	$(CC) $(OPT_CC) -c driver.c -o driver.o

statepoints:
	cd .. && make

clean:
	rm -f fib.s fib.o driver.o a.out output.txt && cd .. && make clean
