//
// Created on 21/06/2017.

#include <stdio.h>
#include <iostream>
using namespace std;

#define N_MAX 51
#define INT_INF 2147483647
#define INT_IINF -2147483648
typedef long long ll;

int _strlen(char str[]) {
  int res = 0;
  while (true) {
    if (str[res] == 0) break;
    res++;
  }
  return res;
}

int get_min(int a, int b) { return a > b ? b : a; }
int get_max(int a, int b) { return a > b ? a : b; }
ll get_min(ll a, ll b) { return a > b ? b : a; }
ll get_max(ll a, ll b) { return a > b ? a : b; }
double get_min(double a, double b) { return a > b ? b : a; }
double get_max(double a, double b) { return a > b ? a : b; }

int C[N_MAX];

struct qelem {
  int x, y, index;
};
struct qelem queue[N_MAX];

int xList[N_MAX];
int yList[N_MAX];
int sort_temp[N_MAX];
int sort_visit[N_MAX];


void swap(int &a, int &b, int &c) { c = a; a = b; b = c; }

#define DIR_X 0
#define DIR_Y 1
#define QUEUE_START -1


void radixSort(int arr[], int start, int end, int dir) {
  // initialization : find the max digit
  int max_num = -1, max_digit = 0;
  for (int i = start; i <= end; i++)
    max_num = get_max(max_num, arr[i]);

  while (max_num != 0) {
    max_digit++;
    max_num /= 10;
  }

  // iteration for each digits
  int digit_unit = 1;
  for (int digit = 1; digit <= max_digit; digit++) {
    for (int i = start; i <= end; i++)
      sort_visit[i] = false;

    int temp_index = 0, arr_index = 0;
    for (int cur_digit = 1; cur_digit <= 10; cur_digit++) {
      for (int i = start; i <= end; i++) {
        while (!sort_visit[i] && (arr[i]/digit_unit)%10 < cur_digit) {
          sort_temp[temp_index++] = arr[i];
          sort_visit[i] = true;
        }
      }
    }

    for (int i = start; i <= end; i++)
      arr[i] = sort_temp[i];
    digit_unit *= 10;
  }
}

int quickSort(int arr[], int start, int end, int dir, int depth = 0) {
  if (start >= end) return depth;

  // for all same number check
  if (arr[start] == arr[end]) {
    bool pass = true;
    for (int i = start + 1; i <= end; i++) {
      if (arr[start] != arr[i]) {
        pass = false;
        break;
      }
    }
    if (pass == true)    return depth;
  }

  int left = start;
  int right = end;
  int pivot = arr[start];
  int temp;
  while (left < right) {
    while (left < right && arr[left] < pivot) left++;
    while (left < right && pivot <= arr[right]) right--;
    if (left < right) swap(arr[left], arr[right], temp);
  }
  arr[right] = pivot;
  int d1 = quickSort(arr, start, right - 1, dir, depth + 1);
  int d2 = quickSort(arr, right + 1, end, dir, depth + 1);
  return get_max(d1, d2);
}

void _quickSort(int arr[], int start, int end) {
  if (start >= end) return;

  int left = start;
  int right = end;
  int pivot = arr[start];
  int temp;
  while (left < right) {
    while (left < right && arr[left] < pivot) left++;
    while (left < right && pivot <= arr[right]) right--;
    if (left < right) swap(arr[left], arr[right], temp);
  }
  arr[right] = pivot;
  _quickSort(arr, start, right - 1);
  _quickSort(arr, right + 1, end);
}


int findBS(int arr[], int N, int x, int dir) {
  int left = 0, right = N - 1;
  while (right - left > 1) {
    int mid = (left + right) / 2;
    if (arr[mid] == x)
      return mid;
    else if (arr[mid] < x)
      left = mid;
    else
      right = mid;
  }
  if (arr[left] == x)  return left;
  if (arr[right] == x) return right;
  return -1;
}


void debug_print(int bits[], int N){
  return;
}

double solve(int N, int M){
  double min_val = INT_INF;
  double max_val = INT_IINF;
  double div[N_MAX];
  double piece[N_MAX];

  // init
  for(int i=0; i < N; i++) {
    div[i] = 1;
    piece[i] = C[i];
    max_val = get_max(max_val, piece[i]);
    min_val = get_min(min_val, piece[i]);
  }
  double min_diff = max_val - min_val;

  // iterate for each try
  for(int t=0; t < M; t++){
    if (min_diff == 0)  break;

    // pick max index
    int max_i = 0, _max_val = INT_IINF;
    for(int i=0; i < N; i++){
      if (piece[i] > piece[max_i] || (piece[i] == piece[max_i] && div[i] < div[max_i])){
        _max_val = piece[i];
        max_i = i;
      }
    }

    // divide
    double new_val = (double)C[max_i]/(double)++div[max_i];
    piece[max_i] = new_val;

    // update the min diff
    max_val = INT_IINF;
    for(int i=0; i < N; i++) max_val = get_max(max_val, piece[i]);
    min_val = get_min(min_val, new_val);
    min_diff = get_min(min_diff, max_val - min_val);
  }
  return min_diff;
}

int main(int argc, char** argv)
{
  int test_case;
  int T;
   freopen("H1717.txt", "r", stdin);
  scanf ("%d", &T);
  /*
  여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for (test_case = 1; test_case <= T; ++test_case)
  {
    /////////////////////////////////////////////////////////////////////////////////////////////
    int N;
    cin >> N;

    for (int i = 0; i < N; i++) {
      int xx; cin >> xx;
      C[i] = xx;
    }
    int M;
    cin >> M;

    double result = solve(N, M);
    printf ("#%d %.10f\n", test_case, result);
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

