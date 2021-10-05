OBJS	= main.o affichage.o fichiers.o fir.o iir.o integration.o lecture.o mesure.o
SOURCE	= main.c affichage.c fichiers.c fir.c iir.c integration.c lecture.c mesure.c
HEADER	=
OUT	= oxymetre.out
CC	 = gcc
FLAGS	 = -g -c
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99 -lcunit

affichage.o: affichage.c
	$(CC) $(FLAGS) affichage.c -std=c99 -lcunit

fichiers.o: fichiers.c
	$(CC) $(FLAGS) fichiers.c -std=c99 -lcunit

fir.o: fir.c
	$(CC) $(FLAGS) fir.c -std=c99 -lcunit

iir.o: iir.c
	$(CC) $(FLAGS) iir.c -std=c99 -lcunit

integration.o: integration.c
	$(CC) $(FLAGS) integration.c -std=c99 -lcunit

lecture.o: lecture.c
	$(CC) $(FLAGS) lecture.c -std=c99 -lcunit

mesure.o: mesure.c
	$(CC) $(FLAGS) mesure.c -std=c99 -lcunit


clean:
	rm -f $(OBJS) $(OUT)