objects = game.o main.o node.o
guess: $(objects)
	cc -o guess game.o main.o node.o
$(objects): guess.h

.PHONY: clean
clean:
	-rm guess $(objects)
run: guess
	./guess
