#include <stdio.h>
#include <stdlib.h>

int main(){
	int data_len = 25*25*3;
	FILE *fileH = fopen("rang25x25.h", "w");
	if(fileH==0){printf("Dateifehler h\n");return 0;}
	fprintf(fileH, "#define RANG_LEN (25*25*2+1)\n");
	fprintf(fileH, "static uint8_t RANG[5][RANG_LEN] ={\n");

	//5r6g5b pixelformat (3Ah =03h)
	for(int i=1; i<6; i++){
		char filename[14];
		sprintf(filename, "rang%d.data", i);
		FILE *fileData = fopen(filename, "rb");
		if(fileData==0){printf("Dateifehler d\n");return 0;}
		unsigned char data[data_len];
		fread(data, 1, data_len, fileData);
		fprintf(fileH, "{0X2C, // %d\n", i);
		//read 3 bytes and write 1 word
		for(int m=0; m<data_len-2; m=m+3){
			if(m%4==0&&m>0)fprintf(fileH, " ");
			if(m%24==0&&m>0)fprintf(fileH, "\n");
			//word 0x0000;//RRRR RGGG GGGB BBBB
			u_int16_t blue = (data[m+2] >>3) & 0x1F;
			u_int16_t green = ((data[m+1] >>2) & 0x3F)<<5;
			u_int16_t red = ((data[m] >>3) & 0x1F)<<11;
			u_int16_t sum = red | green | blue;
			fprintf(fileH, "0x%02X,0x%02X,", (sum&0xFF00)>>8, (sum&0x00FF));
		}
		fprintf(fileH, "},\n");
		fclose(fileData);
	}
	fprintf(fileH, "};\n");
	fclose(fileH);
}
