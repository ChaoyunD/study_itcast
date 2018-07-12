TARGET=main
SRC=$(wildcard ./src/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
CLFAGS=-c -I ./include
$(TARGET):$(OBJ)
	gcc -o $@ $^
%.o:%.c
	gcc -o $@ $< $(CLFAGS)
clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
distclean:
	rm a.out *.~
.PHONY:clean

