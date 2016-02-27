server:server.o in-out.o
	clang++ server.o in-out.o -o server
server.o:server.cc config.h 
	clang++ -c server.cc
in-out.o:in-out.cc in-out.h
	clang++ -c in-out.cc
