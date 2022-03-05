all: editor
build: editor.c
	gcc -g3 -o editor editor.c
clean:
	rm editor
