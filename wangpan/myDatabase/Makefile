CFFLAG:=-Wall -I /usr/include/mysql/ -lmysqlclient
con_mysql:func_mysql.c
	gcc $^ -o $@ $(CFFLAG)
clean:
	rm con_mysql
