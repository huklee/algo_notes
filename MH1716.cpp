/////////////////////////////////////////////////////////////////////////////////////////////
// 2017-06-13 : Too slow - #8 에서 Time Over ~ stage hash table : bad

#include<iostream>
using namespace std;

#define N_MAX 1000000001

int _strlen (char str[]){
  int res = 0;
  while(true){
    if (str[res] == 0) break;
    res++;
  }
  return res;
}

int s, t, a, b;
bool candUp[N_MAX] = {0,};
bool candDown[N_MAX] = {0,};

int get_min (int a, int b){ return a > b ? b : a; }
int get_max (int a, int b){ return a > b ? a : b; }

int solve(int s, int t, int a, int b){
  // init
  memset(candUp, sizeof(bool)*N_MAX, sizeof(bool));
  memset(candDown, sizeof(bool)*N_MAX, sizeof(bool));

  int min_up = t;
  int max_up = t;
  int min_down = s;
  int max_down = s;

  int new_max=0;
  int result = -1;
  int cnt = 0;
  candUp[t] = true;
  candDown[s] = true;

  // exception case : s == t
  if (s == t) return 0;

  while (true){
    if (max_up < min_down)  break;

    int gap_up = max_up - min_up;
    int gap_down = max_down - min_down;

    if (gap_up < gap_down) {
      // step up for up Stage
      int min = N_MAX;
      int max = -1;
      for (int i=min_up; i <= max_up; i++){
        if (candUp[i] == false) continue;
        candUp[i] = false;
        if (candDown[i] == true)
          return cnt;

        if (i - a >= 0){
          min = get_min(min, i - a);
          max = get_max(max, i - a);
          candUp[i - a] = true;
        }

        if (i % b == 0){
          min = get_min(min, i/b);
          max = get_max(max, i/b);
          candUp[i/b] = true;
        }
      }
      cnt++; max_up = max; min_up = min;
    }
    else{
      // step up for down Stage
      int min = N_MAX;
      int max = -1;
      for (int i=max_down; i >= min_down; i--){
        if (candDown[i] == false) continue;
        candDown[i] = false;
        if (candUp[i] == true)
          return cnt;

        if (i + a < N_MAX && i + a > 0){
          min = get_min(min, i + a);
          max = get_max(max, i + a);
          candDown[i + a] = true;
        }

        if (i*b < N_MAX && i*b > 0){
          min = get_min(min, i*b);
          max = get_max(max, i*b);
          candDown[i*b] = true;
        }
      }
      cnt++; max_down = max; min_down = min;
    }
  }
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
  freopen("MH1716.txt", "r", stdin);
  cin>>T;
  /*
     여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
  */
  for(test_case = 1; test_case <= T; ++test_case)
  {

    /////////////////////////////////////////////////////////////////////////////////////////////
    cin >> s >> t >> a >> b;

    int result = solve(s,t,a,b);
    cout << "#" << test_case << " " << result << endl;
    /////////////////////////////////////////////////////////////////////////////////////////////

  }
  return 0;//정상종료시 반드시 0을 리턴해야합니다.
}