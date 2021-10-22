#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char *stringa;
struct node
{
    stringa line;
};
typedef struct node* list;


struct confnode
{
    struct confnode* next;
    int *a;          //per salvare il comando;
    list* piece;        //per salvare i pezzi
};
typedef struct confnode* conflist;

#define TOP 100000000
list hash[TOP];// RENDERE DINAMICO, ORA RADDOPPIA LA D

void newhead(stringa c,list *l)                 //crea pezzo di lista
{
    struct node *temp=malloc(sizeof(struct node*));
    temp->line=malloc(sizeof(char)*(2+strlen(c)));
    strcpy(temp->line,c);
    *l=temp;
}

void confflush(conflist* redo)                   //flush senza free
{
    ///printf("flusho\n");
    *redo=NULL;
}

void strprocess(char* command,int* lines)  //command è il comando
{
    int count=0;
    for(int i=0;i<strlen(command)-2;i++)  //cerco la posizione in cui sta la virgola (count)
    {
        if(command[i]==',')
            break;
        count++;
    }
    char temp[1600];
    for(int i=0;i<1600;i++)  //inizializzo
        temp[i]='/';
    strncpy(temp,command,count);  //copio la roba prima della virgola
    lines[0]=atoi(temp);    //lo metto in lines0
    int cap=strlen(command)-3-count;
    for(int i=0;i<cap;i++)   //copio la roba dopo la virgola
    {
        if(i>1600)
            break;
        temp[i]=command[count+1+i];
    }
    lines[1]=atoi(temp);   //la metto in lines1
}

void reindex(list *h,int count,int start,int howmany)
{
    for(int i=0;i<count;i++)
    {
        // printf("metto %d in %d\n",start+i,start-howmany+i);
        h[start-howmany+i]=h[start+i];
    }
}

void newconfhead(conflist* conf)
{
    conflist mem = malloc(sizeof(conflist));
    mem->next = *conf;
    *conf=mem;
}

void confheadjump(conflist* conf,conflist* redoconf)
{
    conflist temp=*conf;
    //printf("mando \n");
    *conf=(*conf)->next;
    temp->next=*redoconf;
    *redoconf=temp;  ///mette la testa di una in testa all'altra
}


void makepiece(conflist* conf,int start, int before)
{
    (*conf)->piece=malloc(sizeof(list)*(before+1));///copia  gli indirizzi
    int j=0;
    for(int i=0;i<before;i++)
    {
        (*conf)->piece[j]=hash[start+i];
        /// printf("salvo %s",(*conf)->piece[j]->line);           /////PROBLEMA SU REDO
        j++;
    }
}

void makepiecespecial(conflist* conf,int start, int before,int dim)
{
    (*conf)->piece=malloc(sizeof(list)*(dim+1));
    int j=0;
    for(int i=0;i<before;i++)
    {
        (*conf)->piece[j]=hash[start+i];
        /// printf("salvo %s",(*conf)->piece[j]->line);           /////PROBLEMA SU REDO
        j++;
    }
}

void swapperinoinversespecial(conflist* conf, int* currline)    ////salva solo mentre mette all'inizio
{
    int lines0=(*conf)->a[0];
    int oldcur=(*conf)->a[3];
    ///printf(" lavoro con oldcur %d currline %d",(*conf)->a[3],*currline);
    list temp=NULL;
    list* temp2=(*conf)->piece;
    int j=0;
    for(int i=lines0;i<*currline;i++)
    {
        temp=hash[i];
        hash[i]=temp2[j];
        temp2[j]=temp;

        ///printf("salvo %s",temp2[j]->line);
        j++;
    }
    for(int i=*currline;i<oldcur;i++)         ///continua a piazzare quelli dopo
    {
        hash[i]=temp2[j];
        ///printf("rimetto %s",temp2[j]->line);
        j++;
    }
    //printf("inverse passo currline da %d a %d",*currline,oldcur);
    ///printf("currline salvata diventa %d ",oldcur);
    *currline=oldcur;
    //printf("currline reale diventa %d\n",*currline);
}


