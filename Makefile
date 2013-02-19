all:
	g++ -o smpp_client main.cpp tcp_client.hpp smpp_defs.cpp smpp_client.cpp -lboost_system -lboost_thread -std=c++0x -g
