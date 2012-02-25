CC=gcc
#CFLAGS=-lpthread -lreadline -lhistory -lcurses -L/usr/lib/mysql/ -lmysqlclient -lz 
CFLAGS=-lpthread  -L/usr/lib/mysql/ -lmysqlclient -DDBG
all=main.o convert.o 
#all=main.o connect.o get_conf.o
main: $(all) main.h
	$(CC) -o $@ $^ $(CFLAGS)
%.o: %.c main.h
	$(CC) -c -o $@ $<
clean:
	rm -rf *.o main
