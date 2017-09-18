/* AES Key Schedule */
#include <stdio.h>
#include <stdlib.h>
#include "aes.h"

main(){
	B8 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
			0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	B16 keylength = 128;
	KeyBlockRound(keylength);
	KeyExpansion(key, w, Nk);
	getSboxCoordExample();
}

B32 SubWord(B32 word){
	B8 b[]={(int)((word >> 24) & 0xFF),(int)((word >> 16) & 0xFF),
		(int)((word >> 8) & 0XFF),(int)((word & 0XFF))};
        int i=0;
        while(i<4){
		b[i]= getSBoxValue(((b[i]>>4)*16)+(b[i]&0xF));
		i++;
        }
	return ( (b[0] << 24)+(b[1] << 16)+(b[2] << 8)+(b[3] ) );
}

B32 RotWord(B32 word){
	B8 b[]={(int)((word >> 24) & 0xFF),(int)((word >> 16) & 0xFF),
		(int)((word >> 8) & 0XFF),(int)((word & 0XFF));
	B32 rotada =((b[1] << 24)+(b[2] << 16)+(b[3] << 8)+(b[0]));
	return rotada;
}

void KeyExpansion(B8 key[4*Nk], B32 w[Nb*(Nr+1)], B16 Nk){
	B32 temp;
	B16 i = 0;
	while(i < Nk){
		// corrimiento izq 8, 16 o 24 bits para unirlos en 32 bits
		w[i] = ((key[4*i]) << 24) | ((key[4*i+1]) << 16) | ((key[4*i+2]) << 8) | ((key[4*i+3]));
		if(debug){
			printf("\t%x\n", key[4*i]<<24);
			printf("\t  %x\n", key[4*i+1]<<16);
			printf("\t    %x\n", key[4*i+2]<<8);
			printf("\t      %x\n", key[4*i+3]);
			printf("-------------------\n");
			printf(" w[%i] = %x\n\n", i,w[i]);
		}
		i = i + 1;
   	}
	int cota = Nb* (Nr+1);
	printf("iter\ttemp\t\tAfterRotW\tAfterSubW\tRcon[i/Nk]\tAXorRcon\tw[i-Nk] \tw[i]=temp^w[i-Nk]");
	printf("\n-----------------------------------------------------------");
	printf("-----------------------------------------------------------");
	while(i<cota){
		printf("\n%.2i",i);
		temp = w[i-1];
		printf("\t%.8x",temp);
		if(i % Nk == 0){
			temp = RotWord(temp);
			printf("\t%.8x",temp);
			temp = SubWord(temp);
			printf("\t%.8x",temp);
			printf("\t%.8x",Rcon[i/Nk]);
			temp = temp ^ Rcon[i/Nk];
			printf("\t%.8x",temp);
		}else if(Nk>6 && i % Nk == 4){
			temp = SubWord(temp);
			printf("\t\t\t%.8x",temp);
		}else{ printf("\t\t\t\t\t\t\t\t"); }
		w[i] = w[i-Nk] ^ temp;
		printf("\t%.8x",w[i-Nk]);
		printf("\t%.8x",w[i]);
		i++;
	}
	printf("\n");
}


void KeyBlockRound(B16 keylength){
	switch(keylength){
		case 128: Nk = 4; Nr = 10; break;
		case 192: Nk = 6; Nr = 12; break;
		case 256: Nk = 8; Nr = 14; break;
		default: printf("Error-longitud de llave incorrecto\n");
	}
}

void getSboxCoordExample(){
	B32 x;
	B32 y;
	B32 temp = 0xcf4f3c09;
	x = temp & 0xf0000000;
	y = (temp & 0x0f000000) >> 24;
}
