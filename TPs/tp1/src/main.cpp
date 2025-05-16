typedef struct sortperf{
    int cmp;
    int move;
    int calls;
  } sortperf_t;
  
  void resetcounter(sortperf_t * s){
  // Descricao: inicializa estrutura
  // Entrada: 
  // Saida: s
    s->cmp = 0;
    s->move = 0;
    s->calls = 0;
  }
  
  void inccmp(sortperf_t * s, int num){
  // Descricao: incrementa o numero de comparacoes em num 
  // Entrada: s, num
  // Saida: s
    s->cmp += num;
  }
  
  void incmove(sortperf_t * s, int num){
  // Descricao: incrementa o numero de movimentacoes de dados em num 
  // Entrada: s, num
  // Saida: s
    s->move += num;
  }
  
  void inccalls(sortperf_t * s, int num){
  // Descricao: incrementa o numero de chamadas de função em num 
  // Entrada: s, num
  // Saida: s
    s->calls += num;
  }
//insertion sort
/* void insertionSort(int v[], int l, int r, sortperf_t * s) {
  inccalls(s,1);
  int i,j,aux;
  for(i=l+1;i<=r;i++){
    
    aux = v[i];
    incmove(s,1);

    j = i - 1;

    while(j>=l && aux < v[j]){
      inccmp(s,1);
      v[j+1] = v[j];
      incmove(s,1);
      j--;
    }
    inccmp(s,1);
    
    v[j+1] = aux;
    incmove(s,1);
  }
  return;
} */

// standard quicksort partition
/* void partition(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s,1);
int p;
*i = l; *j = r;
p = A[(*i+*j)/2];
do{
  while(p > A[*i]){(*i)++;inccmp(s,1);}
  inccmp(s,1);
  while(p < A[*j]){(*j)--;inccmp(s,1);}
  inccmp(s,1);
  if(*i<=*j){
    swap(&A[*i],&A[*j],s);
    (*i)++;
    (*j)--;
  }
}
while(*i<=*j);
}

// standard quicksort
void quickSort(int * A, int l, int r, sortperf_t *s) { 
inccalls(s,1);
int i,j;
partition(A,l,r,&i,&j,s);
if(l<j){quickSort(A,l,j,s);}
if(r>i){quickSort(A,i,r,s);}
}
 */