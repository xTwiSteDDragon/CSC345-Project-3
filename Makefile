all:
	gcc -o main main.c
	gcc -o main1 main1.c
	gcc -o main2 main2.c
	gcc -o main3 main3.c

clear:
	rm main main1 main2 main3