#include <stdio.h>
int N, M, TC;

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
  scanf ("%d %d", &N, &M);

  for (int i=1; i <= N; i++){
    parent[i] = i;
  }

  for (int i=0; i < M; i++){
    int p1, p2;
    scanf ("%d %d", &p1, &p2);
    merge(p1, p2);
  }
}

int solve(){
  bool visit[N_MAX];

  // 01. init the visit
  for (int i =1; i <= N; i++)
    visit[i] = false;

  // 02. find and count roots
  int result = 0;
  for (int i =1; i <= N; i++){
    int root = find(i);
    if (visit[root] == false)
      result++;
    visit[root] = true;
  }

  return result;
}

int main()
{
//  freopen("M1721.txt", "r", stdin);
  scanf ("%d", &TC);
  for (int tc=1; tc <= TC; tc++){
    input();
    int result = solve();
    printf ("#%d %d\n", tc, result);
  }
}

