//

#include <stdio.h>

typedef long long int ll;

#define DEBUG 0
#define N_MAX 1000002
#define S_MAX 11
#define MASK_MAX (1<<11)
#define MOD 1000000000

// MASK 0 ~ 9
int MASK_ALL = 0x03ff;
ll N, TC, K, M;

char mat[N_MAX][N_MAX];
ll DP[2][N_MAX];
void init();

#define Q_MAX 2000

typedef struct MEETING{
  int s, e, next;
};

typedef struct QUERY{
  int s, e;
};

// MEETING meet[N_MAX];
// QUERY query[Q_MAX];

int parent[N_MAX];


int comp (MEETING a, MEETING b){
  if (a.e == b.e)
    return a.s - b.s;
  else
    return a.e - b.e;
}

template <typename T>
void swap (T &a, T &b){ T temp = a ; a = b; b = temp; }


template <typename T>
void quickSort(T arr[], int start, int end){
  if (start >= end) return;
  int left = start, right = end;
  T temp = arr[right];

  while (left < right){
    while (left < right && comp(arr[left], temp) <  0) left++;
    while (left < right && comp(temp, arr[right]) <= 0) right--;
    if (left < right) swap(arr[left], arr[right]);
  }
  arr[right] = temp;
  quickSort (arr, start, right - 1);
  quickSort (arr, right + 1, end);
}

// TODO : verify
int find (int p){
  while (p != parent[p])
    p = parent[p];
  return p;
}

// TODO : veirfy
void merge(int c1, int c2){
  int p1 = find(c1), p2 = find(c2);
  if (p1 > p2)
    parent[p1] = p2;
  else
    parent[p2] = p1;
}

// DP solution 1
ll _solve_sub(int dest){
  ll result = 0;
  // 01. generate the DP table
  int cur=0, prev=1;
  for(int j=K; j >= 1; j--){
    cur = prev;
    prev = 1^cur;
    for(int i=1; i <= N; i++){

      // base case
      if (j == K){
        if (i == dest)
          DP[cur][i] = 1;
        else
          DP[cur][i] = 0;
        continue;
      }

      // calculate the DP formula
      ll sum = 0;
      for(int a=1; a <= N; a++){
        if (mat[i][a] == 'Y')
          sum = (sum + DP[prev][a]) % M;
      }
      DP[cur][i] = sum;

      // 02. find the solution starting start == dest
      if (i == dest)
        result = (result + sum) % M;
    }
  }
  return result;
}

ll _solve(){
  ll cnt = 0;
  for(int i=1; i <= N; i++)
    cnt = (cnt + _solve_sub(i)) % M;

  return cnt;
}

////////////////////////////////////////////
// Matrix class for Matrix operation
typedef struct Matrix{
  int len;
  ll arr[N_MAX][N_MAX];
};

Matrix mat_ori;
void init_mat(Matrix &matrix);

int input(){
  scanf ("%d", &N);
}

bool is_power_two(int a){
  if ((a&(a - 1)) == 0)
    return true;
  else
    return false;
}

#define POWER_MAX 20
Matrix memo[POWER_MAX];

// TODO : verify
// Dest = A+B
void add (Matrix A, Matrix B, Matrix& Dest){
  // assert that A.len == B.len
  int len = A.len;
  Dest.len = len;
  for (int i = 0; i < len; i++){
    for (int j = 0; j < len; j++){
      Dest.arr[i][j] = (A.arr[i][j] + B.arr[i][j]) % M;
    }
  }
}

// TODO : verify
// Dest = Dest+A
void add (Matrix A, Matrix& Dest){
  Dest.len = A.len;
  for (int i = 0; i < A.len; i++){
    for (int j = 0; j < A.len; j++){
      Dest.arr[i][j] = (Dest.arr[i][j] + A.arr[i][j]) % M;
    }
  }
}

// TODO : verify
// Dest = A*B
void product (Matrix A, Matrix B, Matrix& Dest){
  // assert that A.len == B.len
  int len = A.len;
  Dest.len = len;
  for (int i = 0; i < len; i++){
    for (int j = 0; j < len; j++){
      ll sum = 0;
      for (int k = 0; k < len; k++)
        sum = (sum + A.arr[i][k] * B.arr[k][j]) % M;
      Dest.arr[i][j] = sum;
    }
  }
}

