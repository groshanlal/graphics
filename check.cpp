#include<iostream>

using namespace std;

int main()
{
    int* arr1;
    int* arr2;
    int* p;

    arr1 = new int[4];
    arr2 = new int[4];
    p = NULL;

    arr1[0]=1;
    arr1[1]=7;
    arr1[2]=2;
    arr1[3]=9;

    arr2[0]=1;
    arr2[1]=5;
    arr2[2]=1;
    arr2[3]=0;

    p = arr2;
    arr2 = arr1;
    arr1 = p;

    p = arr1;
    for(int i=0;i<4;i++)
        cout<<p[i]<<", ";
    cout<<endl;


    p = arr2;
    for(int i=0;i<4;i++)
        cout<<p[i]<<", ";
    cout<<endl;

    string s;
    s="ACTUAL";

    if(s=="ACTUAL")
        cout<<s<<endl;

    s="TRANSIENT";
    if(s=="TRANSIENT")
        cout<<s<<endl;

}
