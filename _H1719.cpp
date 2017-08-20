#include <stdio.h>

const int MAXSIZE = 100000;

struct MEETING {
  int s, e;
  int next;
};

struct QUERY {
  int l, r;
};

MEETING meet[MAXSIZE + 1];
QUERY query[MAXSIZE + 1];
int tc, T, N, Q;

int COMP(const MEETING& a, const MEETING& b) {
  if (a.e == b.e)
    return a.s - b.s;
  else
    return a.e - b.e;
}

void QSORT(int from, int to) {
  if (from < to) {
    int l = from, r = to;
    MEETING pivot = meet[(from + to) >> 1];
    while (l <= r) {
      while (COMP(meet[l], pivot) < 0) ++l;
      while (COMP(meet[r], pivot) > 0) --r;
      if (l <= r) {
        MEETING temp = meet[l];
        meet[l] = meet[r], meet[r] = temp;
        ++l, --r;
      }
    }
    QSORT(from, r);
    QSORT(l, to);
  }
}

int BSEARCH(int left, int right, int val) {
  if (left > right)
    return 0;

  int mid = (left + right) >> 1;
  for (; left <= right; mid = (left + right) >> 1) {
    // in case the value matches
    if (meet[mid].e == val) {
      // find the last meeting schedule of same end time
      while (mid < N && meet[mid].e == meet[mid + 1].e)
        ++mid;
      return mid;
    }
    else {
      if (val < meet[mid].e)
        right = mid - 1;
      else
        left = mid + 1;
    }
  }
  return mid;
}

void input(void) {
  scanf("%d%d", &N, &Q);
  for (int i = 1; i <= N; ++i)
    scanf("%d%d", &meet[i].s, &meet[i].e);

  for (int i = 1; i <= Q; ++i)
    scanf("%d%d", &query[i].l, &query[i].r);
}

long long solve(void) {
  long long ans = 0, cnt;
  QSORT(1, N);

  // 바로 다음 이용 가능한 미팅 룸 중 가장 빨리 끝나는 미팅룸 next로 선택
  for (int i = 1; i < N; ++i) {
    for (int j = i + 1; j <= N; ++j) {
      if (meet[i].e <= meet[j].s) {
        meet[i].next = j;
        break;
      }
    }
  }
  meet[N].next = 0;

  for (int i = 1, lIdx, rIdx, left; i <= Q; ++i) {
    // 주어진 오른쪽 끝나는 것 기준으로 right index 찾기
    rIdx = BSEARCH(1, N, query[i].r);
    if (rIdx == 0) continue;

    // 주어진 왼쪽 끝나는 것 기준으로 left index 찾기
    // cf) BS를 통해서 완전 일치하는 elem을 찾지 못해도 상관 없음 -> 일치하지 않으면하나씩 늘려나가면서 탐색하므로
    lIdx = BSEARCH(1, N, query[i].l);
    if (lIdx == 0) lIdx = 1;    // lIdex == 0이면 해당하는 index 없는 경우 : start_index = 1

    cnt = 0;
    left = query[i].l;

    // 가장 왼쪽에서 끝나는 것 기준으로 가장 빨리 끝나는 회의실들을 이어붙이면서 가능한 갯수 찾기
    for (int j = lIdx; j <= rIdx;) {
      if (left <= meet[j].s && query[i].l <= meet[j].s) {
        ++cnt;
        left = meet[j].e;
        j = meet[j].next;

        // end case
        if (j == 0) break;
      }
      else
        ++j;
    }
    ans += i * cnt;
  }
  return ans;
}

int main(void) {
  scanf("%d", &T);
  for (tc = 1; tc <= T; ++tc) {
    input();
    printf("#%d %lld\n", tc, solve());
  }
  return 0;
}
