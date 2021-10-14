all: server.cpp client.cpp
	g++ server.cpp -o server -lpthread -lcrypto -w -lgmpxx -lgmp
	g++ client.cpp -o client -lpthread -lcrypto
	# g++ crypt.cpp -o crypt -lcrypto -w -lgmpxx -lgmp -lstdc++fs
clean:
	rm server client
