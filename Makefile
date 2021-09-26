all: server.cpp client.cpp
	g++ server.cpp -o server -lpthread
	g++ client.cpp -o client -lpthread
	g++ -o crypt crypt.cpp -lcrypto 
clean:
	rm server client crypt
