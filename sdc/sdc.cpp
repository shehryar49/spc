#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;


//GPT can't write this
// Humans are better
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
        if(word.length() >= 4)
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

//checks if character is dilimiter
bool is_word_delimiter(char ch)
{
    return (ch == ' ' || ch == ',' || ch == '.' || ch == '!' || ch == ';' || ch == ':');
}

void compress_file(const char* filename)
{
    vector<string> dict;
    read_dict(dict,"sorted-dict.txt");
    std::string data = "   American      anniversary celebrated yesterday           ";
    std::stringstream s(data);
    std::string word;
    FILE* fp = fopen("compressed.bin","wb");
    size_t i = 0;
    size_t len = data.length();
    int start = 0;
    
    while(start < len && is_word_delimiter(data[start]))
        start++;

    for(i = start; i< len; i++)
    {
        char ch = data[i];
        if(is_word_delimiter(ch))
        {
            int end = i;
            // start to end-1
            // end-1-start+1
            //cout << "start = "<<start<<endl;
            //cout << "end = " << end << endl;
            int word_len = (end - start);
            if(word_len < 0);
            else
            {
                word = data.substr(start,word_len);
                cout <<"|"<< word << "|" <<endl;
            }
            
            while(i < len && is_word_delimiter(data[i]))
                i++;
            start = i;
            //cout << "next start = "<<start << endl;
            //exit(0);
        }
    }
    i--; //since on the last iteration, while exiting the for loop, i was incremented
    cout << "Assumption 1: "<< ( i == len) << endl;
    int word_len = (i - start);
    cout << word_len << endl;
    cout << "i = "<< i<<endl;
    cout << "start = " << start << endl;
    if(word_len <= 0) //ignore
    ;
    else
    {
        word = data.substr(start,word_len);
        cout <<"|"<< word << "|" <<endl;
    }
    /*while(s >> word)
    {
        short word_num = binary_search(word,dict);
        cout << word_num << endl;  
        if(word_num == -1) // word not found in dictionary
        {
            word += " ";
            fwrite(word.c_str(),sizeof(char),word.length(),fp);
        }
        else
        {
            // assumption:
            //word_num is always in range 0 - (2^15 - 1)
            //most significant bit is always set to indicate dictionary native words
            short mask = 0x8000;
            word_num |= mask; //set msb to 1
            fwrite(&word_num,sizeof(short),1,fp);
        }

    }*/
    fclose(fp);
}
int main()
{
    compress_file("nofile");
    //read_and_sort_dict("dict.txt");
}