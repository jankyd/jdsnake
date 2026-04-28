snake: snake.cpp
	g++ snake.cpp -lncurses -o test
.PHONY: clean
clean:
	rm -f *.o
	rm -f test
