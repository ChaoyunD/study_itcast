SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
TARGET=$(patsubst %.c, %, $(SRC))
#$@:目标
#$<:第一个依赖
#$^:全部依赖
CFLAGS= -I ./
all: $(TARGET)
	
%:%.c
	gcc -o $@ $< -g $(CFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
distclean:clean
	rm -f a.out *.~
#伪目标：需要把那些不对应具体文件的目标，声明为伪目标
.PHONY:clean 
