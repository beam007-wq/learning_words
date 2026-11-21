main: main.o setting.o review.o learn.o set_dictionary.o common.o
	zig cc -g build/main.o build/setting.o build/review.o build/learn.o build/set_dictionary.o build/common.o -o ~/Documents/learning_words/bin/main -lncursesw -lpanelw

main.o: src/main.c includes/setting.h includes/common.h includes/review.h includes/learn.h includes/set_dictionary.h
	zig cc -c src/main.c -o build/main.o

setting.o: src/setting.c includes/setting.h includes/common.h
	zig cc -c src/setting.c -o build/setting.o

review.o: src/review.c includes/review.h includes/common.h
	zig cc -c src/review.c -o build/review.o

learn.o: src/learn.c includes/learn.h includes/common.h
	zig cc -c src/learn.c -o build/learn.o

set_dictionary.o: src/set_dictionary.c includes/set_dictionary.h includes/common.h
	zig cc -c src/set_dictionary.c -o build/set_dictionary.o

common.o: src/common.c includes/common.h
	zig cc -c src/common.c -o build/common.o
