//
//

#include <stdio.h>
int N, TC;

typedef long long int ll;

//#define DEBUG 2
#define N_MAX 101
#define S_MAX 11
#define MASK_MAX (1<<11)
#define MOD 1000000000

// MASK 0 ~ 9
int MASK_ALL = 0x03ff;
int memo[S_MAX][N_MAX][MASK_MAX];
void init();

#define N_MAX 102
#define Q_MAX 2000

typedef struct MEETING{
  int s, e, next;
};

typedef struct QUERY{
  int s, e;
};

MEETING meet[N_MAX];
QUERY query[Q_MAX];

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
  scanf ("%d", &N);
}

// get the all possiblities with startNumber / #ofNumbers / mask
ll getPossibilties(int s, int n, int mask) {
  // memoization
  if (memo[s][n][mask] != -1)
    return memo[s][n][mask];

  // base case
  if (n == 1){
    if (mask == MASK_ALL)
      return 1;
    else
      return 0;
  }

  // recursive calculation call
  ll cnt = 0;
  if (s != 9)
    cnt = (cnt + getPossibilties(s + 1, n - 1, mask | (1 << (s + 1)))) % MOD;
  if (s != 0)
    cnt = (cnt + getPossibilties(s - 1, n - 1, mask | (1 << (s - 1)))) % MOD;

#if DEBUG == 1
  printf ("%d %d %d : %d\n", s, n, mask, cnt);
#endif
  memo[s][n][mask] = cnt;
  return cnt;
}

ll solve(){
  ll cnt = 0;
  // start number can be 1 ~ 9
  for (int start=1; start < 10; start++)
    cnt = (cnt + getPossibilties(start, N, 1 << start)) % MOD;
  return cnt;
}

// init the memo array
void init() {
  for(int i=0; i < 10; i++){
    for(int j=0; j <= 100; j++){
      for(int mask=0; mask<(1 << 10); mask++) {
        memo[i][j][mask] = -1;
      }
    }
  }
}

int main()
{
#ifdef DEBUG
  freopen("MH1720.txt", "r", stdin);
#endif
  scanf ("%d", &TC);
  init();
  for (int tc=1; tc <= TC; tc++){
    input();
    ll result = solve();
    printf ("#%d %lld\n", tc, result);
  }
}


