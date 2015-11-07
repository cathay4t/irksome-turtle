CC=gcc
EXEC_NAME=irksome

objects = irksome.o encrypt.o

irksome : $(objects)
	$(CC) -o $(EXEC_NAME) $(objects)

.PHONY : clean

clean :
	rm -f $(EXEC_NAME) $(objects)

