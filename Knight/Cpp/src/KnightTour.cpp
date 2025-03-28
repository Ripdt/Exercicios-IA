#include <iostream>
#include <conio.h>
#include <iomanip>
 
const int Ordem=12;
const int FIM=(Ordem-4) * (Ordem-4);
typedef int matriz[Ordem][Ordem];
 
void PasseioCavalo(matriz,int,int,int=1);
void inicializa(matriz);
void mostra(matriz);
 
int main()
{
  matriz Tabuleiro;
  inicializa(Tabuleiro);
 
  PasseioCavalo(Tabuleiro,2,2);
  mostra(Tabuleiro);
  return 0;
}

static bool continua = true;
static long cont = 0;

void PasseioCavalo(matriz Tab,int L, int C, int passo)
{
  cont++;
  if (Tab[L][C]==0) {
    Tab[L][C] = passo;
    if (passo==FIM){
      continua=false;
      std::cout << "Iterações: " << cont << std::endl;
      return;
    }
    if (continua)
      PasseioCavalo(Tab,L-2,C+1,passo+1);
    if (continua)
      PasseioCavalo(Tab,L-2,C-1,passo+1);
    if (continua)
      PasseioCavalo(Tab,L-1,C-2,passo+1);
    if (continua)
      PasseioCavalo(Tab,L+1,C-2,passo+1);
    if (continua)
      PasseioCavalo(Tab,L+2,C-1,passo+1);
    if (continua)
      PasseioCavalo(Tab,L+2,C+1,passo+1);
    if (continua)
      PasseioCavalo(Tab,L+1,C+2,passo+1);
    if (continua)
      PasseioCavalo(Tab,L-1,C+2,passo+1);
    if (continua)
      Tab[L][C] = 0;
  }
  return;
}
 
void inicializa(matriz Tab)
{
  for(int i=0;i<Ordem;i++)
    for(int j=0;j<Ordem;j++)
      if (i<2 || i>Ordem-3 || j<2 || j>Ordem-3)
        Tab[i][j]=-1;
      else
        Tab[i][j]=0;
}
 
void mostra(matriz Tab)
{
  for(int i=0;i<Ordem;i++) {
    for(int j=0;j<Ordem;j++)
      std::cout << std::setw(4) << Tab[i][j];
    std::cout << std::endl;
  }
}