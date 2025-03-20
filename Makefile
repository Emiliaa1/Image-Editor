FLAG = -Wall -Wextra -I. -g
CODE = image_editor
INPUT_PATH = /home/emi/Downloads/PCLP1-check-tema3-20241209/tasks/image_editor/tests/00-image_editor/00-image_editor.in

build: $(CODE)

image_editor: main.c functions.c
	gcc $(FLAG) main.c functions.c -o image_editor -lm

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  ./$(CODE) < $(INPUT_PATH)

gdb: build
	gdb ./$(CODE) < $(INPUT_PATH)

run: build
	./$(CODE) < $(INPUT_PATH)

clean:
	rm -f $(CODE) *.o