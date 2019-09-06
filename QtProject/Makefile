#######################################################
# Noah-IDE Makefile for QtProject
#
# Modified by zhiyb @ GT-Soft
#

CC = g++
MOC = moc.mipsel
COPT = -c -I/mnt/tmp/qt-2.3.10/include -I/mnt/tmp/qtopia-free-1.5.0/include -pipe -DQWS -fno-exceptions -fno-rtti -Wall -Wsign-compare -W -DNO_DEBUG
BOPT = -L/mnt/tmp/qt-2.3.10/lib -Wl,-rpath,/mnt/tmp/qt-2.3.10/lib -lqpe -lqte
MOCCPP = moc_class.cpp moc_misc.cpp
MOCOBJS = moc_class.o moc_misc.o
OBJS = main.o misc.o class.o $(MOCOBJS)
TARG = QtProject

$(TARG): $(OBJS)
	$(CC) $(BOPT) -o $(TARG) $(OBJS)
	strip $(TARG)

main.o: main.cpp 
	$(CC) $(COPT) main.cpp

misc.o: misc.cpp 
	$(CC) $(COPT) misc.cpp

class.o: class.cpp 
	$(CC) $(COPT) class.cpp

moc_class.o: class.h
	rm -f moc_class.cpp
	$(MOC) -o moc_class.cpp class.h
	$(CC) $(COPT) moc_class.cpp

moc_misc.o: misc.h
	rm -f moc_misc.cpp
	$(MOC) -o moc_misc.cpp misc.h
	$(CC) $(COPT) moc_misc.cpp

clean :
	rm -f $(OBJS) $(MOCCPP)
