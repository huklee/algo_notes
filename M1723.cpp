//
#include <stdio.h>

typedef long long int ll;

#define DEBUG 0
#define N_MAX 1000002

// MASK 0 ~ 9
int MASK_ALL = 0x03ff;
ll N, TC, K, M, S, E;

char mat[N_MAX][N_MAX];
ll DP[2][N_MAX];
void init();


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


int input(){
  scanf ("%lld %lld %lld", &S, &E, &M);
}


ll solve(){
  // formula : CRT (The Chinese Remainder Theorem)
  // S = r1, E = r2, M = r3
  // m1 = lcm(S,E,M)/r1
  // m2 = lcm(S,E,M)/r2
  // m3 = lcm(S,E,M)/r3
  // x = (s1 * m1 * inv(m1 mod r1)
  //    + s2 * m2 * inv(m2 mod r2)
  //    + s3 * m3 * inv(m3 mod r3)) mod lcm (S,E,M)

  ll res;
  res = (S*696*161 + E*10585*1 + M*8760*15)%(365*24*29);
  if (res == 0)
    res = 365*24*29;
  return res;
}

void init() {
  return;
}

int main()
{
#ifdef DEBUG
  freopen("M1723.txt", "r", stdin);
#endif
  scanf ("%d", &TC);
  for (int tc=1; tc <= TC; tc++){
    input();
    init();
    ll result = solve();
    printf ("#%d %lld\n", tc, result);
  }
}


