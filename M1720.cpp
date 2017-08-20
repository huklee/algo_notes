//
// Created on 18/07/2017.
//
//
#include <stdio.h>
#include <iostream>
using namespace std;

#define N_MAX 2001
#define P_MAX 4001
#define MEM_MAX 16
#define DIR_MAX 4
#define INT_INF 2147483647
#define INT_IINF -2147483648

char mat[N_MAX][N_MAX];
char result[N_MAX*2];
long long int year, month;
long long int year_new;
long long int month_new;

typedef long long ll;

bool stack1[P_MAX];
bool stack2[P_MAX];

int _strlen(char str[]) {
  int res = 0;
  while (true) {
    if (str[res] == 0) break;
    res++;
  }
  return res;
}

#define MIN(a,b) (a)<(b) ? (a) : (b)
#define MAX(a,b) (a)>(b) ? (a) : (b)
char get_min(char a, char b) { return a > b ? b : a; }
int get_min(int a, int b) { return a > b ? b : a; }
int get_max(int a, int b) { return a > b ? a : b; }
ll get_min(ll a, ll b) { return a > b ? b : a; }
ll get_max(ll a, ll b) { return a > b ? a : b; }
double get_min(double a, double b) { return a > b ? b : a; }
double get_max(double a, double b) { return a > b ? a : b; }

typedef struct qelem{
  int x, y, index;
};
int N;
int M;
int weight[N_MAX];
qelem queue[N_MAX];

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

bool visit[N_MAX][N_MAX];
typedef struct point{
  int row; int col;
};
point q[N_MAX*2];
int front = 0; int rear = 0;
void enqueue(point p){
  q[front] = p;
  front = (front + 1)%N_MAX;
}
point dequeue(){
  point p = q[rear];
  rear = (rear + 1)%N_MAX;
  return p;
}

int dir_row[2] = {0, 1};
int dir_col[2] = {1, 0};

point cand[N_MAX*2];
int cand_cnt = 0;

// initialization for visit & queue status
void init(){
  for (int i =0; i < N; i++){
    for (int j=0; j < M; j++){
      visit[i][j] = false;
    }
  }
  front = rear = 0;
}

// 각 방향에 대해서 탐색 & 가능한 경우 cand 추가
void add_cand(point p, int dir){
  point new_p = p;
  new_p.row += dir_row[dir];
  new_p.col += dir_col[dir];

  if (new_p.row >= N || new_p.col >= M) return;
  if (visit[new_p.row][new_p.col] == true) return;
  visit[new_p.row][new_p.col] = true;
  cand[cand_cnt++] = new_p;
}

// Solution 2 : using 1 queue & 1 cand array
int _solve(){
  init();
  int answer_size = N + M - 1;
  point p = {0,0};
  enqueue(p);
  result[0] = mat[0][0];

  // answer size만큼 탐색
  for (int l=1; l < answer_size; l++){
    cand_cnt = 0;

    // 모든 queue에 대해서 탐색
    while (front != rear){
      p = dequeue();
      for (int dir=0; dir < 2; dir++)
        add_cand(p, dir);
    }

    // pick the minimum
    char min_char = 'z';
    for (int i = 0; i < cand_cnt; i++)
      min_char = MIN(min_char, mat[cand[i].row][cand[i].col]);
    result[l] = min_char;

    // enqueue only the points of minimum values
    for (int i = 0; i < cand_cnt; i++){
      if (min_char == mat[cand[i].row][cand[i].col]){
        enqueue(cand[i]);
      }
    }
  }

  // mark the end of the result as null(0)
  result[answer_size] = 0;
}

// Solution 1 : using 2 stack
int solve(){
  char min_c[N_MAX*2];
  // initialization
  for (int i=0; i < N_MAX*2; i++)
    min_c[i] = 'z';

  bool *cur, *next;
  stack1[0] = true;
  min_c[0] = mat[0][0];
  int step = 0;

  while (true){
    // iteration end case
    if (step >= N + M - 1) break;

    // initial the next step's stack
    if (step % 2 == 0){
      cur = (bool *) &stack1;
      next = (bool *) &stack2;
    }
    else{
      cur = (bool*)&stack2;
      next = (bool*)&stack1;
    }

    for (int i=0; i < P_MAX; i++) next[i] = false;

    // find the minimum
    for (int i=0; i < step + 1; i++){
      int _i = i;
      int _j = step - i;

      // edge cases
      if (_i >= N)  continue;
      if (_j >= M)  continue;

      char _c = mat[_i][_j];
      if (cur[i] == true)
        min_c[step] = get_min(min_c[step], _c);
    }

    // trim the branches
    for (int i=0; i < step + 1; i++){
      int _i = i;
      int _j = step - i;

      // edge cases
      if (_i >= N)  continue;
      if (_j >= M)  continue;

      char _c = mat[_i][_j];
      if (min_c[step] != _c)
        cur[i] = false;
    }

    // BFS search for each stack
    for (int i=0; i < step + 1; i++){
      int _i = i;
      int _j = step - i;

      // edge cases
      if (_i >= N)  continue;
      if (_j >= M)  continue;

      char _c = mat[_i][_j];
      if (cur[i] == true){
        min_c[step] = get_min(min_c[step], _c);
        next[i] = true;
        next[i + 1] = true;
      }
    }

    step++;
  }

  int i;
  for (i=0; i < N + M - 1; i++) {
    result[i] = min_c[i];
  }
  result[i] = 0;
}

int main(int argc, char** argv)
{
  int test_case;
  int T;
//  freopen("M1720.txt", "r", stdin);
  scanf ("%d", &T);
  /*
  여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for (test_case = 1; test_case <= T; ++test_case)
  {
    /////////////////////////////////////////////////////////////////////////////////////////////
    scanf ("%d %d", &N, &M);
    for (int i=0; i < N; i++)
        scanf ("%s", &mat[i]);

    _solve();
    printf ("#%d %s\n", test_case, result);
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

