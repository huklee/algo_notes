//
//
//

#define SOLVE_DEBUG 2

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define TC_MAX 1000
#define QRC_SIZE 100
#define IMG_SIZE 200
#define POST_NOISE 13000

template <typename T>
void _swap (T &a, T &b){T c = a; a = b; b = c;}


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


char SRC[TC_MAX][QRC_SIZE];
char GRY[TC_MAX][QRC_SIZE][QRC_SIZE];
char QRC[TC_MAX][QRC_SIZE][QRC_SIZE];
char IMG[TC_MAX][IMG_SIZE][IMG_SIZE];
char DST[TC_MAX][QRC_SIZE];

void generate_src(char src[QRC_SIZE]) {
  for (int i =0; i < QRC_SIZE; i++)
    src[i] = 'A' + (rand() % 26);
}

void generate_gray(char img[IMG_SIZE][IMG_SIZE], char gry[QRC_SIZE][QRC_SIZE]) {
  for (int i =0; i < QRC_SIZE; i++)
    for (int j =0; j < QRC_SIZE; j++)
      gry[i][j] = img[2*i][2*j]
          + img[2*i + 1][2*j]
          + img[2*i][2*j + 1]
          + img[2*i + 1][2*j + 1];
}

void generate_img(char img[IMG_SIZE][IMG_SIZE], char qrc[QRC_SIZE][QRC_SIZE]) {
  // 03-1. Genrate the background noise
  for (int i =0; i < IMG_SIZE; i++)
    for (int j =0; j < IMG_SIZE; j++)
      img[i][j] = rand() % 2;

  // 03-2. Stamp the QRC into IMG
  int offseti = rand() % 100;
  int offsetj = rand() % 100;
  for (int i =0; i < QRC_SIZE; i++)
    for (int j =0; j < QRC_SIZE; j++)
      img[offseti + i][offsetj + j] = qrc[i][j];


  // 03-3. Generate the post-noise
  for (int i = 0; i < POST_NOISE; i++)
    img[rand() % IMG_SIZE][rand() % IMG_SIZE] = 1;
}

/////////////////////////////////// SOLVE CODE //////////////////////////////////////////////

// minimum of sum
// 1 2
// 3 4
int min_sum[4][4] = {
    {0, 2, 2, 4},
    {2, 0, 4, 2},
    {2, 4, 0, 2},
    {4, 2, 2, 0}};

int code(int col_shift, int row_shift){
  return col_shift + row_shift*2;
}

#define HEADER_ROW 12
void char2intarr(char c, int arr[5]) {
  int n = c - 'A';
  for (int i = 0; i < 5; i++){
    arr[i] = (n & 1);
    n >>= 1;
  }
}


// TODO : veirfy
void encode(char src[QRC_SIZE], char qrc[QRC_SIZE][QRC_SIZE]) {
  int space = 2;
  int unit_size = 4;

  // 01. header
  for(int row = 0; row < HEADER_ROW; row++)
    for (int col = 0; col < QRC_SIZE; col++)
      qrc[row][col] = 1;


  // 02. unit char
  int n = 0;
  for(int num_src = 0; num_src < QRC_SIZE; num_src++){
    int bitmap[5];
    char2intarr(src[num_src], bitmap);
    for (int k = 0; k < 5; k++, n++){
      if (bitmap[k] == 0) continue;

      // in case of filling, fill only 3*3;
      int row_num = n / 24;
      int col_num = n % 24;
      for(int i = 1; i < unit_size; i++)
        for(int j = 1; j < unit_size; j++){
          int row = row_num*4 + space + HEADER_ROW + i;
          int col = col_num*4 + space + j;
          qrc[row][col] = 1;
        }
    }
  }
}

// TODO : verify
bool isHeaderPosition(char gry[QRC_SIZE][QRC_SIZE], int offseti, int offsetj) {
  for (int i =0; i < HEADER_ROW/2 - 1; i++)
    for (int j =0; j < QRC_SIZE/2 - 1; j++)
      if (gry[offseti + i][offsetj + j] != 4)
        return false;
  return true;
}

// TODO : verify
void findShift(char gry[QRC_SIZE][QRC_SIZE], int offseti, int offsetj, int& col_shift, int& row_shift) {
  col_shift = 0, row_shift = 0;
  for (int i =0; i < HEADER_ROW/2 - 1; i++){
    if (gry[offseti + i][offsetj + QRC_SIZE/2 - 1] != 4){
      col_shift = 1;
      break;
    }
  }

  for (int j =0; j < QRC_SIZE/2 - 1; j++){
    if (gry[offseti + HEADER_ROW/2 - 1][offsetj + j] != 4){
      row_shift = 1;
      break;
    }
  }
}

// TODO : verify
void findHeaderPosition(char gry[QRC_SIZE][QRC_SIZE], int& offseti, int& offsetj) {
  for (offseti = 0; offseti < QRC_SIZE/2; offseti++)
    for(offsetj = 0; offsetj < QRC_SIZE/2; offsetj++)
      if (isHeaderPosition(gry, offseti, offsetj) == true)
        return;
}