void swapperinoinverse(conflist* conf, int* currline)
{
    int lines0=(*conf)->a[0];
    int oldcur=(*conf)->a[3];
    list temp=NULL;
    list* temp2=(*conf)->piece;
    int j=0;
    for(int i=lines0;i<oldcur;i++)
    {
        temp=hash[i];
        hash[i]=temp2[j];
        temp2[j]=temp;

        /// printf("salvo %s",temp2[j]->line);
        j++;
    }
    for(int i=oldcur;i<*currline;i++)         ///continua a piazzare quelli dopo
    {
        hash[i]=temp2[j];
        ///printf("rimetto %s",temp2[j]->line);
        j++;
    }
    //printf("inverse passo currline da %d a %d",*currline,oldcur);
    (*conf)->a[3]=*currline;
    *currline=oldcur;
}
void swapperino(conflist* conf,int* currline)
{
    int lines0=(*conf)->a[0];
    int oldcur=(*conf)->a[3];   ///si prende linea o vecchia linea max
    list temp=NULL;
    list* temp2=(*conf)->piece;
    int j=0;
    ///printf(" lavoro con oldcur %d currline %d lines0 %d\n",oldcur,*currline,lines0);
    for(int i=lines0;i<oldcur;i++)
    {
        temp=hash[i];
        hash[i]=temp2[j];           //scambia i primi elementi
        temp2[j]=temp;

        ///  printf("rimetto %s",temp2[j]->line);
        j++;
    }
    for(int i=oldcur;i<*currline;i++)        ///continua a salvare quelli dopo
    {
        temp2[j]=hash[i];
        ///printf("salvo %s",temp2[j]->line);
        j++;
    }
    //printf("swapperino passo currline da %d a %d",*currline,oldcur);
    (*conf)->a[3]=*currline;
    *currline=oldcur;
}

void swapperinospecial(conflist* conf,int* currline)
{
    int lines0=(*conf)->a[0];
    int oldcur=(*conf)->a[1];
    list temp=NULL;
    list* temp2=(*conf)->piece;
    int j=0;
    ///printf(" lavoro con oldcur %d currline %d lines0 %d\n",oldcur,*currline,lines0);
    for(int i=lines0;i<oldcur;i++)
    {
        temp=hash[i];
        hash[i]=temp2[j];
        temp2[j]=temp;

        // printf("rimetto %s",temp2[j]->line);
        j++;
    }
    for(int i=oldcur;i<*currline;i++)        ///continua a salvare quelli dopo
    {
        temp2[j]=hash[i];
        //printf("salvo %s",temp2[j]->line);
        j++;
    }
    //printf("swapperino passo currline da %d a %d",*currline,oldcur);
    *currline=oldcur;
}

void delete2append(list* h, conflist* conf,int* currline) /////è la undo di append, cambia l'indirizzo dell'ultima riga lasciando le altre come sono
{
    int lastpos=(*conf)->a[3];
    if (lastpos == 1)
    {
        (*conf)->a[3]=*currline;
        *currline=1;
    }
    else
    {
        int oldcur=(*conf)->a[3];
        makepiece(conf,oldcur,*currline-oldcur);
        (*conf)->a[3]=*currline;
        *currline=oldcur;
    }

}


void append2delete(list* h,conflist* conf,int* currline)   ///////rimette le ultime righe precedentemente eliminate
{
    int oldcur=(*conf)->a[3];
    list* bruh=(*conf)->piece;
    int j=0;
    for(int i=*currline;i<oldcur;i++)   //////NON FARLO A PRESCINDERE
    {
        hash[i]=bruh[j];
        j++;
    }
    ///printf("append2delete passo currline da %d a %d\n",*currline,oldcur);
    (*conf)->a[3]=*currline;
    *currline=oldcur;
}

void append2deletespecial(list* h,conflist* conf,int* currline)   ///////
{
    int oldcur=(*conf)->a[3];
    list* bruh=(*conf)->piece;
    int j=0;
    for(int i=(*conf)->a[0];i<oldcur;i++)   //////NON FARLO A PRESCINDERE
    {
        hash[i]=bruh[j];
        j++;
    }
    ///printf("append2delete passo currline da %d a %d\n",*currline,oldcur);
    (*conf)->a[3]=*currline;
    *currline=oldcur;
}


void print(int* lines,int currline,list a[])
{
    if(lines[0]>currline-1)
    {
        for(int i=0;i<lines[1]-lines[0]+1;i++)
        {
            fputs(".\n",stdout);
            // printf(".\n");
        }
    }
    else if(lines[0]!=0)
    {
        int over=0;
        if(lines[1]>currline-1)
        {
            over = lines[1] - currline + 1;
            lines[1] = currline - 1;
        }
        int i;
        for(i=0;i<lines[1]-lines[0]+1;i++)
            fputs(a[lines[0]+i]->line,stdout);
        for(i=0;i<over;i++)
            fputs(".\n",stdout);
    }
    else printf(".\n");
}

