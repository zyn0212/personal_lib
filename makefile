#makefile for zynfunction library
libzynfunc.so : zynfunc.h zyn_linkedlist.c zyn_sorts.c zyn_maths.c zyn_strMath.c zyn_interest.c
	gcc zyn_linkedlist.c zyn_sorts.c zyn_maths.c zyn_strMath.c zyn_interest.c -w -fPIC -shared -o libzynfunc.so
tt : test.c libzynfunc.so
	gcc test.c -o tt -L. -lzynfunc
.PHONY : clean
clean :
	-rm libzynfunc.so tt
