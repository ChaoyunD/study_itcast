SRC=$(wildcard ./*.c)
TARGET=$(patsubst %.c,%,$(SRC))
CFLAGS=-g -Wall
all:$(TARGET)  #template

#template:template.c
#	gcc -o $@ $<
%:%.c
	gcc -o $@ $< $(CFLAGS)
clean:
	rm -rf $(TARGET)
	rm -rf a.out *.~

.PHONY:clean all
