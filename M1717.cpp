//
// Created on 21/06/2017.
//

#include<iostream>
using namespace std;

#define N_MAX 21
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

struct swarm {
  int x, y;
};
struct swarm S[N_MAX];

struct qelem {
  int x, y, index;
};
struct qelem queue[N_MAX];

void swap(int &a, int &b, int &c) { c = a; a = b; b = c; }

#define DIR_X 0
#define DIR_Y 1
#define QUEUE_START -1

// Standard Simple Radix Sort
int sort_temp[N_MAX];
int sort_visit[N_MAX];
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

// standard simple quickSort
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

ll generate_comb(int bits[], int N, int digit=0, int start=0){
  bool isStart = true;
  ll min_val=0;

  // calculation
  if (digit == N/2){
    ll sumx=0, sumy=0;
    for (int i=0; i < N; i++){
      if (bits[i]){
        sumx += S[i].x;
        sumy += S[i].y;
      }
      else{
        sumx -= S[i].x;
        sumy -= S[i].y;
      }
    }
    debug_print(bits, N);
    return sumx*sumx + sumy*sumy;
  }

  // recursive call for making combination
  for (int i=start; i <= N/2 + digit; i++){
    bits[i] = 1;
    ll cand = generate_comb(bits, N, digit + 1, i + 1);
    if (isStart == true){
      min_val = cand;
      isStart = false;
    }
    else
      min_val = get_min (min_val, cand);
    bits[i] = 0;
  }
  return min_val;
}

ll solve(int N){
  int bits[N_MAX];
  for (int i=0; i < N; i++)
    bits[i] = 0;
  return generate_comb(bits, N);
}

int main(int argc, char** argv)
{
  int test_case;
  int T;
  /*
  아래의 freopen 함수는 input.txt 를 read only 형식으로 연 후,
  앞으로 표준 입력(키보드) 대신 input.txt 파일로부터 읽어오겠다는 의미의 코드입니다.
  //여러분이 작성한 코드를 테스트 할 때, 편의를 위해서 input.txt에 입력을 저장한 후,
  freopen 함수를 이용하면 이후 cin 을 수행할 때 표준 입력 대신 파일로부터 입력을 받아올 수 있습니다.
  따라서 테스트를 수행할 때에는 아래 주석을 지우고 이 함수를 사용하셔도 좋습니다.
  freopen 함수를 사용하기 위해서는 #include <cstdio>, 혹은 #include <stdio.h> 가 필요합니다.
  단, 채점을 위해 코드를 제출하실 때에는 반드시 freopen 함수를 지우거나 주석 처리 하셔야 합니다.
  */
  freopen("M1717.txt", "r", stdin);
  cin >> T;
  /*
  여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for (test_case = 1; test_case <= T; ++test_case)
  {
    /////////////////////////////////////////////////////////////////////////////////////////////
    int N;
    cin >> N;

    for (int i = 0; i < N; i++) {
      int xx, yy; cin >> xx >> yy;
      S[i].x = xx;
      S[i].y = yy;
    }
    ll result = solve(N);
    cout << "#" << test_case << " " << result << endl;
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}
