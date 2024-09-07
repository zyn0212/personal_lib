#makefile for zynfunction library
libzynfunc.so : zyn_func.h zyn_linkedlist.c zyn_generalSort.c zyn_intMath.c zyn_strMath.c zyn_others.c
	gcc zyn_linkedlist.c zyn_generalSort.c zyn_intMath.c zyn_strMath.c zyn_others.c -w -fPIC -shared -o libzynfunc.so
tt : test.c libzynfunc.so
	gcc test.c -o tt -L. -lzynfunc
.PHONY : clean
clean :
	-rm libzynfunc.so tt
