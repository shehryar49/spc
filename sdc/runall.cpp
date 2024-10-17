#include <stdio.h>
#include <stdlib.h>
#include <string>


using namespace std;
const char* corpora[] = {
    /*"big.txt", //from internet
    "bible.txt", // from canterbury git repo
    "DATASET.csv", // from Kaggle(spotify reviews)
    "suhaib.txt",  // from Kaggle
    "world192.txt", // from canterbury corpus
    "fields.c", // from canterbury corpus
    */"sqlite3.c", //sqlite3 source code
};
const char* dicts[] = {
    "20k-google.txt",
    "google-10k-usa.txt",
    "ours1.txt",
    "cdict.txt"
};

#ifndef _WIN32
    #define PATH_DELIM "\\"
#else
    #define PATH_DELIM "/"
#endif

size_t getfilesize(const std::string& path)
{
    FILE* fp = fopen(path.c_str(),"rb");
    if(!fp)
    {
        puts("error opening file!");
        exit(1);
    }
    fseek(fp,0,SEEK_END);
    size_t bytes = ftell(fp);
    fclose(fp);
    return bytes;
}
int main()
{
    std::string file_path;
    std::string dict_path;
    std::string cmd;
    for(size_t i = 0; i< sizeof(corpora) / sizeof(const char*); i++)
    {
        file_path = (string)"testing-corpus" PATH_DELIM + corpora[i];
        size_t original_size = getfilesize(file_path); 
        printf("%s %lf KB\n",corpora[i],(double)original_size / 1024);
        for(size_t j = 0; j<sizeof(dicts)/sizeof(const char*); j++)
        {
            dict_path = (string)"dicts" PATH_DELIM + dicts[j];
            #ifdef _WIN32
                cmd = "sdc.exe "+dict_path+" "+file_path;
            #else
                cmd = "./sdc "+dict_path+" "+file_path;
            #endif
            system(cmd.c_str());

            size_t new_size = getfilesize("compressed.bin");
            double perc_compressed = double(original_size - new_size) / original_size;
            printf("  %s  %lf\n",dicts[j],perc_compressed);
        }
    }
}