void change2change(conflist* conf,int* currline)
{
    int lines1=(*conf)->a[0];
    int lines2=(*conf)->a[1];
    list* temp=(*conf)->piece;      //scambia i puntatori (change interna)
    list temp2=NULL;
    int lines[2];
    lines[0]=lines1;
    lines[1]=lines2;
    ///printlinkedlist(temp,3);
    int j=0;
    for(int i=lines1;i<lines2+1;i++)
    {
        temp2=hash[i];
        hash[i]=(*conf)->piece[j];
        temp[j]=temp2;
        j++;
    }
    //printf("lines2 %d\n",lines2);
}

void deshift(conflist* conf,int* currline)  ///trash
{
    int lines0=(*conf)->a[0];
    int lines1=(*conf)->a[1];
    int count=(*conf)->a[3];
    list* nice=(*conf)->piece;
    int j=0;
    for(int i=count-lines1-1;i>0;i--)
    {
        hash[lines1-lines0+1+i]=hash[i];
        ///printf("sposto stra di %d la parola %s",lines1-lines0+1,hash[lines1-lines0+1+i]->line);
    }
    ///printf("deshift passo currline da %d a %d\n",*currline,count);
    (*conf)->a[3]=*currline;
    *currline=count;
    int k=1;
    for(int i=0;i<lines1-lines0+1;i++)
    {
        hash[k]=nice[j];
        ///printf("metto %s",nice[j]->line);
        j++;
        k++;
    }
}

void pushsave(conflist*conf,int lines0,int lines1,int* currline)
{
    newconfhead(conf);
    makepiece(conf, lines0, lines1 - lines0 + 1);
    (*conf)->a = malloc(sizeof(int) * 4);
    (*conf)->a[0] = lines0;
    (*conf)->a[1] = lines1;
    (*conf)->a[2] = 6;
    (*conf)->a[3] = *currline;
}

void undosave(list* h,conflist* conf,int* currline)   ///////
{
    int oldcur=(*conf)->a[3];
    list* bruh=(*conf)->piece;
    int j=0;
    for(int i=*currline;i<oldcur;i++)   //////NON FARLO A PRESCINDERE
    {
        hash[i]=bruh[j];
        j++;
    }
    (*conf)->a[3]=*currline;
    *currline=oldcur;
}


void undodebug(conflist* conf,conflist* redoconf, int *currline)
{
    if(*conf==NULL)
        return;
    conflist temp=*conf;
    /// temp=confslam(temp,count);   ////elimina tutti quelli prima del prescelto
    ///   printf("cerco di ripristinare la configurazione:\n");
    if(temp->a[0]==0)
    {
        printf("mando useless in redo\n");
    }
    if(temp->a[2]==1)  ///redoify   la delete è stata pushata durante l'append, quindi si poppa trasformandosi nuovamente in append
    {
        printf("mando append in redo\n");

    }
    if(temp->a[2]==2)
    {
        printf("mando change in redo\n");
    }
    if(temp->a[2]==3)
    {
        printf("mando changeappend in redo\n");
    }
    if(temp->a[2]==4)
    {
        printf("rimetto in coda\n");
    }
    if(temp->a[2]==5)
    {
        printf("faccio deshift\n");
    }
    if(temp->a[2]==6)
    {
        printf("faccio inverse\n");
    }
}


