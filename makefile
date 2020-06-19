a.out : large_int_client.o	large_int_impl.o	
	gcc large_int_client.o large_int_impl.o

large_int_client.o: large_int_client.c large_int_header.h	
	gcc -c large_int_client.c

large_int_impl.o : large_int_impl.c large_int_header.h	
	gcc -c large_int_impl.c

