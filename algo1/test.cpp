#include <climits>
#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <gmp.h>
#include <stdint.h>

char arr[5] = {0,0,0,0,0};
int count = 1;
void permute(char* str,int i,int len)
{
    if(i == len)
        printf("%d. %s\n",count++,arr);
    for(int k=0;k < len;k++)
    {
        if(str[k] != 0)
        {
            arr[i] = str[k];
            str[k] = 0;
            permute(str,i+1,len);
            str[k] = arr[i];
        }
    }
}
size_t fact(size_t x)
{
    size_t result = 1;
    while(x != 0)
    {
        result *= x;
        x-=1;
    }
    return result;
}
char tmp[] = "abcdefghi";
void remove_char(int idx,int len)
{
    for(int i=idx;i < len-1 ;i++)
    {
        tmp[i] = tmp[i+1];
    }
    tmp[len-1] = 0;
}

void decompress(int x,int len)
{
    if(len == 0)
    {
        puts("");
        return;
    }
   // puts(tmp);
    int starts[len];
    int f = fact(len-1);
    for(int i=0;i<len;i++)
    {
        if(i == 0)
            starts[i] = 1;
        else
        {
            starts[i] = starts[i-1] + f;
        }
        //printf("%d\n",starts[i]);
    }
    for(int i = 0;i < len;i++)
    {
        if(starts[i] == x)
        {
            printf("%c",tmp[i]);
            remove_char(i,len);
            decompress(1,len-1); // my magic formula (i)
            return;
        }
        else if(x >= starts[i] && x<= starts[i]+f-1)
        {
            int implicit_num = x - starts[i] + 1; // my magic formula (ii)
            printf("%c",tmp[i]);
            remove_char(i,len);
            decompress(implicit_num, len-1);
            return;
        }
    }
}
struct spc_pair
{
    char ch;
    int count;
    spc_pair(char a,int b) : ch(a), count(b)
    {}
    bool operator==(const spc_pair& b)
    {
        return ch == b.ch;
    }
};
struct spc_binary_pair
{
    unsigned char byte;
    size_t count;
    spc_binary_pair(unsigned char a,size_t b)
    {
        byte = a;
        count = b;
    }
    bool operator==(const spc_binary_pair& b)
    {
        return byte == b.byte;
    }
};

