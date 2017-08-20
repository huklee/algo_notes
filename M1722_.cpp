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
int N, TC;

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


ll solve(){
  // formula : N = s + ... + s + k -1
  //             = (s + (s + k - 1))*k/2
  //             = (2s + k - 1)*k/2
  // a = 2*n/k = 2s + k - 1
  // s = (a - k + 1)/2
  // k : 2 ~ sqr(2*N)

  ll res = 0, k=2, a;
  while (k*k < 2*N){
    if ((2*N)% k == 0){
      a = 2*N/k;
      if ((a - k + 1) % 2 == 0)
        res++;
    }
    k++;
  }

  return res + 1;
}

int input(){
  scanf ("%d", &N);
}

void init() {
  return;
}

int main()
{
#ifdef DEBUG
//  freopen("M1722.txt", "r", stdin);
#endif
  scanf ("%d", &TC);
  for (int tc=1; tc <= TC; tc++){
    input();
    init();
    ll result = solve();
    printf ("#%d %lld\n", tc, result);
  }
}


