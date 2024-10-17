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
    if(!fin)
    {
        puts("error opening file");
        return;
    }
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

int main(int argc,const char* argv[])
{
    if(argc!=2)
    {
        puts("usage: ./sort-dict file");
        return 1;
    }
    read_and_sort_dict(argv[1]);
}
