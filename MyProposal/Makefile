all: server.cpp client.cpp
	g++ server.cpp -o server -lpthread -lcrypto -w -lgmpxx -lgmp
	g++ client.cpp -o client -lpthread -lcrypto

clean:
	rm server client
