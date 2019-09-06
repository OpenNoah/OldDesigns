#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

#define NAME "\e[92;40m"
#define TEXT "\e[37;40m"
#define MSG "\e[93;40m"
#define LIGHT "\e[97;40m"
#define BOARD "\e[97;40m"
#define SPOT "\e[96;40m"
#define FIRST "\e[91;40m"
#define LAST "\e[94;40m"
#define SELECT "\e[91;102m"
#define UNSELECT "\e[31;40m"
#define ESC_END "\e[0m"

char * chess[2] = { FIRST "⊙", LAST "×" };
char * unit[2] = { "⊙", "×" };
char * menu[6] = { " 悔  棋 ", " 和  棋 ", " 存  档 ", " 读  档 ", " 其  它 ", " 退  出 " };
char * menumsg[6] = { TEXT "选项1: " LIGHT "后退一步", TEXT "选项2: " LIGHT "重新开始", TEXT "选项3: " LIGHT "保存状态", TEXT "选项4: " LIGHT "读取状态", TEXT "选项5: " LIGHT "其它选项", TEXT "选项6: " LIGHT "退出游戏" };
char * counts[5] = { LIGHT "回合" MSG "次数: " LIGHT, LIGHT "悔棋" MSG "次数: " LIGHT, LIGHT "和棋" MSG "次数: " LIGHT , FIRST "⊙" MSG "获胜次数: " LIGHT, LAST "×" MSG "获胜次数: " LIGHT };
char * selectcolor[2] = { UNSELECT, SELECT };
char yesno[2][3] = { "否", "是" };

void MsgShow ( const char * message )
{
  printf ( "\e[16;1H\e[K%s" ESC_END, message );
}

void TurnShow ()
{
  char tmp[40];
  sprintf ( tmp, TEXT "该%s" TEXT "了...", chess[turn] );
  MsgShow ( tmp );
}

void WinShow ( const int type )
{
  char tmp[40];
  if ( type )
    sprintf ( tmp, "%s" TEXT "获胜了!", chess[turn] );
  else
    strcpy ( tmp, TEXT "棋盘已满,和棋" );
  MsgShow ( tmp );
}

void CountShow ()
{
  int i;
  for ( i = 0; i < 5; i++ )
    printf ( "\e[%d;20H\e[K%s%d" ESC_END, i + 11, counts[i], count[i] );
}

void MenuShow ( const int index )
{
  int i;
  for ( i = 0; i < 6; i++ )
  {
    printf ( "\e[%d;22H\e[K%s%s" ESC_END, i + 4, selectcolor[index == i], menu[i] );
    if ( index != -1 )
      MsgShow ( menumsg[index] );
  }
}

void BoardShow ( const int num )
{
  int i, j;
  printf ( "\e[2J\e[1;1H\
" BOARD "╔╦╦╦╦╦╦╦╦╦╦╦╦╦╗" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|" NAME "   ***五子棋***\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬" SPOT "╬" BOARD "╬╬╬╬╬╬╬" SPOT "╬" BOARD "╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬" SPOT "╬" BOARD "╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬" SPOT "╬" BOARD "╬╬╬╬╬╬╬" SPOT "╬" BOARD "╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╠╬╬╬╬╬╬╬╬╬╬╬╬╬╣" TEXT "|\n\
" BOARD "╚╩╩╩╩╩╩╩╩╩╩╩╩╩╝" TEXT "|" );
  for ( i = 0; i < 15; i++ )
    for ( j = 0; j < 15; j++ )
      if ( board[j][i] != 0 )
        printf ( "\e[%d;%dH%s", j + 1, i + 1, chess[board[j][i] - 1] );
  MenuShow ( num );
  CountShow ();
}
