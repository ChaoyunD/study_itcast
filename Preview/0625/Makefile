SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
TARGET=$(patsubst %.c,%,$(SRC))
CLAFGS=-I ./

all: $(TARGET)

%:%.c
	gcc -o $@ $< $(CLAFGS) -g
clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
distclean:
	rm -f a.out *.~
.PHONY:clean
