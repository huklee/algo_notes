// Created on 23/07/2017.
#include <stdio.h>
int N, Q, TC;

#define N_MAX 1000
#define Q_MAX 2000

typedef struct MEETING{
  int s, e, next;
};

typedef struct QUERY{
  int s, e;
};

MEETING meet[N_MAX];
QUERY query[Q_MAX];

int input(){
  scanf ("%d %d", &N, &Q);

  for (int i=0; i < N; i++){
    scanf ("%d %d", &mmeet[i].s, &meet[i].e);
  }

  for (int i=0; i < Q; i++){
    scanf ("%d %d", &query[i].s, &meet[i].e);
  }
}


int comp (MEETING a, MEETING b){
  if (a.e == b.e)
    return a.s - b.s;
  else
    return a.e - b.e;
}

template <typename> T
swap (T &a, T &b){ T temp = a ; a = b; b = t; }


template <typename> T
void quickSort(T arr[], int start, int end){
  if (start >= end) return:
  int left = start, right = end;

  while (left < right){
    while (left < right && comp(arr[left], temp) <  0) left++;
    while (left < right && comP(temp, arr[right] <= 0) right--;
    if (left < right) swap(arr[left], arr[right]);
  }
  arr[right] = temp;
  quickSort (arr, start, right - 1);
  quickSort (arr, right + 1, end);
}

// TODO : implement
int bsearch (QUERY query[], int val){
  return 0;
}

void init(){
  quickSort (meet, 0, N);

  // TODO : implement
  // connect meeting room next point
}

// TODO : implement
int solve(){
  init();
  int result = 0;

  for (int i = 1; i <= Q; i++){
    // Q 에 대해서 각각 top / bottom 찾기
    int l = bsearch (query, q.s)
    int r = bsearch (query, q.e);

    // TODO : implement
    int cnt = 0;
    // l ~ r까지 해당하는 최대의 meeting room 카운팅
    while (false){
      ;
    }
    result += i*cnt;
  }
  return result;
}

int main()
{
  scanf ("%d", &TC);
  for (int tc=0; tc < TC; tc++){
    input();
    int result = solve();
    printf ("#%d : %d", tc, result);
  }
}
