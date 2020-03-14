#include <iostream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#define NMAX 105
using namespace std;
void resetare(char v[], int dim)
{
    for(int i=0; i<dim; i++)
        v[i]=NULL;
}
bool esteNumar (char v[NMAX][NMAX], int &indice)
{
    int i,contor=0;
    for (i=0; i<strlen(v[indice]); i++)
        if (v[indice][i]>='0' && v[indice][i]<='9')
            contor++;
    if (contor==strlen(v[indice]))
        return true;
    else
        return false;

}
bool esteTrig(char v[NMAX][NMAX], int indice)
{
    if(strcmp(v[indice],"sin")==0 || strcmp(v[indice],"cos")==0 || strcmp(v[indice],"ln")==0 || strcmp(v[indice],"rad")==0 || strcmp(v[indice],"abs")==0 ||strcmp(v[indice],"exp")==0)
        return true;
    else
        return false;
}
void CreareVectorCuvinte (char s[],char v[][NMAX], int &dim)
{
    dim=0;
    int i,j,op,ok;
    char p[NMAX];
    int k;
    char *pointer;
    while (i<strlen(s))
    {
        if ( s[i]=='(' || s[i]==')' || s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' || s[i]=='^' || s[i]=='#' || s[i]=='=' || s[i]=='<' || s[i]=='>' )
        {
            v[++dim][0]=s[i];
            i++;
        }
        else
        {
            resetare(p,NMAX);
            k=-1;
            ok=0;
            while ( (s[i]>='a'&&s[i]<='z') || (s[i]>='0'&&s[i]<='9') ) // cat timp s[i] nu este un operator
            {
                p[++k]=s[i];
                strcpy(s+i,s+i+1);
            }
            p[++k]=NULL;
            strcpy(v[++dim],p);
            v[++dim][0]=s[i];
            i++;
        }
    }
}
int prioritate(char v[NMAX][NMAX],int indice)
{
    if(v[indice][0]=='^')
        return 3;
    else if(v[indice][0]=='*' || v[indice][0]=='/')
        return 2;
    else if(v[indice][0]=='+' || v[indice][0]=='-')
        return 1;
    else if (v[indice][0]=='(' || v[indice][0]==')')
        return 0;
    else if (strcmp(v[indice],"sin")==0 || strcmp(v[indice],"cos")==0 || strcmp(v[indice],"ln")==0 || strcmp(v[indice],"rad")==0 || strcmp(v[indice],"abs")==0 ||strcmp(v[indice],"exp")==0)
        return 5;
    else
        return 4;
}
void Reparare(char v[NMAX][NMAX], int dim)
{
    int i;
    for(i=1; i<=dim; i++)
        if(esteNumar(v,i)==0)
            if (v[i][1]>='0'&&v[i][1]<='9'||prioritate(v,i)==4)
            {
                cout<<"Valoarea lui "<< v[i]<<" este: ";
                cin>>v[i];
            }
}
void FormaPost(char v[NMAX][NMAX],char post[NMAX][NMAX], int dim, int &j)
{
    char f[NMAX][NMAX];
    int k=0,i;
    for(i=1; i<=dim; i++)
    {
        if(esteNumar(v,i)==1)
            strcpy(post[++j],v[i]);
        else if(k==0)
            strcpy(f[++k],v[i]);
        else if(prioritate(v,i)>prioritate(f,k)||strcmp(v[i],"(")==0)
            strcpy(f[++k],v[i]);
        else if(prioritate(v,i)<=prioritate(f,k)&&prioritate(v,i)!=0)
        {
            strcpy(post[++j],f[k--]);
            i--;
        }
        else if(strcmp(v[i],")")==0)
            if(prioritate(f,k)==0&&esteTrig(f,k-1)==1)
            {
                strcpy(post[++j],f[--k]);
                k--;
            }
            else
            {
                while (prioritate(f,k)>0)
                {
                    strcpy(post[++j],f[k]);
                    k--;
                }

                k--;
                if (esteTrig(f,k)==1)
                {
                    strcpy(post[++j],f[k]);
                    k--;
                }
            }

    }
    while(k!=0)
        if(esteTrig(f,k-1)==1)
        strcpy(post[++j],f[k--]);
    else  strcpy(post[j++],f[k--]);

}
void DoubleToChar(double f, char s[NMAX][NMAX],int &j)
{
    int i=0, rff=0, dim=0, uc;
    double cf=f;
    do
    {
        i++;
        f=f*10;
    }
    while (int(f)!=f);
    int ff=int(cf);
    while (ff)
    {
        rff=rff*10+ff%10;
        ff=ff/10;
    }
    while (rff)
    {
        uc=rff%10;
        char c=uc+'0';
        s[j][dim]=c;
        rff=rff/10;
        dim++;
    }
    if(i!=0)
        s[j][dim]='.';
    int fractionar = (cf-(int)cf)*pow(10,i);
    rff=0;
    while (fractionar)
    {
        rff=rff*10+fractionar%10;
        fractionar=fractionar/10;
    }
    while (rff)
    {
        s[j][++dim]=rff%10+'0';
        rff=rff/10;
    }
    s[j][++dim]='\0';
}
double Calculare(char post[NMAX][NMAX], int j, double &suma)
{
    char raspuns[NMAX][NMAX];
    int k=0,i;
    for(i=1; i<=j; i++)
    {
            if(esteNumar(post,i)==1&&(post[i][1]!=' '||strcmp(post[i],NULL)!=0))
                strcpy(raspuns[++k],post[i]);
            else if(strcmp(post[i],"+")==0)
            {
                suma=atof(raspuns[k])+atof(raspuns[--k]);
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"-")==0)
            {
                suma=atof(raspuns[--k])-atof(raspuns[++k]);k--;
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"/")==0)
            {
                suma=atof(raspuns[--k])/atof(raspuns[++k]);k--;
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"*")==0)
            {
                suma=atof(raspuns[k])*atof(raspuns[--k]);
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"^")==0)
            {
                suma= pow(atof(raspuns[--k]),atof(raspuns[++k]));k--;
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"sin")==0)
            {
                suma= sin(atof(raspuns[k]));
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"cos")==0)
            {
                suma= cos(atof(raspuns[k]));
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"ln")==0)
            {
                suma= log(atof(raspuns[k]));
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"rad")==0)
            {
                suma= sqrt(atof(raspuns[k]));
                DoubleToChar(suma,raspuns,k);
            }
            else if(strcmp(post[i],"abs")==0)
            {
                suma= abs(atof(raspuns[k]));
                DoubleToChar(suma,raspuns,k);
            }
    }suma=atof(raspuns[1]);
}
int VerificaParanteze (char v[][NMAX],int dim, int &caz, int &pozitie)
{
    int nrParantezeDeschise=0;
    int nrParantezeInchise=0;
    char parantezaDeschisa[NMAX]={'('};
    char parantezaInchisa[NMAX]={')'};
    int i;
    caz=0,pozitie=0;
    for (i=1; i<=dim; i++)
    {
        if (strcmp(v[i],parantezaDeschisa)==0)
            nrParantezeDeschise++;
        else if (strcmp(v[i],parantezaInchisa)==0)
            nrParantezeInchise++;

        if (strcmp(v[i],parantezaDeschisa)==0 && strcmp(v[i+1],parantezaInchisa)==0)
        {
            caz=1;
            pozitie=i;
            return 0; // nu exista element intre paranteze

        }
        if (strcmp(v[i],parantezaInchisa)==0 && strcmp(v[i+1],parantezaDeschisa)==0)
        {
            caz=2;
            pozitie=i;
            return 0; // nu exista operator intre paranteze
        }
    }
    if (nrParantezeDeschise>nrParantezeInchise)
        caz=3;
    if (nrParantezeDeschise<nrParantezeInchise)
        caz=4;
    if (nrParantezeDeschise==nrParantezeInchise)
        return 1;
    else
        return 0;
}
int VerificaExpresie ( char v[][NMAX], int dim)
{
    int ok=1,i;
    for (i=1;i<dim;i++)
    {

    if (prioritate(v,i)>=1 && prioritate(v,i)<=3)
        if (prioritate(v,i+1)>=1 && prioritate(v,i+1)<=3)
        ok=0;
     if (prioritate(v,i)==5 && prioritate(v,i+1)!=0)
        ok=0;
     if (esteNumar(v,i))
        if (prioritate(v,i+1)==0)
        ok=0;

     return ok;

    }

}

