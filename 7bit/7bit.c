#include <stdio.h>
#include <stdlib.h>

int main(int argc,const char* argv[])
{
	if(argc != 2)
	{
		puts("usage: 7bit filename");
		return 1;
	}
	
	FILE* fp = fopen(argv[1],"rb");
	char buffer[257];
	int read;
	char btw = 0;
	char btw_mask = 0
	while((read = fread(buffer,1,256,fp)))
	{
		buffer[read] = 0;
		char effective_bits_mask = 0x7f;
		
		int i = 0;
		for(i = 0; i < read;i++)
		{
			char effective_byte = (buffer[i] & mask);
			effective_byte <<= 1;
			btw |= effective_byte;
			printf("%d\n",(buffer[i] & mask));
		}
		//printf("%s",buffer);
	}
	//puts("");
	return 0;
}
