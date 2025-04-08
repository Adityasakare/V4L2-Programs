CC=g++
CFLAGS=-Wall `pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`

program4: Program4.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f program4