int main()
{double suma;
    char post[NMAX][NMAX],p[NMAX],v[NMAX][NMAX],s[NMAX];
    int dim, i,j=0,caz,pozitie;
    cin.getline(s,NMAX);
    CreareVectorCuvinte(s,v,dim);


//aici faci erorile
    if (VerificaParanteze(v,dim,caz,pozitie)==0)
    {
        if (caz==1)
        {
            cout <<"Pozitia "<<pozitie<<": nu exista element intre paranteze";
            return 0;
        }
        else if (caz==2)
        {
            cout <<"Pozitia "<<pozitie<<": nu exista operator intre paranteze";
            return 0;
        }
        else if(caz==3)
        {
            cout <<"Sunt mai multe paranteze deschise decat inchise";
            return 0;
        }
        else if(caz==4)
        {
            cout<<"Sunt mai multe paranteze inchise decat deschise";
            return 0;
        }
    }
    if (VerificaExpresie(v,dim)==0)
        {cout <<"Expresia este scrisa gresit";
    return 0;}

    Reparare(v,dim);

    /*for(i=1; i<=dim; i++)
        cout <<endl<<v[i];*/ ///vector de cuvinte

    FormaPost(v,post,dim,j);

    /*for(i=1; i<=j; i++)
        cout<<endl<<post[i];*/ ///forma postfixata

    Calculare(post,j,suma);
    cout<<suma;
    return 0; //merge +, -, /, * (fara 5 si 10), rad
}
