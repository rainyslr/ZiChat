client:client.o in-out.o
	clang++ client.o in-out.o -g -o client
client.o:client.cc config.h
	clang++ -g -c client.cc
in-out.o:in-out.cc in-out.h
	clang++ -g -c in-out.cc
