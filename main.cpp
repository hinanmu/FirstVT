#include <iostream>
#include"fstream"
#include <cstring>
#include <cstdio>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cctype>
#define MAX 507

using namespace std;

class WF
{
    public:
    string left;
    vector<string> right;
    WF ( const string& str )
    {
        left = str;
    }
    void insert ( char str[] )
    {
        right.push_back(str);
    }
    void print ( )
    {
        printf ( "%s->%s" , left.c_str() , right[0].c_str() );
        for ( int i = 1 ; i < right.size() ; i++ )
            printf ( "|%s" , right[i].c_str() );
        puts("");
    }
};

vector<char> VT;
vector<WF> VN_set;  //保存文法对象列表
map<string,int> VN_dic;
set<char> first[MAX];
int used[MAX];
int vis[MAX];


void dfs (  int x )
{
    if ( vis[x] ) return;
    vis[x] = 1;  //访问标志位
    string& left = VN_set[x].left;  //非终结符
    for ( int i = 0 ; i < VN_set[x].right.size() ; i++ )
    {
        string& str = VN_set[x].right[i];
        if ( isupper(str[0]) ) //如果产生式的第一个位置为大写字母（非终结符）
        {
            int y = VN_dic[str.substr(0,1)]-1;   //令y为第一个位置为非终结符的序号，在之后DFS  p→Qa•••
            if ( str.length() > 1 && !isupper(str[1] ) )
                first[x].insert ( str[1] );//第一个位置为非终结符，第二个位置为终结符则加入p→Qa•••
            dfs ( y ); //继续找Q  p→Qa•••
            set<char>::iterator it = first[y].begin();
            for ( ; it!= first[y].end() ; it++ ) //  p→Qa••,把Q的FisrtVT加入到P中
                first[x].insert ( *it );
        }
        else
            first[x].insert ( str[0] );  // P→a•••
    }
}

void make_first ( )
{
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        if ( vis[i] ) continue;
        else dfs ( i );
#define DEBUG
#ifdef DEBUG
    puts("------------FIRSTVT-------------------");
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        printf ( "%s : " , VN_set[i].left.c_str() );
        set<char>::iterator it = first[i].begin();
        for ( ; it!= first[i].end() ; it++ )
            printf ( "%c " , *it );
        puts ("" );
    }
#endif
}

int main ( )
{
    int n;
    char s[MAX];

    ifstream in;
    in.open("C://Users//nanmu//Desktop//in.txt");

    while(!in.eof()){
            in >> s;
        memset ( used , 0 , sizeof ( used ) );


            int len = strlen(s),j;
            for ( j = 0 ; j < len ; j++ )
                if ( s[j] == '-' )
                    break;
            s[j] = 0;

            if ( !VN_dic[s] )//防止非终结符对应的文法对象重复生成
            {

                VN_set.push_back ( WF(s) ); //插入非终结符
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert ( s + j + 2 ); //插入产生式，可能有多个

            for ( int k = 0 ; k < j; k++ )
                if ( !isupper(s[k] ) )
                {
                    if ( used[s[k]] ) continue;
                    used[s[k]] = 1;
                    VT.push_back ( s[k] );
                }
            for ( int k = j+2 ; k < len; k++ )
                if ( !isupper(s[k] ) )
                {
                    if ( used[s[k]] ) continue;
                    VT.push_back ( s[k] );
                    used[s[k]] = VT.size();
                }
        }
#define DEBUG
#ifdef DEBUG
        puts ("************VT*******************");
        for ( int i = 0 ; i < VT.size() ; i++ )
            printf ( "%c " , VT[i] );
        puts ("");
        puts("************ *****************");
        for ( int i = 0 ; i < VN_set.size() ; i++ )
            VN_set[i].print();
        puts("************************************");
#endif
        make_first();



}
