#makefile for zynfunction library
libzynfunc.so : libzynfunc.h libzynfunc.c
	gcc libzynfunc.c -fPIC -shared -o libzynfunc.so
.PHONY : clean
clean :
	-rm libzynfunc.so
