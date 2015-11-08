optimize: optimize.main.o optimize.o
	g++ -o optimize optimize.main.o optimize.o

optimize.main.o: optimize.main.cpp optimize.h
	g++ -c optimize.main.cpp

optimize.o: optimize.cpp optimize.h
	g++ -c optimize.cpp

merge: merge.main.o merge.o
	g++ -std=c++11 -o merge merge.main.o merge.o

merge.main.o: merge.main.cpp merge.h
	g++ -std=c++11 -c merge.main.cpp

merge.o: merge.cpp merge.h
	g++ -std=c++11 -c merge.cpp

clean:
	rm -f *.o *~
