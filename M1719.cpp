//
// Created on 08/07/2017.
//
#include <stdio.h>
#include <iostream>
using namespace std;

#define N_MAX 21
#define MEM_MAX 16
#define DIR_MAX 4
#define INT_INF 2147483647
#define INT_IINF -2147483648

char mat[N_MAX][N_MAX];
bool state[N_MAX][N_MAX][MEM_MAX][DIR_MAX];
long long int year, month;
long long int year_new;
long long int month_new;

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

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

bool operate(char oper, int &mem, int &dir){
  if (oper == '<') dir = DIR_LEFT;
  else if (oper == '>') dir = DIR_RIGHT;
  else if (oper == '^') dir = DIR_UP;
  else if (oper == 'v') dir = DIR_DOWN;
  else if (oper == '_'){
    if (mem == 0)
      dir = DIR_RIGHT;
    else
      dir = DIR_LEFT;
  }
  else if (oper == '|'){
    if (mem == 0)
      dir = DIR_DOWN;
    else
      dir = DIR_UP;
  }
  else if (oper == '@')
    return true;
  else if (oper - '0' >= 0 && oper - '0' <= 9)
    mem = oper - '0';
  else if (oper == '+')
    mem++;
  else if (oper == '-')
    mem = (mem == 0) ? 15 : mem - 1;
  return false;
}

void move(int dir, int &row, int &col){
  if (dir == DIR_UP)
    row = (row == 0) ? year - 1 : row - 1;
  else if (dir == DIR_DOWN)
    row = (row == year - 1) ? 0 : row + 1;
  else if (dir == DIR_LEFT)
    col = (col == 0) ? month - 1 : col - 1;
  else if (dir == DIR_RIGHT)
    col = (col == month - 1) ? 0 : col + 1;
}

// solve function
bool isEnd(int mem, int dir, int row, int col){
  // init
  if (row == -1 && col == -1)
    row = col = 0;
  else
    move(dir, row, col);

  // doing loop
  while (!state[row][col][mem][dir]){
    state[row][col][mem][dir] = true;
    if (mat[row][col] == '?')
      return isEnd(mem, DIR_UP, row, col)
          || isEnd(mem, DIR_DOWN, row, col)
          || isEnd(mem, DIR_LEFT, row, col)
          || isEnd(mem, DIR_RIGHT, row, col);
    else{
      if (operate(mat[row][col], mem, dir) == true)
        return true;
    }
    move(dir, row, col);
  }
  return false;
}

bool solve(){
  long long int acc = (year - 2016) * 12 + month - 1;
  year_new = acc / 13 + 2016;
  month_new = acc % 13 + 1;
  return false;
}

int main(int argc, char** argv)
{
  int test_case;
  int T;
  freopen("M1719.txt", "r", stdin);
  scanf ("%d", &T);
  /*
  여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for (test_case = 1; test_case <= T; ++test_case)
  {
    /////////////////////////////////////////////////////////////////////////////////////////////
    scanf("%lld %lld", &year, &month);

    bool result = solve();
    printf ("#%lld %lld %d\n", test_case, year_new, month_new);
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

