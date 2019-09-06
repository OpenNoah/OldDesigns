/**************************************
 *FILE    :ai.c
 *PROJECT :Five
 *AUTHOR  :zhiyb
 *CREATED :9/26/2011
***************************************/

#include "display.h"

void ai ()
{
  int qiju[2][15][15][8][2];  //棋型数组
  int i, j, k, q, a = 1, b = 0, d;
  int y1 = 0, y2 = 0, x1 = 0, x2 = 0;
  int a1[15][15], a2[15][15];

  for ( i = 0; i < 2; i++ )
    for ( j = 0; j < 15; j++ )
      for ( k = 0; k < 15; k++ )
        for ( q = 0; q < 8; q++ )
          for ( d = 0; d < 2; d++ )
            qiju[i][j][k][q][d] = 0;

  //为双方填写棋型表
  for ( k = 0; k < 2; k++ )
    for ( i = 0; i < 15; i++ )
      for ( j = 0; j < 15; j++ )
        if ( board[i][j] == 0 )
          for ( q = 0; q < 8; q++ )
          {
            d = k + 1;
            switch ( q )
            {
              case 0:
                while ( j - a >= 0 && board[i][j - a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i][j - a] == 0;
                break;
              case 1:
                while ( i - a >= 0 && j - a >= 0 && board[i - a][j - a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i - a][j - a] == 0;
                break;
              case 2:
                while ( i - a >= 0 && board[i - a][j] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i - a][j] == 0;
                break;
              case 3:
                while ( i - a >= 0 && j + a <= 14 && board[i - a][j + a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i - a][j + a] == 0;
                break;
              case 4:
                while ( j + a <= 14 && board[i][j + a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i][j + a] == 0;
                break;
              case 5:
                while ( i + a <= 14 && j + a <= 14 && board[i + a][j + a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i + a][j + a] == 0;
                break;
              case 6:
                while ( i + a <= 14 && board[i + a][j] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i + a][j] == 0;
                break;
              case 7:
                while ( i + a <= 14 && j - a >= 0 && board[i + a][j - a] == d )
                  a++;
                qiju[k][i][j][q][1] = board[i + a][j - a] == 0;
                break;
            }
            qiju[k][i][j][q][0] = a - 1;
            a = 1;
          }

  //根据评分规则对每一个空格评分
  for ( i = 0; i < 15; i++ )
    for ( j = 0; j < 15; j++ )
    {
      k = 0;
      for ( q = 0; q < 4; q++ )
      {
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) >= 4 )
          b += k == turn ? 50000 : 7000;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 3 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 301;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 2 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 43;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 1 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 11;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 3 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 63;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 2 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 6;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 1 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 1;
      }
      if ( b == 126 || b == 189 || b == 252 )
        b = 1500;
      else if ( b == 106 )
        b = 1000;
      a1[i][j] = b;
      b = 0;
      k = 1;
      for ( q = 0; q < 4; q++ )
      {
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) >= 4 )
          b += k == turn ? 50000 : 30000;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 3 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 1500;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 2 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 51;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 1 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1 )
          b += 16;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 3 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 71;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 2 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 7;
        if ( ( qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0] ) == 1 && ( ( qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0 ) || ( qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1 ) ) )
          b += 2;
      }
      if ( b == 142 || b == 213 || b == 284 )
        b = 1500;
      else if ( b == 122 )
        b = 1300;
      a2[i][j] = b;
      b = 0;
    }

  //算出分数最高的空位,填写坐标
  for ( i = 0; i < 15; i++ )
    for ( j = 0; j < 15; j++ )
    {
      if ( a1[y1][x1] < a1[i][j] || ( a1[y1][x1] == 0 && board[i][j] == 0 ) )
      {
        y1 = i;
        x1 = j;
      }
      if ( a2[y2][x2] < a2[i][j] || ( a2[y2][x2] == 0 && board[i][j] == 0 ) )
      {
        y2 = i;
        x2 = j;
      }
    }
  if ( a2[y2][x2] >= a1[y1][x1] )
  {
    x = x2;
    y = y2;
  } else {
    x = x1;
    y = y1;
  }
  if ( step == 0 )
  {
    x = 7;
    y = 7;
  }
}
