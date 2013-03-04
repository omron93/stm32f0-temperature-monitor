#patch to libraries
INCLUDE=`pkg-config gtkmm-3.0 --cflags --libs`

build: main.o
		g++ -o main main.o $(INCLUDE)
		
clean:
		rm *.o main
		
main.o: main.cpp gui.glade
		g++ main.cpp -c $(INCLUDE)
