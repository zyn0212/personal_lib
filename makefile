#makefile for zynfunction library
libzynfunc.so : zynfunc.h zynfunc.c
	gcc zynfunc.c -fPIC -shared -o libzynfunc.so
.PHONY : clean
clean :
	-rm libzynfunc.so a.out
