/*
단순하게 greedy한 접근하면 문제점이 발생한다는 점을 알 수 있다.
먼저, 관찰해야 할 점은 이름의 길이는 항상 같다는 점과,
이름의 k번째 문자가 될 수 있는 것은
보드의(1,k), (2,k-1), …, (k,1)들 중에서 유효한 칸들로만 제한된다는 것이다.
  
  
풀이는 bfs와 비슷하게 진행된다.
처음 보드에는 0,0 에만 색칠되어 있다. 첫 번째 글자는 (0,0)인 것이 확정되기 때문이다.
이름의 다음 글자 후보들은, 이미 색칠되어 있는 보드 칸에서 오른쪽 or 아래쪽들이다.
이 후보들 중 알파벳 중 가장 사전 순으로 빠른 것들만 색칠되어 다음 검사를 진행할 최종 후보들이 된다.
  
  
즉, 갈 수 있는 후보들을 뽑고 가장 작은 알파벳을 가진 후보들만 다시 추려내는 과정을
N+M-1의 글자까지 반복해 진행하면 된다.
*/
#include<stdio.h>
#define SIZE 2011
#define QSIZE 4444
#define MIN(a,b) (a)<(b) ? (a) : (b)


int N, M;
char map[SIZE][SIZE];
int visit[SIZE][SIZE];// 방문 유무
int dirN[2] = { 0, 1 };// 우, 하
int dirM[2] = { 1, 0 };
char ans[QSIZE]; //정답 저장
int len = 0; // 정답의 길이


typedef struct point {
  int n, m;
};


point q[QSIZE]; //최종 후보들을 넣을 큐
int front = 0, rear = 0;

void enq(point x) {
 q[front] = x;
 front = (front + 1) % QSIZE;
}

point deq() {
 point x = q[rear];
 rear = (rear + 1) % QSIZE;
 return x;
}


//초기화
void init() {
 for (int i = 0; i < N; i++) {
  for (int j = 0; j < M; j++)
   visit[i][j] = 0; //방문 초기화
 }

 for (int i = 0; i < QSIZE; i++) {
  ans[i] = 0; // 답 초기화
 }
 rear = 0, front = 0; // 큐 초기화
}

void solve(){
 len = M + N - 1; // 완성될 답의 길이
 point x = { 0, 0 }; // 첫 시작점
 enq(x);
 ans[0] = map[0][0];


 //ans[l]번째 문자를 하나씩 결정해 나감
 for (int l = 1; l < len; l++) {
  point candidates[SIZE];
  int size = 0;

  //길이가 l일 때 갈 수 있는 곳을 모두 후보에 넣는다.
  while (front != rear) {
   point now = deq();

   for (int i = 0; i < 2; i++) {
    point next = now;
    next.n += dirN[i];
    next.m += dirM[i];

    if (next.n >= N || next.m >= M) continue;
    if (visit[next.n][next.m] == 0) {
     visit[next.n][next.m] = 1;
     candidates[size].n = next.n;
     candidates[size].m = next.m;
     size++;
    }
   }
  }

  // 후보들 중 가장 작은 알파벳 찾기
  char min = 'z' + 2;
  for (int i = 0; i < size; i++) {
   min = MIN(min, map[candidates[i].n][candidates[i].m]);
  }

  // 큐를 초기화하고,
  //가장 작을 알파벳을 가진 후보들만 최종적으로 다시 큐에 넣어 줌
  front = 0, rear = 0;
  for (int i = 0; i < size; i++) {
   if (map[candidates[i].n][candidates[i].m] == min)
    enq(candidates[i]);
  }

  //결정된 ans[l]번째 알파벳을 저장함
  ans[l] = min;
 }
}

int main() {
 int T;
 freopen("M1720.txt","r", stdin);
 scanf("%d", &T);

 for (int t = 1; t <= T; t++) {
  scanf("%d %d", &N, &M);
  init();

  for (int n = 0; n < N; n++) {
   scanf("%s", map[n]);
  }

  solve();
  printf("#%d %s\n", t, ans);
 }
 return 0;
}
