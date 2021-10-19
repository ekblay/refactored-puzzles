all: server.cpp client.cpp execute.cpp
	g++ execute.cpp -o execute  -lrt -lcrypto -w -lgmpxx -lgmp
	g++ server.cpp -o server -lpthread  -lcrypto -w -lgmpxx -lgmp
	g++ client.cpp -o client -lcrypto
clean:
	rm execute
