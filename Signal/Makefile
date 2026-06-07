#signal:signal.cc
#	g++ -w -o $@ $^ -std=c++17

#.PHONY:clean
#clean:
#	rm -f signal

.PHONY:all
all: signal mykill

mykill:mykill.cc
	g++ -w -o $@ $^ -std=c++17

signal:signal.cc
	g++ -w -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f mykill signal