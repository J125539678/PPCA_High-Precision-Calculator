#pragma once
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iostream>
//using namespace std;
FILE *input = stdin;
string s="";
int len=0;
char *next_token() {
    char *res = NULL;
	string ans;
    int cur=0;
    int ch;
    int counter = 0;
    while (!cur)
    {
        if(len > 0 && (s[0] == '(' || s[0] == ')'))// for cases:'(' ')'
        {
            ans=s;
            cur=len;
            s="";
            len=0;
            break;
        }
        if (ch == '\"')
        	++counter;
        if ((ch = fgetc(input)) == EOF)
            break;
        switch (ch)
        {
            case '(':
            case ')':
            	if (counter % 2 == 0 and s != "#\\"){
                	if(len>0)
                    	ans=s;cur=len;
                	s="";
                	s=s+(char)ch;
                	len=1;
                	break;
            	}
            default:
                if (isspace(ch) and counter % 2 == 0 and s != "#\\") 
                {
                    if(len>0)
                         ans=s;cur=len;
                    s="";
                    len=0;
                }
                else
				{
					s=s+(char)ch;
                    len++;
                }
        }
    }
    if(cur>0)
    {
        res=new char[cur+2];
        for(int i=0;i<=cur-1;i++)
            res[i]=ans[i];
		res[cur]='\0';
    }
    return res;
}