void popundo(conflist* conf,conflist* redoconf, int *currline)
{
    if(*conf==NULL)
        return;
    conflist temp=*conf;
    /// temp=confslam(temp,count);   ////elimina tutti quelli prima del prescelto
    ///   printf("cerco di ripristinare la configurazione:\n");
    ///undodebug(conf,redoconf,currline);
    if(temp->a[0]==0)
    {
        confheadjump(conf,redoconf);
        //printf("mando useless in redo\n");
    }
    if(temp->a[2]==1)  ///redoify   la delete è stata pushata durante l'append, quindi si poppa trasformandosi nuovamente in append
    {
        delete2append(hash,conf,currline);
        confheadjump(conf,redoconf);

        ///printf("mando append in redo\n");

    }
    if(temp->a[2]==2)
    {
        change2change(conf,currline);
        //printf("mando in redostack:\n");
        //printlist((*conf)->data,5);
        confheadjump(conf,redoconf);
        //printf("mando change in redo\n");
        //printf("stato save:\n");
        //printlist((*conf)->data,5);
    }
    if(temp->a[2]==3)
    {
        swapperino(conf,currline);
        confheadjump(conf,redoconf);
        ///printf("mando changeappend in redo\n");
        return;
    }
    if(temp->a[2]==4)
    {
        append2deletespecial(hash,conf,currline);
        confheadjump(conf,redoconf);
    }
    if(temp->a[2]==5)
    {
        deshift(conf,currline);
        ///printf("faccio deshift\n");
        confheadjump(conf,redoconf);
    }
    if(temp->a[2]==6)
    {
        swapperinoinversespecial(conf,currline);
        ///printf("faccio inverse\n");
        confheadjump(conf,redoconf);
    }
}


void pushundoappend(list* l,conflist* conf,int* lines,int currline)    /////dà le informazioni per effettuare una undo di appen (cioe delete)
{

    newconfhead(conf);
    makepiece(conf,lines[0],lines[1]-lines[0]+1);
    (*conf)->a=malloc((sizeof(int)*4));
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=lines[1];
    (*conf)->a[2]=1;
    (*conf)->a[3]=currline;
    ///printf("pusho append\n");
}

void pushundochangeonly(conflist* conf,int* lines,int* currline)
{

    //array2link(&l,lines);
    newconfhead(conf);
    makepiece(conf,lines[0],lines[1]-lines[0]+1);
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=lines[1];
    (*conf)->a[2]=2;
    (*conf)->a[3]=*currline;
    //printf("stato hash:\n");
    ///   printf("pusho changeonly\n");


    //printf("stato save:\n");
    //printlinkedlist((*conf)->data,5);


}

void pushundodeltail(conflist* conf,int* lines,int* currline)
{
    newconfhead(conf);
    makepiece(conf,lines[0],lines[1]-lines[0]+1);
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=lines[1];
    (*conf)->a[2]=4;
    (*conf)->a[3]=*currline;
    ///printf("pusho deltail\n");
}

void savefrom(conflist* conf,int* lines,int* currline)
{
    newconfhead(conf);
    makepiece(conf,lines[0],*currline-lines[0]);
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=*currline-lines[1]+lines[0]-1;
    (*conf)->a[2]=6;
    (*conf)->a[3]=*currline;
    //printf("pusho save\n");
}



void pushundochangeappend(conflist* conf,int* lines,int* currline)
{

    newconfhead(conf);
    makepiecespecial(conf,lines[0],*currline-lines[0],*currline-lines[0]);  ///lines[1] deve essere sempre currline-1
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=lines[1];
    (*conf)->a[2]=3;
    (*conf)->a[3]=*currline;
    ///printf("pusho changeappend\n",*currline);
    //printf("stato hash:\n");
    // printf("pusho change\n");


    //printf("stato save:\n");
    //printlinkedlist((*conf)->data,5);


}

void pushundodelhead(conflist* conf,int* lines,int* currline)
{
    newconfhead(conf);
    makepiece(conf,lines[0],lines[1]-lines[0]+1);  ///lines[1] deve essere sempre currline-1
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=lines[0];
    (*conf)->a[1]=lines[1];
    (*conf)->a[2]=5;
    (*conf)->a[3]=*currline;
    ///printf("salvo %d da ripristinare\n",*currline);
}



void pushuseless(conflist* conf)
{
    newconfhead(conf);
    (*conf)->a=malloc(sizeof(int)*4);
    (*conf)->a[0]=0;
    ///  printf("pusho useless \n");
}

