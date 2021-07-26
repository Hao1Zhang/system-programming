all: part1 part2 part3

part1:smsh.h smsh2.c execute.c splitline.c
	gcc -o smsh2 smsh2.c splitline.c execute.c
part2:smsh.h smsh3.c execute.c splitline.c
		gcc -o smsh3 smsh3.c splitline.c execute.c
part3:smsh.h smsh4.c execute.c splitline.c
	gcc -o smsh4 smsh4.c splitline.c execute.c
clean:
	rm smsh2 smsh3 smsh4