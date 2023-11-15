#include <stdio.h>
#include <stdlib.h>

int main(){
	int figure_count = 8;
	char *arrayname = "BLOCK";
	char *filenames[1] = {"_15x15.data"};
	int width = 15;
	int height = 15;
	int data_len = width*height*3;
	FILE *fileH = fopen("blocks15x15.h", "w");
	if(fileH==0){printf("Dateifehler h\n");return 0;}
	fprintf(fileH, "#define %s_WIDTH %d\n", arrayname, width);
	fprintf(fileH, "#define %s_HEIGHT %d\n", arrayname, height);
	fprintf(fileH, "#define %s_COUNT %d\n", arrayname, figure_count);
	fprintf(fileH, "#define %s_LEN (%s_WIDTH*%s_HEIGHT*2+1)\n", \
						arrayname, arrayname, arrayname);
	fprintf(fileH, "static uint8_t %s[%s_COUNT][%s_LEN] ={\n", \
						arrayname, arrayname, arrayname);

	//16bit 565 pixelformat (3Ah =55h)
	for(int i=0; i<figure_count; i++){
		char filename[13];
		sprintf(filename, "b%d%s", i, filenames[0]);
		FILE *fileData = fopen(filename, "rb");
		if(fileData==0){printf("Dateifehler data\n");return 0;}
		unsigned char data[data_len];
		fread(data, 1, data_len, fileData);
		fprintf(fileH, "{0X2C, // %d\n", i);
		//read 3 bytes and write 2 bytes
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
