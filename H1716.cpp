#include<iostream>
using namespace std;

#define N_MAX 100001

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

struct light {
  int x, y, val;
};
struct light L[N_MAX];

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

int getVal(int index, int dir) {
  if (dir == DIR_X)
    return L[index].x;
  else if (dir == DIR_Y)
    return L[index].y;
  return -1;
}

int get_level(int index) {
  return L[index].val;
}

void radixSort(int arr[], int start, int end, int dir) {
  // initialization : find the max digit
  int max_num = -1, max_digit = 0;
  for (int i = start; i <= end; i++)
    max_num = get_max(max_num, getVal(arr[i], dir));

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
        while (!sort_visit[i] && (getVal(arr[i], dir)/digit_unit)%10 < cur_digit) {
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
  if (getVal(arr[start], dir) == getVal(arr[end], dir)) {
    bool pass = true;
    for (int i = start + 1; i <= end; i++) {
      if (getVal(arr[start], dir) != getVal(arr[i], dir)) {
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
    while (left < right && getVal(arr[left], dir) < getVal(pivot, dir)) left++;
    while (left < right && getVal(pivot, dir) <= getVal(arr[right], dir)) right--;
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
    if (getVal(arr[mid], dir) == x)
      return mid;
    else if (getVal(arr[mid], dir) < x)
      left = mid;
    else
      right = mid;
  }
  if (getVal(arr[left], dir) == x)  return left;
  if (getVal(arr[right], dir) == x) return right;
  return -1;
}

int get_val(int index) { return L[index].val; }


int solve(int N, int xl, int yl, int xb, int yb) {
  // 01. generate the sorted x & y List
  radixSort(xList, 0, N - 1, DIR_X);
  radixSort(yList, 0, N - 1, DIR_Y);

  // 02. do the dfs
  int q_cnt = 1;
  int q_cur = 0;
  int sx, sy;
  int level = 0;

  queue[0].x = xl;
  queue[0].y = yl;
  queue[0].index = -1;

  //  cout << "==START== ";
  do {
    //    cout << q_cur << " : " << queue[q_cur].x << "," << queue[q_cur].y << ":"
    //         << (queue[q_cnt].index == -1 ? -1 : L[queue[q_cnt].index].val)
    //         << endl;

    int cand, cand_max, cand_min;
    level = get_level(queue[q_cur].index);
    sx = queue[q_cur].x;
    sy = queue[q_cur].y;

    if (sx == xb || sy == yb) {
      //      cout << "== END ==" << endl;
      return level;
    }

    // for x Axis
    cand = cand_max = cand_min = findBS(xList, N, sx, DIR_X);
    while (cand_max < N - 1 && getVal(xList[cand_max + 1], DIR_X) == sx) cand_max++;
    while (cand_min > 0 && getVal(xList[cand_min - 1], DIR_X) == sx) cand_min--;

    if (cand_min != -1) {
      for (int i = cand_min; i <= cand_max; i++) {
        int real_index = xList[i];
        // visited case
        if (get_level(real_index) != 0)  continue;

        // enqueue
        queue[q_cnt].x = L[real_index].x;
        queue[q_cnt].y = L[real_index].y;
        queue[q_cnt].index = real_index;
        q_cnt++;

        L[real_index].val = level + 1;
      }
    }

    // for y Axis
    cand = cand_max = cand_min = findBS(yList, N, sy, DIR_Y);
    while (cand_max < N - 1 && getVal(yList[cand_max + 1], DIR_Y) == sy) cand_max++;
    while (cand_min > 0 && getVal(yList[cand_min - 1], DIR_Y) == sy) cand_min--;

    if (cand_min != -1) {
      for (int i = cand_min; i <= cand_max; i++) {
        int real_index = yList[i];
        // visited case
        if (get_level(real_index) != 0)  continue;

        // enqueue
        queue[q_cnt].x = L[real_index].x;
        queue[q_cnt].y = L[real_index].y;
        queue[q_cnt].index = real_index;
        q_cnt++;

        L[real_index].val = level + 1;
      }
    }
    q_cur++;
  } while (q_cur < q_cnt);

  return -1;
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
  freopen("H1716.txt", "r", stdin);
  cin >> T;
  /*
  여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for (test_case = 1; test_case <= T; ++test_case)
  {
    //    memset(L, 0, N_MAX*sizeof(struct light));
    for (int i = 0; i<N_MAX; i++) {
      L[i].x = 0;
      L[i].y = 0;
      L[i].val = 0;
    }

    int N, xl, yl, xb, yb;
    /////////////////////////////////////////////////////////////////////////////////////////////
    cin >> N >> xl >> yl >> xb >> yb;

    for (int i = 0; i < N; i++) {
      int xx, yy; cin >> xx >> yy;
      L[i].x = xx;
      L[i].y = yy;
      xList[i] = i;
      yList[i] = i;
    }
    int result = solve(N, xl, yl, xb, yb);
    cout << "#" << test_case << " " << result << endl;
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}