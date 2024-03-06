#STEFAN Alexandru - 315CB

build:
	gcc -o quadtree -g -std=gnu11 main.c functions.c
run: build
	./quadtree
clean:
	rm -r quadtree