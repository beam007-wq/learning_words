main: main.o setting.o review.o learn.o set_dictionary.o common.o
	zig cc -g ../build/main.o ../build/setting.o ../build/review.o ../build/learn.o ../build/set_dictionary.o ../build/common.o -o ~/Documents/learning_words/bin/main -lncursesw -lpanelw

main.o: main.c setting.h common.h review.h learn.h set_dictionary.h
	zig cc -c main.c -o ../build/main.o

setting.o: setting.c setting.h common.h
	zig cc -c setting.c -o ../build/setting.o

review.o: review.c review.h common.h
	zig cc -c review.c -o ../build/review.o

learn.o: learn.c learn.h common.h
	zig cc -c learn.c -o ../build/learn.o

set_dictionary.o: set_dictionary.c set_dictionary.h common.h
	zig cc -c set_dictionary.c -o ../build/set_dictionary.o

common.o: common.c common.h
	zig cc -c common.c -o ../build/common.o
