all: build
build: 
	gcc -o tema1 -g -Wall functiiLG.c functii-tabHash.c SDA-tema1.c
run:
	./tema1
clean:
	rm -rf tema1