#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;
int i=0;
string b;
bool ok=0;
int mm=1;
void brute(int go,string thr,string pu)
{
    ofstream fout("nrsp");
    fout<<"c="<<go<<endl;
    fout<<"threads="<<thr<<endl;
    fout<<"port="<<pu<<endl;
    fout<<"bash brut $c $threads $port"<<endl;
    system("bash nrsp");
    system("rm -rf xaa  xab  xac  xad  xae  xaf  xag  xah  xai  xaj  xak  xal  xam  xan  xao  xap  xaq  xar  xas  xat  xau  xav  xaw  xax  xay");
    system("pkill -f cola");
    system("killall -9 cola");
    system("rm -rf nrsp");
}

int  split(int nr,string tor,string port)
{
    int aux,nrsp,j=1;


    aux=nr;
    while(aux>75000)
    {
        aux-=75000;
        j++;
    }
    system("split -l 75000 ips");
    cout<<"Avem " << nr<<" ipuri si am creat "<<j;
    if(j!=1)
    cout<<" fisiere\n";
    else
    cout<<" fisier\n";
    cout<<"Port "<<port<<endl;
    cout<<"Incepem scanul cu "<<tor<<" threads\n";

    system("sleep 5");
    return j;
}
int main()
{

    ifstream fin("ips");
    while(fin>>b)
    {
        i++;
    }
    string poz,aux,aux2;
    ifstream fin2("a");
    while(fin2>>poz)
    {
        if(mm==2)
        aux=poz;
        if(mm==4)
        aux2=poz;
        mm++;
    }

    int n;
    n=split(i,aux,aux2);
    brute(n,aux,aux2);

}

