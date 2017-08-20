//
//
//
// Simple DP Solution without using Matrix operations

#include <stdio.h>

typedef long long int ll;

#define DEBUG 1
#define N_MAX 37
#define K_MAX (1000000 + 2)
#define S_MAX 11
#define MASK_MAX (1<<11)
#define MOD 1000000000

// MASK 0 ~ 9
int MASK_ALL = 0x03ff;
int N, TC, K, M;

char mat[N_MAX][N_MAX];
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

int input(){
  scanf ("%d %d %d", &N, &K, &M);
  for (int i=1; i <=N; i++){
    scanf("%s", mat[i] + 1);
  }
}


ll solve_sub(int dest){
  ll DP[2][N_MAX];
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

ll solve(){
  ll cnt = 0;
  for(int i=1; i <= N; i++)
    cnt = (cnt + solve_sub(i)) % M;

  return cnt;
}

// init the DP array
void init() {
  ;
}

int main()
{
#ifdef DEBUG
  freopen("H1721.txt", "r", stdin);
#endif
  scanf ("%d", &TC);
  for (int tc=1; tc <= TC; tc++){
    input();
    init();
    ll result = solve();
    printf ("#%d %lld\n", tc, result);
  }
}

