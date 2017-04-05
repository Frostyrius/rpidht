LIBRARY_LOC=/usr/local/lib
BIN_LOC=/usr/local/bin
INCLUDE_LOC=/usr/local/include

all: dhtquery sethigh toSQL lib

toSQL: src/dhtxx.o src/toSQL.o	
	gcc -O2 -o toSQL src/toSQL.o src/dhtxx.o -l bcm2835 -lsqlite3

src/toSQL.o: src/dhtxx.o src/toSQL.c
	gcc -O2 -o src/toSQL.o -std=gnu99 -I include -c src/toSQL.c 

src/dhtxx.o: src/dhtxx.c include/dhtxx.h
	gcc -O2 -o src/dhtxx.o -std=gnu99 -I include -c src/dhtxx.c

src/dhtquery.o: src/dhtxx.o src/dhtquery.c
	gcc -O2 -o src/dhtquery.o -std=gnu99 -I include -c src/dhtquery.c

dhtquery: src/dhtxx.o src/dhtquery.o
	gcc -O2 -o dhtquery src/dhtquery.o src/dhtxx.o -l bcm2835

clean:
	rm -rf src/*.o
	rm -rf lib/*.a
	rm -rf dhtquery
	rm -rf sethigh
	rm -rf toSQL

lib/libdhtxx.a: src/dhtxx.o include/dhtxx.h
	ar rcs lib/libdhtxx.a src/dhtxx.o

sethigh: src/sethigh.c
	gcc -O2 -o sethigh src/sethigh.c -l bcm2835

install: dhtquery-install sethigh-install lib-install

dhtquery-install: dhtquery
	cp dhtquery $(BIN_LOC)

sethigh-install: sethigh
	cp sethigh $(BIN_LOC)

lib-install: lib/libdhtxx.a
	cp lib/libdhtxx.a $(LIBRARY_LOC)
	cp include/dhtxx.h $(INCLUDE_LOC)

