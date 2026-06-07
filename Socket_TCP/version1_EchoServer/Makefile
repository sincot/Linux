.PHONY:all
all:server_tcp client_tcp

server_tcp:EchoTcpServerMain.cc
	g++ -o $@ $^ -std=c++17 -lpthread
client_tcp:EchoTcpClient.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f server_tcp client_tcp