// TODO : verify
char intarr2char(int pInt[5]) {
  int sum = 0;
  for (int k = 4; k >= 0; k--){
    sum <<= 1;
    sum |= pInt[k];
  }
  return sum + 'A';
}

// TODO : implement
int bit_decode(char gry[QRC_SIZE][QRC_SIZE], int si, int sj, int shift_code) {
  // Sum Check Process
  int sum = gry[si][sj]
      + gry[si + 1][sj]
      + gry[si][sj + 1]
      + gry[si + 1][sj + 1];
  bool sum_check = (sum >= 9) ? 1 : 0;

  // cell Check Process
  bool cell_check = true;
  int row_dir[4] = {0, 0, 1,1};
  int col_dir[4] = {0, 1, 0, 1};
  for (int dir = 0; dir < 4; dir++){
    int row = si + row_dir[dir];
    int col = sj + col_dir[dir];
    if (gry[row][col] < min_sum[dir][shift_code]){
      cell_check = false;
      break;
    }
  }

  return sum_check && cell_check;
}

// TODO : verify
void decode(char gry[QRC_SIZE][QRC_SIZE], char dst[QRC_SIZE]) {
  // 01. find the position of the header
  int space = 2;
  int offseti, offsetj;
  int col_shift, row_shift, shift_code;
  findHeaderPosition(gry, offseti, offsetj);
  findShift(gry, offseti, offsetj, col_shift, row_shift);
  shift_code = code(col_shift, row_shift);

  // 02. decode the characters
  int num_src = 0;
  for (int n = 0; n < QRC_SIZE * 5; n++){
    int bitmap[5];

    // decode the each bit
    int k = n % 5;
    int row_num = n / 24;
    int col_num = n % 24;

    int si = offseti + row_num*2 + space/2 + HEADER_ROW/2;
    int sj = offsetj + col_num*2 + space/2;
    bitmap[k] = bit_decode(gry, si, sj, shift_code);

    // decode the each character
    if (n % 5 == 4){
      dst[num_src] = intarr2char(bitmap);
      num_src++;
    }
  }
}

/////////////////////////////////// SOLVE CODE //////////////////////////////////////////////

clock_t t;

int DEBUG_PRINT = 0;
int evaluate(char src[QRC_SIZE], char dst[QRC_SIZE]) {
  int point = (float)(clock() - t)/CLOCKS_PER_SEC*100;
  for (int i =0; i < QRC_SIZE; i++)
    if (src[i] != dst[i])
      point += 100;

  return point;
}

void print_qrc(char image[QRC_SIZE][QRC_SIZE]){
  for (int i =0; i < QRC_SIZE; i++){
    for (int j =0; j < QRC_SIZE; j++){
      printf ("%c", (image[i][j] == 0 ? ' ' : '.'));
    }
    printf ("\n");
  }
}

void print_img(char image[IMG_SIZE][IMG_SIZE]){
  for (int i =0; i < IMG_SIZE; i++){
    for (int j =0; j < IMG_SIZE; j++){
      printf ("%c", (image[i][j] == 0 ? ' ' : '.'));
    }
    printf ("\n");
  }
}

void print_gry(char image[QRC_SIZE][QRC_SIZE]){
  for (int i =0; i < QRC_SIZE; i++){
    for (int j =0; j < QRC_SIZE; j++){
      printf ("%d", image[i][j]);
    }
    printf ("\n");
  }
}

void solve(){
  int total_point = 0;
  for (int tc=0; tc < TC_MAX; tc++){
    // 01. Generate the SRC
    generate_src(SRC[tc]);
    t = clock();

#if SOLVE_DEBUG >= 1
    for(int i =0; i < QRC_SIZE; i++)
      cout << SRC[tc][i];
    cout << endl;
#endif

    // 02. Encode SRC -> QRC
    encode(SRC[tc], QRC[tc]);

#if SOLVE_DEBUG == 1
    if (tc == DEBUG_PRINT)
      print_qrc(QRC[tc]);
#endif
    // 03. Generate IMG : QRC -> IMG
    generate_img(IMG[tc], QRC[tc]);

#if SOLVE_DEBUG == 1
    if (tc == DEBUG_PRINT)
      print_img(IMG[tc]);
#endif

    // 04. Convert IMG into GRY
    generate_gray(IMG[tc], GRY[tc]);

#if SOLVE_DEBUG == 1
    if (tc == DEBUG_PRINT)
      print_gry(GRY[tc]);
#endif

    // 05. Decode
    decode(GRY[tc], DST[tc]);

#if SOLVE_DEBUG >= 1
    for(int i =0; i < QRC_SIZE; i++)
      cout << DST[tc][i];
    cout << endl;
#endif

    // 06. Evaluate
    total_point += evaluate (SRC[tc], DST[tc]);

  }

  printf ("RESULT : %d\n",total_point);
  return;
}


int main(){
  srand(3);


#ifdef SOLVE_DEBUG
  freopen("output.txt", "w", stdout);
#elif SOLVE_DEBUG == 2
#else
  clock_t t = clock();
  for (int tc=1; tc < TC_CASE; tc++){
    generate();
#endif

  solve();
#ifdef SOLVE_DEBUG
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

