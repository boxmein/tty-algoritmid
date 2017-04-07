FILES=algo2.c
projekt: algo2

algo2: $(FILES:.c=.o)
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<
