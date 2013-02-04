del *.o

ar -x libc.a

del lib_a-fcntlr.o
del lib_a-freer.o
del lib_a-mallinfor.o
del lib_a-mallocr.o
del lib_a-malloptr.o
del lib_a-mstats.o
del lib_a-mtrim.o
del lib_a-raise.o
del lib_a-signal.o
del lib_a-signalr.o

ar rs libc_new.a *.o

del *.o
pause