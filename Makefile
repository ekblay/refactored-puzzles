all: server.cpp client.cpp
	g++ server.cpp -o server -lpthread
	g++ client.cpp -o client -lpthread
clean:
	rm server client
