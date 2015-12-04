CC=g++ -Wall

all: producer consumer start console

start: start.o helper.o
	$(CC) -o start start.o helper.o

start.o: start.cc helper.cc helper.h
	$(CC) -c start.cc helper.cc

producer: producer.o helper.o
	$(CC) -o producer producer.o helper.o

producer.o: producer.cc helper.cc helper.h
	$(CC) -c producer.cc helper.cc

consumer: consumer.o helper.o
	$(CC) -o consumer consumer.o helper.o

consumer.o: consumer.cc helper.cc helper.h
	$(CC) -c consumer.cc helper.cc

console: console.o
	$(CC) -o console console.o

console.o: console.cc
	$(CC) -c console.cc

tidy:
	rm -f *.o core

clean:
	rm -f start producer consumer *.o core
