link=`mysql_config --cflags --libs`
object=main.o main_assistant.o wrap_function.o database.o

server:$(object)
	gcc -o server -pthread $(object) $(link)

main.o:main.c common.h 
	gcc -c main.c

main_assistant.o:main_assistant.c wrap_function.o common.h deal_with_message.h database.h
	gcc -c main_assistant.c
wrap_function.o:wrap_function.c common.h
	gcc -c wrap_function.c 
database.o:database.c database.h
	gcc -c database.c

clean:
	rm *.o