size_t counts[26];
size_t compress(const char* str)
{
  size_t len = strlen(str);

  //Generate letter counts

  memset(counts,0,sizeof(size_t)*26);

  
  for(size_t i = 0;i < len;i++)
    counts[str[i] - 'a']++;
  
  std::vector<spc_pair> sorted_charset;
  for(size_t i = 0; i < 26;i++)
  {
    if(counts[i] != 0)
        sorted_charset.push_back(spc_pair('a'+i,counts[i]));
  }
 // puts("Determined count");
  //////////////
  size_t count = 0;
  size_t f = fact(len - 1);
  for(size_t i=0;i < len-1; i++)
  {
	char ch = str[i];
    //find first non zero and store its' idx in curr
    size_t permutations_skipped = 0;
    size_t idx;
    for(size_t j = 0; j < sorted_charset.size(); j++)
    {
        if(sorted_charset[j].ch == ch)
        {idx = j; break;}
        permutations_skipped += sorted_charset[j].count * f;
    }
    //int permutations_skipped = (curr - 1) * fact(len - (i+1));
    count += permutations_skipped;
    //decrement charset[curr]
    sorted_charset[idx].count -= 1;
    if(sorted_charset[idx].count == 0)
        sorted_charset.erase(sorted_charset.begin()+idx);
    f /= len - (i+1);
    
  }
  return count + 1;
}
void decompress1(size_t magic_num)
{
    size_t len = 0;
    std::vector<spc_pair> sorted_charset;
    for(size_t i = 0; i < 26;i++)
    {
        if(counts[i] != 0)    
            sorted_charset.push_back(spc_pair('a'+i,counts[i]));
        len += counts[i];
    }

    size_t f = fact(len-1);

    while(len != 0)
    {
        size_t ntile = (magic_num - 1) / f + 1;
        //update magic_num
        if((magic_num - 1) % f == 0)
            magic_num = 1;
        else
            magic_num = magic_num - (f * (ntile - 1) + 1) + 1;
        size_t tmp = 0;
        for(size_t i = 0; i<sorted_charset.size();i++)
        {
            tmp += sorted_charset[i].count;
            if(tmp >= ntile)
            {
                fputc(sorted_charset[i].ch,stdout);
                sorted_charset[i].count -=1;
                if(sorted_charset[i].count == 0)
                    sorted_charset.erase(sorted_charset.begin() + i);
                break;
            }
        }
        if(len != 1)
            f /= (len - 1); // f is now the factorial of len-1
        len--;
    }
    puts("");
}
///////////////
void generate_seq_bignum(mpz_t x,mpz_t factorial)
{
    std::string seq;
    mpz_t high;
    mpz_init(high);
    mpz_set(high,factorial);

    mpz_t low;
    mpz_init(low);
    mpz_set_ui(low,1);

    mpz_t mid;
    mpz_init(mid);
    mpz_set_ui(mid,0);

    int i = 0;
    uint8_t byte = 0;
    uint8_t mask = 0x80;
    FILE* fp = fopen("compressed.bin","wb");
    while(mpz_cmp(mid,x)!=0)
    {
        mpz_sub(mid,high,low);
        mpz_div_ui(mid,mid,2);
        mpz_add(mid,mid,low);
        if(mpz_cmp(x,mid) <= 0)
        {
            mpz_set(high,mid);
            fputc('0',stdout);
            //seq += '0';
            //byte has all bits zero alread
        }
        else 
        {
            mpz_add_ui(mid,mid,1);
            mpz_set(low,mid);
            //seq +='1';
            fputc('1',stdout);
            byte |= mask;
        }    
        i++;
        if(i == 8)
        {
            fputc(byte,fp);
            byte = 0;
            mask = 0x80;
            i = 0;
        }
        else
            mask >>= 1;
    }
   // printf("\ni = %d\n",i);
    fputc(byte,fp);
    fputc((char)i,fp);
    fclose(fp);
    puts("");
}
mpz_t magic_num;
void get_bignum_from_seq(const char* filename,mpz_t n)
{
    FILE* fp = fopen(filename,"rb");
    fseek(fp,0,SEEK_END);
    size_t total = ftell(fp);
    rewind(fp);
    fseek(fp,total-1,SEEK_SET);
    uint8_t last_byte = fgetc(fp);
    rewind(fp);
    mpz_t low;
    mpz_init(low);
    mpz_set_ui(low,1);

    mpz_t high;
    mpz_init(high);
    mpz_set(high,n);

    mpz_t mid;
    mpz_init(mid);
    mpz_set_ui(mid,0);
    
    int i = 0;
    uint8_t byte;
    uint8_t mask = 0x80;
    char ch;
    for(size_t k=1;k<=total-1;k++)
    {
        byte = fgetc(fp);
        mask = 0x80;
        size_t bits_to_use = (k == total-1) ? last_byte : 8;
        for(size_t i = 0;i<bits_to_use;i++)
        {
            int a = (byte & mask) != 0;
            printf("%d",a);
            mpz_sub(mid,high,low);
            mpz_div_ui(mid,mid,2);
            mpz_add(mid,mid,low);
            if((byte & mask) == 0)
                mpz_set(high,mid);
            else
            {
                mpz_add_ui(mid,mid,1);
                mpz_set(low,mid);
            }
            mask >>= 1;
        }
    }
    printf("\nnum = ");
    mpz_out_str(stdout,10,mid);
    puts("");
    fclose(fp);
    mpz_set(magic_num,mid);
}
////////////
mpz_t factorial_result;
void bignum_factorial(mpz_t x)
{
    mpz_set_ui(factorial_result,1);
    while(mpz_cmp_ui(x, 0) != 0)
    {
        mpz_mul(factorial_result,factorial_result,x);
        mpz_sub_ui(x,x,1);
    }
}
void write_magicnum(const char* filename)
{
    FILE* fp = fopen(filename,"wb");
    
    //Convert magic number to base256
    mpz_t r;
    mpz_init(r);
    const int chosen_base = 256; // 2^9 * 2^11 * 2^10 = 2^30
    mpz_t base;
    mpz_init(base);
    mpz_set_ui(base,chosen_base);

    size_t bytes = 0;
    while(mpz_cmp_ui(magic_num,0) != 0)
    {
        mpz_tdiv_r(r,magic_num,base);
        uint8_t m = mpz_get_ui(r);
        //printf("%d\n",m);
        
        fputc(m,fp);
        bytes++;
        mpz_div_ui(magic_num,magic_num,chosen_base);
    }
    fclose(fp);
    //printf("bytes taken = %zu\n",bytes);
}
void minify(mpz_t a,mpz_t b)    //tries to minify the magic number
{
    mpz_t x;
    mpz_init(x);
    mpz_set(x,a);
    size_t twos;
    size_t threes;
    size_t fives;
    
    mpz_t r;
    mpz_init(r);

    mpz_t d;
    mpz_init(d);
    mpz_set_ui(d,2);

    while(mpz_cmp_ui(d,1000000)!=0)
    {
        /*printf("new  = ");
        mpz_out_str(stdout,10,x);
        puts("");*/
        mpz_mod(r,x,d);
        if(mpz_cmp_ui(r,0) == 0)
        {
            mpz_div(x,x,d);
        }
        mpz_add_ui(d,d,1);
    }
//    printf("2s = %zu, 3s = %zu, 5s = %zu\n",twos,threes,fives);
    printf("new  = ");
    mpz_out_str(stdout,10,x);
    puts("");
    return;
  //  mpz_t d;
   // mpz_init(d);
   // mpz_set_ui(d,100000);

    mpz_mul_ui(d,d,100000);
    mpz_mul_ui(d,d,100000);
    mpz_mul_ui(d,d,100000);
    mpz_mul_ui(d,d,100000);
    
    mpz_t fact;
    mpz_init(fact);
    mpz_set(fact,b);
    mpz_div(fact,fact,d);


    mpz_t tmp;
    mpz_init(tmp);
    mpz_div(tmp,x,fact); // x/(n!/2)

    mpz_t remainder;
    mpz_init(remainder);

    mpz_tdiv_r(remainder,x,fact);

    printf("new  = ");
    mpz_out_str(stdout,10,tmp);
    puts("");

    printf("remainder  = ");
    mpz_out_str(stdout,10,remainder);
    puts("");
}

