LDIR=./lib
11:
	gcc sham.c -o sham -Llib -linfodef -lrandombytes -lm
22:
	gcc rsa.c -o rsa -Llib -linfodef -lrandombytes -lm
33:
	gcc l-gamal.c -o lgamal -Llib -linfodef -lrandombytes -lm
