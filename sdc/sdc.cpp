#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;
bool islessthan(const std::string& s1,const std::string& s2)
{
    size_t i = 0;
    size_t j = 0;
    size_t len1 = s1.length();
    size_t len2 = s2.length();
    
    while(i < len1 && j < len2)
    {
        char ch1 = tolower(s1[i]);
        char ch2 = tolower(s2[j]);
        if(ch1 < ch2)
            return true;
        else if(ch1 > ch2)
            return false;  
        i++;
        j++;
    }
    if(len1 < len2)
        return true;
    return false;
}
void bubblesort(vector<string>& words)
{
    size_t n = words.size();
    std::string tmp;
    for(size_t i=1;i<=n-1;i++)
    {
        for(size_t j = 0;j < n - i;j++)
        {
            if(!islessthan(words[j],words[j+1]))
            {
                //swap words[j] and words[j+1]
                tmp = words[j];
                words[j] = words[j+1];
                words[j+1] = tmp;
            }
        }
    }
}
void read_and_sort_dict(const char* filename)
{
    ifstream fin(filename,ios::in);
    std::string word;
    vector<string> words;
    while(fin >> word)
    {
        if(word.length() >= 3)
            words.push_back(word);
    }
    fin.close();

    // sort the dictionary
    bubblesort(words);

    for(auto word: words)
        cout << word << endl;
}
void read_dict(std::vector<string>& dict,const std::string& filename)
{
    ifstream fin(filename);
    std::string word;
    dict.clear();
    while(fin >> word)
    {
        dict.push_back(word);
    }
}

int binary_search(const std::string& word, const std::vector<string>& words)
{
    int l = 0;
    int h = words.size() - 1;

    while(l<=h)
    {
        int mid = (l+h)/2;
        if(words[mid] == word)
            return mid;
        if(islessthan(word,words[mid]))
            h = mid-1;
        else
            l = mid+1;
    }
    return -1;
}

//checks if character is delimiter
bool is_word_delimiter(char ch)
{
    return (ch == ' ' || ch == ',' || ch == '.' || ch == '!' || ch == ';' || ch == ':' || ch == '-' || ch == '/' || ch == '\n' || ch =='\r' || ch=='\t' || ch == '#' || ch == '*' || ch == '(');
}
void write_word(const string& str,const vector<string>& dict,FILE* fp)
{
    short word_num = (str.length() <= 2) ? -1 : binary_search(str,dict);
    
    //if(word_num)
    //    printf("%s %d\n",str.c_str(),word_num);
    if(word_num == -1) // word not found in dictionary
        fputs(str.c_str(),fp);
    else
    {
        // assumption:
        //word_num is always in range 0 - (2^15 - 1)
        //most significant bit is always set to indicate dictionary native words
        unsigned char byte2 = word_num & 0x00ff;
        word_num >>= 8;
        unsigned char byte1 = (word_num & 0x00ff); 
        // write as big endian
        byte1 |= 0x80;
        fputc(byte1,fp);
        fputc(byte2,fp);
    }
}

void compress_file(const char* filename,const vector<string>& dict)
{
    std::string word;
    FILE* fin = fopen(filename,"rb");
    std::string buffer;
    buffer.resize(256);
    FILE* fp = fopen("compressed.bin","wb");
    int len;
    while((len = fread(&buffer[0],sizeof(char),256,fin)))
    {
        //Compress each block
        int i = 0;
        int start = 0;        
        while(start < len && is_word_delimiter(buffer[start]))
        {
            fputc(buffer[start],fp);
            start++;
        }

        for(i = start; i< len; i++)
        {
            char ch = buffer[i];
            if(is_word_delimiter(ch))
            {
                int end = i;
                int word_len = (end - start);
                if(word_len < 0);
                else
                {
                    word = buffer.substr(start,word_len);
                    write_word(word,dict,fp);
                }
                
                while(i < len && is_word_delimiter(buffer[i]))
                {
                    fputc(buffer[i],fp);
                    i++;
                }
                start = i;
            }
        }
        
        int word_len = (i - start);
        if(word_len <= 0) //ignore
        ;
        else
        {
            word = buffer.substr(start,word_len);
            write_word(word,dict,fp);
        }
    }
    fclose(fp);
    fclose(fin);
}
void decompress_file(const char* filename,const vector<string>& dict)
{
    FILE* fp = fopen(filename,"rb");
    if(!fp)
    {
        printf("error opening file!\n");
        return;
    }
    char buffer[256];
    size_t read;
    FILE* fout = fopen("decompressed.txt","wb");
    if(!fout)
    {
	    printf("error opening file!\n");
	    return;
    }
    while((read = fread(buffer,sizeof(char),256,fp)))
    {
        for(size_t i = 0; i < read; i++)
        {
            char ch = buffer[i];
            if(ch & 0x80)
            {
                ch &= 0x7f;
                // 0000 0000 0XXX XXXX
                short word_num = ch;
                word_num <<= 8;
                // read next byte, there is always one
                unsigned char continuation; 
                if(i+1 >= read)
                {
                    continuation = fgetc(fp);
                    //puts("Invalid file");
                    //exit(1);
                }
                else
                    continuation = buffer[i+1];
                word_num |= continuation;
		if(word_num < 0 || (int)word_num >= (int)dict.size())
		{
			puts("decompressing invalid file!");
			exit(1);
		}
		const char* word = dict[word_num].c_str();
                fputs(word,fout);
                i+=1;
            }
            else
                fputc(ch,fout);
        }
    }
    fclose(fp);
    fclose(fout);
}

int main(int argc,const char* argv[])
{
    //IMPORTANT: Limit the size of dictionaries to 32768 words for now i.e 2^15
    //read_and_sort_dict("google-10k-usa.txt");
    if(argc!=3)
    {
        puts("usage: ./sdc sorted-dict textfile");
        return 1;
    }
    vector<string> dict;
    read_dict(dict,argv[1]);
   // puts("--------------COMPRESSING-------------------");
    compress_file(argv[2],dict);
    //puts("--------------DECOMPRESSING-----------------");
    decompress_file("compressed.bin",dict);
}
