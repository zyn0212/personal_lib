#makefile for zynfunction library
libzynfunc.so : zynfunc.h zynfunc.c zyn_sorts.c
	gcc zynfunc.c zyn_sorts.c -fPIC -shared -o libzynfunc.so
.PHONY : clean
clean :
	-rm libzynfunc.so
