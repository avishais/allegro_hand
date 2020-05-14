CXX= gcc
CXXFLAGS= 
LDFLAGS= -lLinuxCAN_API


all:
	${CXX} can.cpp -o h ${CXXFLAGS} ${LDFLAGS} -std=c++11 #-Wall