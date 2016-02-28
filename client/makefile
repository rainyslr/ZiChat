client:client.o in-out.o packet.o
	clang++ client.o in-out.o packet.o -g -o client
client.o:client.cc config.h
	clang++ -g -c client.cc
in-out.o:in-out.cc in-out.h
	clang++ -g -c in-out.cc
packet.o:packet.cc packet.h
	clang++ -g -c packet.cc