// TODO : verify
// A = A + 1
void add_eigen (Matrix &A){
  int len = A.len;
  for (int i = 0; i < A.len; i++){
    A.arr[i][i] += 1;
  }
}

// TODO : verify
// Dest = A
void copy(Matrix A, Matrix& Dest){
  Dest.len = A.len;
  for (int i = 0; i < A.len; i++){
    for (int j = 0; j < A.len; j++){
      Dest.arr[i][j] = A.arr[i][j];
    }
  }
}

// A = An
void getA(Matrix& Dest, int n){
  // using memo
  if (is_power_two(n)){
    // n = 2^k
    int k = 0, acc = 1;
    while(acc < n){
      k ++; acc <<= 1;
    }
    copy(memo[k], Dest);

#if DEBUG == 1
    printf ("=== A[%d] ===\n", n);
    for (int i = 0; i < Dest.len; i++){
      for (int j = 0; j < Dest.len; j++){
        printf ("%d ", Dest.arr[i][j]);
      }
      printf("\n");
    }
    printf("\n");
#endif

    return;
  }

  Matrix res, A, B;
  int k = 1, acc = 0;

  // 01. find the max K
  while(k*2 < n)
    k *= 2;

  // 02. add the small components
  //    until the accumulated number reaches to n
  while (acc < n){
    if (k + acc <= n){
      getA(A, k);
      if (acc == 0)
        copy(A, res);
      else{
        copy(res, B);
        product(A, B, res);
      }
      acc += k;
    }
    else
      k /= 2;
  }
  copy(res, Dest);

#if DEBUG == 1
  printf ("=== A[%d] ===\n", n);
  for (int i = 0; i < Dest.len; i++){
    for (int j = 0; j < Dest.len; j++){
      printf ("%d ", Dest.arr[i][j]);
    }
    printf("\n");
  }
  printf("\n");
#endif
}


// A = Sn
void getS(Matrix& Dest, int n){
  Matrix res, A, B, sub_res;
  init_mat(res); init_mat(B); init_mat(sub_res);
  // Base case
  if (n == 1){
    getA(Dest, 1);

#if DEBUG == 1
    printf ("=== S[%d] ===\n", n);
    for (int i = 0; i < Dest.len; i++){
      for (int j = 0; j < Dest.len; j++){
        printf ("%d ", Dest.arr[i][j]);
      }
      printf("\n");
    }
    printf("\n");
#endif

    return;
  }

  int remain = n;
  while (remain > 0){
    // S2n = (An + 1)*Sn
    if (remain % 2 == 0){
      getA(A, remain/2);
      add_eigen(A);
      getS(B, remain/2);
      product(A, B, sub_res);
      add(sub_res, res, res);
      break;
    }

      // Sn = An + Sn-1
    else{
      getA(A, remain);
      add(A, res);
      remain -= 1;
    }
  }
  copy(res, Dest);

#if DEBUG == 1
  printf ("=== S[%d] ===\n", n);
  for (int i = 0; i < Dest.len; i++){
    for (int j = 0; j < Dest.len; j++){
      printf ("%d ", Dest.arr[i][j]);
    }
    printf("\n");
  }
  printf("\n");
#endif

  return;
}

void init_mat(Matrix &matrix) {
  matrix.len = N;
  for (int i = 0; i < N; i++){
    for (int j =0; j < N; j++){
      matrix.arr[i][j] = 0;

    }
  }
}

ll solve(){
  // formula : N = s + ... + s + k -1
  //             = (s + (s + k - 1))*k/2
  //             = (2s + k - 1)*k/2
  // a = 2*n/k = 2s + k - 1
  // s = (a - k + 1)/2
  // k : 2 ~ sqr(2*N)

  ll res = 0, k=2, a;
  while (k*k < 2*N){
    if (2*N/k == 0){
      a = 2*N/k;
      if ((a - k + 1) % 2 == 0)
        res++;
    }
    k++;
  }

  return res;
}

void init() {
  return;
}

int main()
{
#ifdef DEBUG
  freopen("M1722.txt", "r", stdin);
#endif
  scanf ("%d", &TC);
  for (int tc=1; tc <= TC; tc++){
    input();
    init();
    ll result = solve();
    printf ("#%d %lld\n", tc, result);
  }
}


