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

// quicksort with insertion for small partitions and median of 3
/* void quickSort3Ins(int * A, int l, int r, sortperf_t *s) {
  inccalls(s,1);
  int i,j;
  partition3(A,l,r,&i,&j,s);
  
  if(l<j){
    if (j - l <= 50) {
      insertionSort(A, l, j,s);
    }
    else{quickSort3Ins(A,l,j,s);}
  }
  
  if(r>i){
    if (r - i <= 50) {
      insertionSort(A, i, r,s);
    }
    else{quickSort3Ins(A,i,r,s);
    }
  } 
} */

// quicksort partition using median of 3
/* void partition3(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s,1);
  *i = l; *j = r;
  int a = A[l];
  int b = A[(l + r) / 2];
  int c = A[r];
  int p = median(a, b, c);

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
} */

/* // median of 3 integers
int median (int a, int b, int c) {
  if ((a <= b) && (b <= c)) return b;  // a b c
  if ((a <= c) && (c <= b)) return c;  // a c b
  if ((b <= a) && (a <= c)) return a;  // b a c
  if ((b <= c) && (c <= a)) return c;  // b c a
  if ((c <= a) && (a <= b)) return a;  // c a b
  return b;                            // c b a
} */