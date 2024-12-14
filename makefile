all: submission

submission: submission.o
	g++ submission.o -o submission
	
submission.o: submission.cpp
	g++ -c submission.cpp

clean:
	rm -f *.o submission
