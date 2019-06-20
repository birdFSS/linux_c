OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXE:=test
CC:=gcc
$(EXE):$(OBJS)
	$(CC) -o $@ $^
clean:
	rm -rf $(EXE) $(OBJS)