size_t binary_counts[256];

void binary_compress(const char* filename)
{
    FILE* fp = fopen(filename,"rb");
    if(!fp)
    {
        perror("fopen");
        return;
    }
    unsigned char buffer[2048];
    size_t read = 0;
    size_t len = 0; // total bytes
    while((read = fread(buffer,sizeof(char),2048,fp)))
    {
        for(size_t i = 0; i < read; i++)
        {
            binary_counts[buffer[i]]++;
        }
        len+=read;
    }
    rewind(fp);

    //Generate letter counts    
    std::vector<spc_binary_pair> sorted_charset;
    for(size_t i = 0; i < 256;i++)
    {
        if(binary_counts[i]!=0)
            sorted_charset.push_back(spc_binary_pair(i,binary_counts[i]));
    }
    //printf("len = %zu\n",len);

    //////////////
    mpz_set_ui(magic_num,0); // count = 0, this is the magic number we will be returning shortly
    mpz_t tmp;
    mpz_init(tmp);
    mpz_set_ui(tmp,len);

    bignum_factorial(tmp);
    mpz_set(tmp,factorial_result);
    mpz_div_ui(factorial_result,factorial_result,len);

    mpz_t magic_num_offset;
    mpz_init(magic_num_offset);
    mpz_set(magic_num_offset,factorial_result);

    // factorial_result stores the answer
    mpz_t permutations_skipped;
    mpz_init(permutations_skipped);
    mpz_t aux;
    mpz_init(aux);
    for(size_t i=0; i < len-1; i++) //i < len - 1
    {
        unsigned char ch = fgetc(fp);
        //find first non zero and store its' idx in curr
        mpz_set_ui(permutations_skipped,0);
        size_t idx;
        for(size_t j = 0; j < sorted_charset.size(); j++)
        {
            if(sorted_charset[j].byte == ch)
            {idx = j; break;}
            mpz_mul_ui(aux,factorial_result,sorted_charset[j].count);
            mpz_add(permutations_skipped,permutations_skipped,aux);
        }
        mpz_add(magic_num,magic_num,permutations_skipped);
        //decrement charset[curr]
        sorted_charset[idx].count -= 1;
        if(sorted_charset[idx].count == 0)
            sorted_charset.erase(sorted_charset.begin()+idx);
        mpz_div_ui(factorial_result,factorial_result,len - (i+1));
    }
    fclose(fp);
    
    mpz_add_ui(magic_num,magic_num,1);
    
    printf("magic number = ");
    mpz_out_str(stdout,10,magic_num);
    puts("");
    //write_magicnum("compressed.bin");
    //generate_seq_bignum(magic_num, tmp);

}
void binary_decompress(const char* filename,const char* outfile)
{
    size_t len = 0;
    std::vector<spc_binary_pair> sorted_charset;
    for(size_t i = 0; i < 256;i++)
    {
        if(binary_counts[i] != 0)    
            sorted_charset.push_back(spc_binary_pair(i,binary_counts[i]));
        len += binary_counts[i];
    }

    //printf("len = %zu\n",len);
   // return;
    mpz_t tmp;
    mpz_init(tmp);
    mpz_set_ui(tmp,len);
    bignum_factorial(tmp);
    mpz_set(tmp,factorial_result);
    mpz_div_ui(factorial_result,factorial_result,len);
    get_bignum_from_seq(filename,tmp);
   // mpz_sub_ui(tmp,tmp,1);
    //bignum_factorial(tmp);
    mpz_t ntile;
    mpz_init(ntile);
    
    mpz_t aux;
    mpz_init(aux);
    
    mpz_t remainder;
    mpz_init(remainder);
    FILE* fp = fopen(outfile,"wb");
    while(len != 0)
    {
        mpz_set(aux,magic_num);
        
        mpz_sub_ui(aux,aux,1);

        mpz_tdiv_r(remainder,aux,factorial_result); // 
        mpz_tdiv_q(aux,aux,factorial_result);
        mpz_add_ui(aux,aux,1); // aux = (magic_num - 1) / f + 1
        mpz_set(ntile,aux); // ntile = (magic_num - 1) / f + 1

        //update magic_num
        mpz_sub_ui(aux,aux,1);

        if(mpz_cmp_ui(remainder,0) == 0)
            mpz_set_ui(magic_num,1);
        else
        {
            mpz_set(aux,ntile);
            mpz_sub_ui(aux,aux,1);
            mpz_mul(aux,aux,factorial_result);
            mpz_add_ui(aux,aux,1);
            mpz_sub(aux,magic_num,aux);
            mpz_add_ui(aux,aux,1);
            mpz_set(magic_num,aux);        
            //magic_num = magic_num - (f * (ntile - 1) + 1) + 1;
            //return;
        }        
        size_t tmp = 0;
        for(size_t i = 0; i<sorted_charset.size();i++)
        {
            tmp += sorted_charset[i].count;
            if(mpz_cmp_ui(ntile,tmp) <= 0)
            {
                fputc(sorted_charset[i].byte,fp);
                sorted_charset[i].count -=1;
                if(sorted_charset[i].count == 0)
                    sorted_charset.erase(sorted_charset.begin() + i);
                break;
            }
        }
        if(len != 1)
            mpz_div_ui(factorial_result,factorial_result,len-1); // f is now the factorial of len-1
        len--;
    }
    fclose(fp);
}
void init_spc()
{
    mpz_init(factorial_result);
    mpz_init(magic_num);
    memset(binary_counts,0,sizeof(size_t)*256);
}

std::string generate_seq(size_t x,size_t n)
{
    if(x == 0)
        exit(1);
    //x is in range [1,n]
    size_t mid = 0;
    size_t low = 1;
    size_t high = n;
    std::string seq;
    //000010110
    while(mid != x)
    {
        //printf("[%zu , %zu]\n",low,high);
        mid = (high - low)/2 + low;
        if(x <= mid)
        {
            high = mid;
            seq+='0';
        }
        else 
        {
            low = mid+1;
            seq += '1';
        }  
    }
    return seq;
}
size_t get_num_from_seq(std::string seq)
{
    size_t low = 1;
    size_t high = 1000;
    size_t mid;
    for(auto ch: seq)
    {
        mid = (high - low)/2 + low;
        if(ch == '0')
            high = mid;
        else
            low = mid+1;
    }
    return mid;
}
void save_magic_num()
{
    mpz_t a;
    mpz_t b;
    mpz_t c;
    mpz_t d;

}
int main(int argc,const char* argv[])
{
    if(argc!=2)
        return 1;
    init_spc();
    binary_compress(argv[1]);
}
