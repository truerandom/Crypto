#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>

// Instalar en Debian libssl-dev
// gcc des_openssl.c -o des_openssl.o -lcrypto
// uso: program pass text
// Con informacion de: http://blog.fpmurphy.com/2010/04/openssl-des-api.html#sthash.lvb2s67j.SoASWZFN.dpbs
#define BUFSIZE 256

void format(){
	printf("-----------------------------------------------------------------------------------------------------\n");
}

void main(int argc, unsigned char* argv[]){
	unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
	unsigned char *e = out;
	// Crea una variable llamada key, que es de tipo DES_cblock
	DES_cblock key;
	// Copia 8 bytes de la contraseña dada por el usuario en key
	memcpy( key, argv[1],8);
	// establece la paridad de la llave en impar
	DES_set_odd_parity( &key );
	// Crea una variable llamada keysched, que es de tipo DES_key_schedule
	DES_key_schedule keysched;
	// Copia tantos 0 en las variables como sea su tamanio
	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(back, 0, sizeof(back));

	// Revisa la paridad de la llave key, y hace la planificación de la llave
	DES_set_key_checked( &key, &keysched );
	/* 8 bytes de texto en claro */
	strcpy(in, argv[2]);
	format();
        int longitud = strlen(argv[2]);
	if(longitud%64!=0){
		printf("EL MENSAJE NO ES UN BLOQUE IDEAL, CIFRANDO DE CUALQUIER FORMA\n");
	}
	printf("Texto en claro: %s\n", in);
	printf("Longitud msg: %u\n",longitud);
	format();
	// Cifrado
	int i;
	for(i=0;i<longitud; i+=8){
		DES_ecb_encrypt(&(in[i]),&(out[i]),&keysched,DES_ENCRYPT);
	}
	format();
	printf("Criptograma:\n");
	while (*e) printf("%02x", *e++);
	printf("\n");
	format();
	// Descifrado
	for(i=0;i<longitud; i+=8){
		DES_ecb_encrypt(&(out[i]),&(back[i]),&keysched,DES_DECRYPT);
	}
	format();
	printf("Texto descifrado: %s\n", back);
	format();
}