void redodebug(conflist* conf,conflist* redo,int* currline)
{
    if(*redo==NULL)
        return;
    conflist temp=*redo;
    if(temp->a[0]==0)
    {
        printf("mando useless in undo\n");
        return;
    }
    if(temp->a[2]==2)
    {
        printf("mando change in undo\n");
        return;

    }
    if(temp->a[2]==1)
    {
        printf("mando append in undo\n");
    }
    if(temp->a[2]==3)
    {
        printf("faccio inverse\n");
    }
    if(temp->a[2]==4)
    {
        printf("rimetto in coda\n");
    }

    if(temp->a[2]==5)
    {
        printf("passso currlien da %d ",*currline);
    }
    if(temp->a[2]==6)
    {
        printf("faccio swapperino\n");
    }
}
void popredo(conflist* conf,conflist* redo,int* currline)
{
    if(*redo==NULL)
        return;
    conflist temp=*redo;
    //   redodebug(conf,redo,currline);
    if(temp->a[0]==0)
    {
        confheadjump(redo,conf);
        ////printf("mando useless in undo\n");
        return;
    }
    if(temp->a[2]==2)
    {
        change2change(redo,currline);
        ///printf("mando change in undo\n");
        confheadjump(redo,conf);
        return;

    }
    if(temp->a[2]==1)
    {
        append2delete(hash,redo,currline);
        ///printf("mando append in undo\n");
        confheadjump(redo,conf);
    }
    if(temp->a[2]==3)
    {
        swapperinoinverse(redo,currline);
        ///printf("faccio inverse\n");
        confheadjump(redo,conf);
    }
    if(temp->a[2]==4)
    {
        delete2append(hash,redo,currline);
        confheadjump(redo,conf);
    }

    if(temp->a[2]==5)
    {
        int lines0=(*redo)->a[0];
        int lines1=(*redo)->a[1];
        int curr=(*redo)->a[3];
        (*redo)->a[3]=*currline;

        reindex(hash,*currline-lines1-1,lines1+1,lines1-lines0+1);
        ///printf("passso currlien da %d a %d",*currline,curr);
        *currline=curr;
        confheadjump(redo,conf);
    }
    if(temp->a[2]==6)
    {
        swapperinospecial(redo,currline);
        ///printf("faccio swapperino\n");
        confheadjump(redo,conf);
    }
}

void b4(int before,int start)                    //ok
{
    char command[1028];
    //  temp=listrun(temp,start);
    // printf("salto a %s",hash[start]->line);
    for(int i=0;i<before;i++)
    {
        //    fgets(command,1028,stdin);
        //  hash[start+i]->line=realloc(hash[start+i]->line,sizeof(char)*(2+strlen(command)));
        //strcpy(hash[start+i]->line,command);
        list l;
        fgets(command,1028,stdin);
        newhead(command,&l);  //doppia malloc
        hash[start+i]=l;
    }

}

void scanna(int* currline, int before)
{
    char command[1025];
    int lol=*currline;
    for(int i=0;i<before;i++)              //fare funzione
    {
        list temp;
        fgets(command,1028,stdin);
        newhead(command,&temp);
        hash[lol]=temp;
        lol++;
    }
    *currline=lol;
}
int cap=0;
int redocap=0;
void undopopper(conflist* conf,conflist* redo,int* currline,int jump,int* timeline)
{
    for (int i = 0; i < jump; i++)
    {
        if (conf == NULL)
            break;
        popundo(conf, redo, currline);
        //       printf("currline è %d\n", currline);
        *timeline=*timeline+1;
    }
}

void redopopper(conflist* conf,conflist* redo,int* currline,int jump,int* timeline)
{
    for (int i = 0; i > jump; i--) {
        if (redo == NULL)
            break;
        popredo(conf, redo, currline);
        ///     printf("currline è %d\n",currline);
        ///  printf("riduco\n");
        *timeline=*timeline-1;
    }
}

