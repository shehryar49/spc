#include <stdio.h>
#include <stdlib.h>

int main(int argc,const char* argv[])
{
    if(argc != 2)
    {
        puts("usage: ./make_ascii filename");
        return 1;
    }
    unsigned char mask = 0x80;
    FILE* fp = fopen(argv[1],"rb");
    if(!fp)
    {
        perror("fopen");
        return 1;
    }
    char ch = 0;
    while((ch = fgetc(fp)) != EOF)
    {
        if(!(ch & mask))
            fputc(ch,stdout);
    }
    fclose(fp);
}
