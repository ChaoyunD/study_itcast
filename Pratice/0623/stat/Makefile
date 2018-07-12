# By Panda Guo at  2018-06-21 14:29:55 created

#  编译同名的.c 到

src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src)) 

CC = gcc

LDFLAGS = -Wall -g


all: $(target)



%: %.c
	$(CC) -o $@ $< $(LDFLAGS)

# .c.o:
# 	$(CC) -o $@ $< $(LDFLAGS)



clean:
	-rm -rf $(target)

distclean: clean
	@-rm -rf a.out *~

.PHONY: clean distclean  all
