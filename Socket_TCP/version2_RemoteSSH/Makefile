.PHONY:all
all:server_ssh client_ssh

server_ssh:SSH_Server.cc
	g++ -o $@ $^ -std=c++17 -lpthread
client_ssh:SSH_Client.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f server_ssh client_ssh