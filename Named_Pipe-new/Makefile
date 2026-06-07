# 同时创建 Server 和 Client
.PHONY:all
all: Client Server

Server:Server.cc
	g++ -o $@ $^ -std=c++17
Client:Client.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f Server Client
