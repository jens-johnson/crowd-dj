CXX = g++

%.o : %.cpp 
	$(CXX) -c $(CFLAGS) $<

main: 	
	g++ server.cpp -std=c++11 -lcurl -o server	

