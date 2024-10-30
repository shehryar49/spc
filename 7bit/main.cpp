#include <stdio.h>
#include <stdlib.h>

void sevenbit_encode(const char* filename)
{
    FILE* fp = fopen(filename,"rb");

    if(!fp)
    {
	perror("fopen");
	exit(1);
    }

    char rd_buffer[8];
    char wt_buffer[7];
    FILE* fout = fopen("compressed.bin","wb");
    size_t read = 0;
    while((read = fread(rd_buffer,sizeof(char),8,fp)) == 8)
    {
	//The following instructions also have a general formula
	//not using a nested loop right now
	wt_buffer[0] = (rd_buffer[0] << 1) | (rd_buffer[1] & 0x40 >> 6);
	wt_buffer[1] = (rd_buffer[1] << 2) | (rd_buffer[2] & 0x60 >> 5);
	wt_buffer[2] = (rd_buffer[2] << 3) | (rd_buffer[3] & 0x70 >> 4);
	wt_buffer[3] = (rd_buffer[3] << 4) | (rd_buffer[4] & 0x78 >> 3);
	wt_buffer[4] = (rd_buffer[4] << 5) | (rd_buffer[5] & 0x7c >> 2);
	wt_buffer[5] = (rd_buffer[5] << 6) | (rd_buffer[6] & 0x7e >> 1);
	wt_buffer[6] = (rd_buffer[6] << 7) | (rd_buffer[7] & 0x7f);
	fwrite(wt_buffer,sizeof(char),7,fout);
    }
    fclose(fp);
    fclose(fout);
}
int main()
{
    sevenbit_encode("big.txt");
}