int main()
{
    int timeline=0;
    int flag=0;
    conflist conf=NULL;
    conflist redo=NULL;
    int currline=1;
    int lines[2];
    char command[1028];
    int hang=0;
    while(1)
    {
        fgets(command,1028,stdin);
        char c=command[strlen(command)-2];
        if(c=='q')
            break;
        if(c=='r')
        {
            fflush(stdin);
        }
        if(c=='u')
        {
            lines[0]=atoi(command);
            undopopper(&conf,&redo,&currline,lines[0],&timeline);
            int hang=0;
            int jump = hang-timeline;
    /*        if (lines[0] > cap)
            {
                redocap=redocap+cap;
                hang = hang+cap;
                cap=0;
            }
            else
            {
                cap=cap-lines[0];
                hang = hang + lines[0];             //time la posizione dove ci si trovaa
                redocap=redocap+lines[0];
            }*/
            while(1)
            {
                fgets(command,1028,stdin);
                c=command[strlen(command)-2];
                if (c == 'u')                     //hang tiene la posizione rispetto a dove ci si trova
                {
                    lines[0] = atoi(command);
                    undopopper(&conf,&redo,&currline,lines[0],&timeline);
                    /*     ///printf("%d\n",lines[0])
                         if (lines[0] > cap)
                         {
                             redocap=redocap+cap;
                             hang = hang+cap;
                             cap=0;
                         }
                         else
                         {
                             cap=cap-lines[0];
                             hang = hang + lines[0];             //time la posizione dove ci si trovaa
                             redocap=redocap+lines[0];
                         }
                         jump = hang - timeline;*/
                    //    printf("devo andare a %d hang: %d lines: %d\n",jump,hang,lines[0]);
                } else if (c == 'r')
                {
                    lines[0] = atoi(command);
                    redopopper(&conf,&redo,&currline,0-lines[0],&timeline);
/*                    //if (hang < 0)
                      //  hang = 0;
                    if (lines[0] > redocap)
                    {
                        cap=redocap+cap;
                        hang = hang-redocap;
                        redocap=0;
                    }
                    else
                    {
                        cap=cap+lines[0];
                        hang = hang - lines[0];             //time la posizione dove ci si trovaa
                        redocap=redocap-lines[0];
                    }
                    jump = hang - timeline;*/
                    ///printf("devo andare a %d\n", jump);

                } else if (c =='p')              ///quando si vuole stampare ci si muove senza poppare la pila redo;
                {
                    ////se hang è piu indietro si faranno redo pop sennò undo pop

                    ////dopo aver salvato la differenza mi muovo
                   /* if (jump > 0)
                        undopopper(&conf,&redo,&currline,jump,&timeline);
                    if (jump < 0)
                        redopopper(&conf,&redo,&currline,jump,&timeline);*/
/*                    if (hang > 0)
                        undopopper(&conf,&redo,&currline,hang,&timeline);
                    if (hang < 0)
                        redopopper(&conf,&redo,&currline,hang,&timeline);*/
                    hang = 0;
                    ///printf("jump a zero\n");
                    flag = 0;
                    jump = 0;
                    strprocess(command, lines);
                    print(lines, currline, hash);
                }
                else if (c=='q')
                {
                    break;
                }

                else if ((c=='d')||(c=='c'))
                {

            /*        if (jump > 0)
                        undopopper(&conf,&redo,&currline,jump,&timeline);
                    if (jump < 0)
                        redopopper(&conf,&redo,&currline,jump,&timeline);
                    hang = 0;*/
                    break;
                    ///printf("sono a %d\n", timeline);
                }

                ///printf("%d cap %d redocap %d hang\n",cap,redocap,hang);
                  //    printf("%d hang\n",hang);
                  //     printf("%d timeline\n",timeline);
            }
                 /////       CAMBIARE IL SISTEMA DI CONTEGGIO DI UNDO E REDO
            redocap=0;

         ///   printf("cap aggiornato %d\n",cap);
            timeline = 0;
            confflush(&redo);

 /*           while(1)
            {
                if(c=='u')
                {
                    lines[0]=atoi(command);
                    hang=hang+lines[0];
                    if(hang>cap)
                        hang=cap;
                    jump=hang-timeline;
                }
                if(c=='r')
                {
                    lines[0]=atoi(command);
                    hang=hang-lines[0];
                    if(hang<0)
                        hang=0;
                    jump=hang-timeline;
                }
                if(c=='p')
                {
                    if(jump>0)
                        undopopper(&conf,&redo,&currline,jump);
                    if(jump<0)
                        redopopper(&conf,&redo,&currline,jump);
                    hang=timeline;
                    jump=0;
                    strprocess(command,lines);
                    print(lines,currline,hash);
                }
                if((c=='d')||(c=='c'))
                {
                    if(jump>0)
                        undopopper(&conf,&redo,&currline,jump);
                    if(jump<0)
                        redopopper(&conf,&redo,&currline,jump);
                    hang=0;
                    timeline=0;
                    confflush(&redo);
                    break;
                }
                if(c=='q')
                    break;

            }*/


         /*   do {
                flag = 1;
                /// printf("%c\n",c);
                if (c == 'u')                     //hang tiene la posizione rispetto a dove ci si trova
                {
                    lines[0] = atoi(command);
                    hang = hang + lines[0];             //time la posizione dove ci si trovaa
                    jump = hang - timeline;
                    //    printf("devo andare a %d hang: %d lines: %d\n",jump,hang,lines[0]);
                } else if (c == 'r') {
                    lines[0] = atoi(command);
                    hang = hang - lines[0];
                    if (hang < 0)
                        hang = 0;
                    jump = hang - timeline;
                    ///printf("devo andare a %d\n", jump);

                } else if (c =='p')              ///quando si vuole stampare ci si muove senza poppare la pila redo;
                {
                    ////se hang è piu indietro si faranno redo pop sennò undo pop

                    ////dopo aver salvato la differenza mi muovo
                    if (jump > 0)
                        undopopper(&conf,&redo,&currline,jump,&timeline);
                    if (jump < 0)
                        redopopper(&conf,&redo,&currline,jump,&timeline);

                    hang = timeline;
                    ///printf("jump a zero\n");
                    flag = 0;
                    jump = 0;
                    strprocess(command, lines);
                    print(lines, currline, hash);
                }
                    ///      printf("%d hang\n",hang);
                    ///       printf("%d timeline\n",timeline);
                else if ((c=='c')||(c=='d'))
                {
                    if (jump > 0)
                        undopopper(&conf,&redo,&currline,jump,&timeline);
                    if (jump < 0)
                        redopopper(&conf,&redo,&currline,jump,&timeline);
                    hang = timeline;
                    ///printf("sono a %d\n", timeline);
                    flag = 0;
                }
                fgets(command, 1028, stdin);
                c = command[strlen(command) - 2];
            } while ((c == 'u') || (c == 'r') || (c == 'p'));
            //printf("%d\n",jump);
            timeline = 0;
            hang = 0;
            confflush(&redo);

            */ /////IN PROGRESSO*
        }
        if(c=='c')
        {
            strprocess(command,lines);
            if(lines[0]<currline)
            {
                int before;
                strprocess(command,lines);
                if (lines[1] < currline)
                {
                    before = lines[1] - lines[0] + 1;
                    cap++;
                    pushundochangeonly(&conf,lines,&currline);
                    b4( before, lines[0]);
                    //          printf("%s",conf->piece[0]->line);
                }
                else
                {
                    before=currline-lines[0];
                    pushundochangeappend(&conf,lines,&currline);
                    cap++;
                    b4(before,lines[0]);
                    scanna(&currline,lines[1]-currline+1);
                }
            }
            if(currline==lines[0])
            {
                cap++;
                int bro=currline;
                scanna(&currline,lines[1]-lines[0]+1);             //fare funzione
                pushundoappend(hash, &conf,lines,bro);


            }
        }
        if(c=='p')
        {
            strprocess(command,lines);
            print(lines,currline,hash);
        }
        if(c=='d')
        {
            strprocess(command,lines);
            if(lines[0]<currline)    ///se è una linea esistente
            {
                if(lines[0]==0)
                {
                    if(lines[1]==0)
                    {
                        pushuseless(&conf);
                        cap++;
                    }
                    else
                        lines[0]=1;
                }
                else
                if(lines[0]==1)  ////se è la prima
                {
                    if(lines[1]>currline-1)
                        lines[1]=currline-1;
                    if(lines[1]==currline-1)
                    {
                        pushundodeltail(&conf,lines,&currline);
                        cap++;
                        currline=1;
                    }
                    else   ////cancellazione in testa
                    {
                        savefrom(&conf,lines,&currline);
                        cap++;
                        reindex(hash,currline-lines[1]-1,lines[1]+1,lines[1]-lines[0]+1);
                        currline=currline-lines[1]-lines[0]+1;
                    }
                }
                else if(lines[0]==currline-1)    ////se è l'ultima
                {a
                    pushundodeltail(&conf,lines,&currline);
                    cap++;
                    currline=currline-1;
                }
                else if(lines[0]<currline-1) ////se non è l'ultima
                {
                    if(lines[1]>currline-1)
                        lines[1]=currline-1;
                    if(lines[1]==currline-1)
                    {
                        pushundodeltail(&conf,lines,&currline);
                        cap++;
                        currline=currline-lines[1]+lines[0]-1;
                    }
                    if(lines[1]<currline-1)    ///cancellazione in mezzo
                    {
                        savefrom(&conf,lines,&currline);
                        cap++;
                        reindex(hash,currline-lines[1]-1,lines[1]+1,lines[1]-lines[0]+1);

                        currline=currline-lines[1]+lines[0]-1;
                    }
                }
            }
            else
            {
                pushuseless(&conf);                    ////IN PROGRESSO
                ///printf("PUSHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n");
                cap++;
            }
        }
    }
    return 0;
}
