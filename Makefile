#MakeFile to build and deploy SearchBooks
# For CSC3004 Software Development

# Put your user name below:
USER= skon

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14

RM= /bin/rm -f

all: genGraph PutHTML PutCGI

genGraph.o: genGraph.cpp Node.h Point.h Graph.h
	$(CC) -c $(CFLAGS) genGraph.cpp

Point.o: Point.cpp Point.h
	$(CC) -c $(CFLAGS) Point.cpp

Node.o: Node.cpp Node.h
	$(CC) -c $(CFLAGS) Node.cpp

Graph.o: Graph.cpp Graph.h
	$(CC) -c $(CFLAGS) Graph.cpp

genGraph: Node.o Point.o Graph.o genGraph.o
	$(CC) $(CFLAGS) genGraph.o Point.o Node.o Graph.o -o genGraph -L/usr/local/lib -lcgicc


PutHTML: MSView.js MSView.html MSView.css
	cp MSViewK.html /var/www/html/research/$(USER)/MSSIG
	cp MSViewK.js /var/www/html/research/$(USER)/MSSIG
	cp MSView.html /var/www/html/research/$(USER)/MSSIG
	cp MSView.js /var/www/html/research/$(USER)/MSSIG
	cp MSView.css /var/www/html/research/$(USER)/MSSIG
	cp gears.gif /var/www/html/research/$(USER)/MSSIG

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/research/$(USER)/MSSIG

PutCGI: genGraph
	chmod 757 genGraph
	cp genGraph /usr/lib/cgi-bin/$(USER)_genGraphSig3.cgi

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

clean:
	rm *.o genGraph 
