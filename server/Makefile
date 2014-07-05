CC=g++
GG=-Wall -std=c++0x

agendaX: Date User Meeting Storage AgendaService Xserver AgendaX.cpp
	$(CC) $(GG) -o agendaX Date.o User.o Meeting.o Storage.o AgendaService.o Xserver.o AgendaX.cpp

Date: Date.h Date.cpp
	$(CC) $(GG) -c Date.cpp

User: User.h User.cpp
	$(CC) $(GG) -c User.cpp

Meeting: Meeting.h Meeting.cpp
	$(CC) $(GG) -c Meeting.cpp

Storage: Storage.h Storage.cpp
	$(CC) $(GG) -c Storage.cpp

AgendaService: AgendaService.h AgendaService.cpp
	$(CC) $(GG) -c AgendaService.cpp

Xserver: Xserver.cpp Xserver.h
	$(CC) $(GG) -c Xserver.cpp

clean :
	rm *.o *~ agendaX

