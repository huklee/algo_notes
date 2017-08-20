//
//

//#define TIME_STAMP_DEBUG 1
#define SOLVE_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define MIN_H 64
#define MAX_H 128 + 1
#define MAX_R MAX_H*2 + 1
#define DIGIT 36
#define TC_CASE 10000
#define MAX_D36_UNIT 1296

template <typename T>
void _swap (T &a, T &b){T c = a; a = b; b = c;}

char h1[MAX_H];
char h2[MAX_H];
char result[MAX_R];

#define D36_UNIT_BIT 64
#if D36_UNIT_BIT == 32
#define D36_COMP_UNIT 2
#define D36_DIGIT_MAX 1296
#define D36_MAX 256
struct D36{
  int d[D36_MAX];
};
struct D36 _h1, _h2, _res;
#elif D36_UNIT_BIT == 64
#define D36_COMP_UNIT 4
#define D36_MAX 128
#define D36_DIGIT_MAX 1679616
struct D36{
  long long d[D36_MAX];
};
struct D36 _h1, _h2, _res;
#endif


bool is_preprocess = false;
char prep[MAX_D36_UNIT];

int _strlen(char str[]) {
  int res = 0;
  while (true) {
    if (str[res] == 0) break;
    res++;
  }
  return res;
}

int c2i (char c){
  if (c >= 0 && c <= '9')
    return c - '0';
  else
    return c - 'A' + 10;
}

char i2c (int i){
  if (i >= 0 && i <= 9)
    return '0' + i;
  else
    return 'A' + i - 10;
}

// TODO : improve
inline void D36_mul (struct D36 &a, struct D36 &b, struct D36 &res){
  register long long temp_arr[D36_MAX] = {0,};

  // Multiplication
  for (register int i=0 ; i< D36_MAX/2; i++){
    for (int j=0 ; j< D36_MAX/2; j++) {
      temp_arr[i + j] += a.d[i]*b.d[j];
    }
  }

  // carry
  register long long carry = 0;
  for (register int i=0; i < D36_MAX; i++){
    temp_arr[i] += carry;
    carry = temp_arr[i]/D36_DIGIT_MAX;
    res.d[i] = temp_arr[i]%D36_DIGIT_MAX;
  }
}

// TODO : DONE
void str_to_D36 (char carr[], struct D36 &d36_arr){
  int len = _strlen (carr);
  int iH2 = 0;

  // convert the 2 char into unit
  for (int i=len - 1; i >= 0; i -= D36_COMP_UNIT){
    long long temp = 0;

    #if D36_UNIT_BIT == 64
    if (i > 2)
      temp += 36*36*36 * c2i(carr[i - 3]);
    if (i > 1)
      temp += 36*36 * c2i(carr[i - 2]);
    #endif

    if (i > 0)
      temp += 36 * c2i(carr[i - 1]);
    temp += c2i(carr[i]);

    d36_arr.d[iH2++] = temp;
  }

  // fill 0 to other digits
  for (; iH2 < D36_MAX/2; iH2++)
    d36_arr.d[iH2] = 0;
}

// TODO : DONE
void D36_to_str (struct D36 &_res, char res[]){
  // Convert an int into 2 char
  int istr = 0;
  int i_last = 0;
  for (int iH2=0; iH2 < D36_MAX/2; iH2++){
    long long temp = _res.d[iH2];

    if (temp > 0) i_last = istr;
    if (temp >= 36) i_last = istr + 1;

#if D36_UNIT_BIT == 64
    if (temp >= 36*36) i_last = istr + 2;
    if (temp >= 36*36*36) i_last = istr + 3;
#endif

#if D36_UNIT_BIT == 32
    res[istr] = i2c(temp%36);
    res[istr + 1] = i2c(temp/36);
#elif D36_UNIT_BIT == 64
    res[istr] =     i2c(temp%36); temp /= 36;
    res[istr + 1] = i2c(temp%36); temp /= 36;
    res[istr + 2] = i2c(temp%36); temp /= 36;
    res[istr + 3] = i2c(temp%36); temp /= 36;
#endif

    #if D36_UNIT_BIT == 32
    istr += 2;
    #elif D36_UNIT_BIT == 64
    istr += 4;
    #endif
  }
  i_last++;
  res[i_last] = 0;

  // reverse();
  for (int i = 0; i < i_last/2; i++)
    _swap (res[i], res[i_last - 1 -i]);
}

void generate(){
  int c, cnt;

  cnt = rand()%(MAX_H - MIN_H - 1) + MIN_H;
  for(int i=0; i < cnt; i++) {
    c = rand()%DIGIT;
    h1[i] = (c < 10) ? c + '0' : c + 'A' - 10;
  }
  h1[cnt] = 0;

  cnt = rand()%(MAX_H - MIN_H - 1) + MIN_H;
  for(int i=0; i < cnt; i++) {
    c = rand()%DIGIT;
    h2[i] = (c < 10) ? c + '0' : c + 'A' - 10;
  }
  h2[cnt] = 0;
}

void solve(){
#ifdef TIME_STAMP_DEBUG
  clock_t t;
  t = clock();
#endif
  str_to_D36 (h1, _h1);
#ifdef TIME_STAMP_DEBUG
  cout << "str_to_D36 time elapsed : " << (float)(clock() - t)/CLOCKS_PER_SEC << "sec" << endl;
#endif

#ifdef TIME_STAMP_DEBUG
  t = clock();
#endif
  str_to_D36 (h2, _h2);
#ifdef TIME_STAMP_DEBUG
  cout << "str_to_D36 time elapsed : " << (float)(clock() - t)/CLOCKS_PER_SEC << "sec" << endl;
#endif

#ifdef TIME_STAMP_DEBUG
  t = clock();
#endif
  D36_mul (_h1, _h2, _res);
#ifdef TIME_STAMP_DEBUG
  cout << "str_to_D36 time elapsed : " << (float)(clock() - t)/CLOCKS_PER_SEC << "sec" << endl;
#endif

#ifdef TIME_STAMP_DEBUG
  t = clock();
#endif
  D36_to_str (_res, result);
#ifdef TIME_STAMP_DEBUG
  cout << "str_to_D36 time elapsed : " << (float)(clock() - t)/CLOCKS_PER_SEC << "sec" << endl;
  cout << endl << endl;
#endif

  return;
}

int main(){
  srand(3);


#if SOLVE_DEBUG == 1
  h1[0] = '2';
  h1[1] = '4';
  h1[2] = 'N';
  h1[3] = 0;

  h2[0] = '7';
  h2[1] = '0';
  h2[2] = 'K';
  h2[3] = 'V';
  h2[4] = 0;
#elif SOLVE_DEBUG == 2
  h1[0] = 'E';
  h1[1] = 'X';
  h1[2] = 'P';
  h1[3] = 'E';
  h1[4] = 'R';
  h1[5] = 'T';
  h1[6] = 0;

  h2[0] = 'E';
  h2[1] = 'X';
  h2[2] = 'P';
  h2[3] = 'E';
  h2[4] = 'R';
  h2[5] = 'T';
  h2[6] = 0;

#else
  clock_t t = clock();
  for (int tc=1; tc < TC_CASE; tc++){
    generate();
#endif


    solve();
#ifdef SOLVE_DEBUG
  cout << h1 << endl;
  cout << h2 << endl;
  cout << result << endl;
#else
//    cout << "#" << tc << " try : " << endl;
//    cout << h1 << endl;
//    cout << h2 << endl;
//    cout << result << endl;
  }
  cout << "time elapsed : " << (float)(clock() - t)/CLOCKS_PER_SEC << "sec" << endl;
#endif
  return 0;
}
