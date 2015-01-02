OBJECTS = main.o  
INC = -Wall -I/usr/local/include
LIBS = -L/usr/local/lib -lm  -L/usr/local/boost 
GSL_LIBS = -lgsl -lgslcblas
FFT_LIBS = -lfftw3 -lm
BOOST_LIBS = -lboost_filesystem -lboost_system -lboost_timer
CC = g++

all: main

%.o:     %.cpp
	$(CC) -c $(INC) $< -o $@

main: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) $(GSL_LIBS) $(BOOST_LIBS) $(FFT_LIBS) -o $@

clean:
	-rm -f $(OBJECTS) main

