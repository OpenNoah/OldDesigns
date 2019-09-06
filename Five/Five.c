#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "display.h"
#include "keyboard.h"
#include "msgbox.h"

int board[15][15];
int save_x[230], save_y[230], count[5];
int x = 7, y = 7, step;
int turn_init, turn, state, help_go, useai;

extern FILE * savefile;

int win ( const int );

void reset ()
{
  int i, j;
  for ( i = 0; i < 15; i++ )
    for ( j = 0; j < 15; j++ )
      board[i][j] = 0;
  for ( i = 0; i < sizeof ( save_x ) / sizeof ( save_x[0] ); i++ )
  {
    save_x[i] = -1;
    save_y[i] = -1;
  }
  step = 0; turn = turn_init; x = 7; y = 7;
  count[0] = 0; count[1] = 0;
}

void init ()
{
  system ( QCOP "QPE/System \"setKeyboard(QString)\" NULL" );
  getsize ();
  freopen ( "/dev/tty0", "r+", stdin );
  system ( "stty -echo" );
  printf ( "\e[2J\e[1;1H\e[?25h" );
  count[2] = 0; count[3] = 0; count[4] = 0;
  turn_init = 0;
}

void recover ()
{
  system ( "stty echo" );
  printf ( "\e[2J\e[1;1H\e[?25h" );
  freopen ( "/dev/tty", "r+", stdin );
  system ( QCOP "QPE/System \"setKeyboard(QString)\" TTY:" TTY );
}

int win ( const int type )
{
  int quit = 0, quitgame = 0;
  if ( type )
    if ( useai == turn + 1 )
      strcpy ( msgv[0], "AI获胜了!!!" );
    else
      strcpy ( msgv[0], "您获胜了!!!" );
  else
    strcpy ( msgv[0], "棋盘已满,和棋" );
  sprintf ( msgv[1], "回合数: %d", count[0] );
  strcpy ( msgv[2], "" );
  strcpy ( chov[0], " 再来一盘 " );
  strcpy ( chov[1], " 查看棋盘 " );
  strcpy ( chov[2], " 退出游戏 " );
  strcpy ( chov[3], "" );
  while ( ! quit )
  {
    switch ( msgbox ( 0 ) )
    {
      case 1:
        BoardShow ( -1 );
        WinShow ( type );
        printf ( "\e[%d;%dH", y + 1, x + 1 );
        getch ();
        break;
      case 2:
        quitgame = 1;
      case 0:
      case -2:
      case -3:
        quit = 1;
    }
  }
  count[type ? 3 + turn : 2]++;
  reset ();
  step = -1;
  turn_init = ! turn_init;
  return quitgame;
}

int check_win ()
{
  int i, j, check_x, check_y, num;
  if ( step == 224 )
    return win ( 0 );
  for ( i = 0; i < 4; i++ )
  {
    num = 0;
    for ( j = 1; j > -2; j -= 2 )
    {
      check_x = x; check_y = y;
      while ( board[check_y][check_x] == turn + 1 )
      {
        num++;
        check_x += ( i == 0 ) * j;
        check_x += ( i == 1 ) * j;
        check_y += ( i == 1 ) * j;
        check_y += ( i == 2 ) * j;
        check_x -= ( i == 3 ) * j;
        check_y += ( i == 3 ) * j;
        if ( check_x < 0 || check_y < 0 || check_x > 14 || check_y > 14 )
          break;
      }
    }
    if ( num > 5 )
      return win ( 1 );
  }
  return 0;
}

void go ()
{
  int quit;
  if ( board[y][x] != 0 )
    return;
  count[0] += turn != turn_init;
  board[y][x] = turn + 1;
  save_x[step] = x;
  save_y[step] = y;
  quit = check_win ();
  step++;
  turn = ! turn;
  if ( quit )
  {
    if ( state )
      savestate ( 0 );
    recover ();
    exit ( 0 );
  }
  if ( help_go == 3 || help_go == turn + 1 )
    ai ();
}

void undo ()
{
  if ( step == 0 || ( useai != 0 && step == 2 - useai ) )
    return;
  step--; turn = ! turn;
  x = save_x[step]; y = save_y[step];
  board[y][x] = 0;
  count[0] -= turn != turn_init;
  count[1] += useai != turn + 1;
}

void redo ()
{
  if ( save_x[step] == -1 || save_y[step] == -1 )
    return;
  x = save_x[step]; y = save_y[step];
  board[y][x] = turn + 1;
  count[0] += turn != turn_init;
  count[1] -= useai != turn + 1;
  step++; turn = 1 - turn;
  if ( help_go == 3 || ( help_go != 0 && help_go == turn + 1 ) )
    ai ();
}

void deletestate ( const int num )
{
  char tmp[30];
  if ( num == 0 )
    strcpy ( msgv[0], "您确定要清除即时存档?" );
  else
    sprintf ( msgv[0], "您确定要清除存档%d?", num );
  strcpy ( msgv[1], "" );
  strcpy ( chov[0], " 是 " );
  strcpy ( chov[1], " 否 " );
  strcpy ( chov[2], "" );
  switch ( msgbox ( 0 ) )
  {
    case -1:
      recover ();
      exit ( 1 );
    case 0:
      removestate ( num );
  }
}

void save_select ( const int saveload, const char msg[6] )
{
  int num, quit = 0;
  while ( ! quit )
  {
    sprintf ( msgv[0], "%s存档", msg );
    strcpy ( msgv[1], "请选择存档位置:" );
    strcpy ( msgv[2], "" );
    strcpy ( chov[0], " 即时存档 " );
    for ( num = 1; num <= 5; num++ )
      sprintf ( chov[num - ( saveload != 2 ) ], " 存档位置%d ", num );
    strcpy ( chov[5 + ( saveload == 2 ) ], " 返回菜单 " );
    strcpy ( chov[6 + ( saveload == 2 ) ], "" );
    num = msgbox ( 0 );
    switch ( num )
    {
      case -1:
        recover ();
        exit ( 1 );
      default:
        if ( saveload == 2 )
        {
          deletestate ( num );
          break;
        } else {
          if ( num == 5 )
            return;
          if ( saveload == 1 )
            savestate ( num + 1 );
          else
            loadstate ( num + 1 );
        }
      case -2:
      case -3:
      case 6:
        quit = 1;
    }
  }
}

int resetgame ()
{
  strcpy ( msgv[0], "您确定要重置游戏?" );
  strcpy ( msgv[1], "" );
  strcpy ( chov[0], " 是 " );
  strcpy ( chov[1], " 否 " );
  strcpy ( chov[2], "" );
  switch ( msgbox ( 0 ) )
  {
    case -1:
      recover ();
      exit ( 1 );
    case 0:
      init ();
      reset ();
      return 1;
  }
  return 0;
}

void otheroption ()
{
  int quit = 0, index = 0;
  int set_state = state;
  while ( ! quit )
  {
    strcpy ( msgv[0], "其它选项" );
    strcpy ( msgv[1], "" );
    strcpy ( chov[0], " 重置游戏 " );
    strcpy ( chov[1], " 清除存档 " );
    strcpy ( chov[3], " 保存设置 " );
    strcpy ( chov[4], " 取消设置 " );
    strcpy ( chov[5], "" );
    sprintf ( chov[2], " 使用即时存档? (%s) ", yesno[set_state] );
    index = msgbox ( index );
    switch ( index )
    {
      case -1:
        recover ();
        exit ( 1 );
      case 0:
        quit = resetgame ();
        break;
      case 1:
        save_select ( 2, "清除" );
        break;
      case 2:
        set_state = ! set_state;
        break;
      case 3:
        state = set_state;
        savesettings ();
      case 4:
      case -2:
      case -3:
        quit = 1;
    }
  }
}

void go_menu ()
{
  int quit_menu = 0, index = 0, key;
  while ( ! quit_menu )
  {
    BoardShow ( index );
    printf ( "\e[%d;%dH", y + 1, x + 1 );
    key = getch ();
    switch ( key )
    {
      case UP:
        index -= index != 0;
        break;
      case DOWN:
        index += index != 5;
        break;
      case PGUP:
      case LEFT:
        index = 0;
        break;
      case PGDOWN:
      case RIGHT:
        index = 5;
        break;
      case ENTER:
      case SOUND:
        switch ( index )
        {
          case 0:
            undo ();
            if ( useai )
              undo ();
            break;
          case 1:
            reset ();
            count[2]++;
            turn_init = ! turn_init;
            turn = ! turn;
            quit_menu = 1;
            break;
          case 2:
            save_select ( 1, "保存" );
            break;
          case 3:
            save_select ( 0, "读取" );
            break;
          case 4:
            otheroption ();
            break;
          case 5:
            recover ();
            exit ( 0 );
        }
        break;
      case ESC:
      case STOP:
      case MENU:
        quit_menu = 1;
    }
  }
}

int whichai ()
{
  strcpy ( msgv[0], "开始游戏!" );
  strcpy ( msgv[1], "请选择先手方:" );
  strcpy ( msgv[2], "" );
  strcpy ( chov[0], " 你 先手 " );
  strcpy ( chov[1], " AI 先手 " );
  strcpy ( chov[2], "" );
  switch ( msgbox ( 0 ) )
  {
    case -1:
      recover ();
      exit ( 1 );
    case -2:
    case -3:
      recover ();
      exit ( 0 );
    case 1:
      return 1;
    case 0:
      return 2;
  }
}

int whichhelp ()
{
  strcpy ( msgv[0], "开始游戏!" );
  strcpy ( msgv[1], "请选择辅助方:" );
  strcpy ( msgv[2], "" );
  strcpy ( chov[0], "辅助 先手" );
  strcpy ( chov[1], "辅助 后手" );
  strcpy ( chov[2], "辅助 双方" );
  strcpy ( chov[3], "" );
  switch ( msgbox ( 2 ) )
  {
    case -1:
      recover ();
      exit ( 1 );
    case -3:
      recover ();
      exit ( 0 );
    case 2:
    case -2:
      return 3;
    case 1:
      return 2;
    case 0:
      return 1;
  }
}

int main ()
{
  char * tmp[20];
  int quit_main = 0, quit = 0;
  init (); reset ();
#if 0
  strcpy ( msgv[0], "开始游戏!" );
  strcpy ( msgv[1], "请选择游戏模式:" );
  strcpy ( msgv[2], "" );
  strcpy ( chov[0], " 人机 对弈 " );
  strcpy ( chov[1], " 人机 AI辅助 " );
  strcpy ( chov[2], " 双人 对弈 " );
  strcpy ( chov[3], " 双人 AI辅助 " );
  strcpy ( chov[4], " 退出游戏 " );
  strcpy ( chov[5], "" );
  switch ( msgbox ( 0 ) )
  {
    case -1:
      recover ();
      exit ( 1 );
    case 4:
    case -2:
    case -3:
      recover ();
      exit ( 0 );
    case 0:
      help_go = 0;
      useai = whichai ();
      break;
    case 1:
      help_go = 3;
      useai = whichai ();
      break;
    case 2:
      help_go = 0;
      useai = 0;
      break;
    case 3:
      help_go = whichhelp ();
      useai = 0;
  }
#endif
  useai = 1;
  loadsettings ();
  if ( state )
    loadstate ( 0 );
  if ( useai == turn + 1 )
  {
    ai ();
    go ();
  }
  while ( ! quit_main )
  {
    if ( state )
      savestate ( 0 );
    BoardShow ( -1 );
    TurnShow ();
    printf ( "\e[%d;%dH", y + 1, x + 1 );
    switch ( getch () )
    {
      case UP:
        y -= y != 0;
        break;
      case DOWN:
        y += y != 14;
        break;
      case LEFT:
        x -= x != 0;
        break;
      case RIGHT:
        x += x != 14;
        break;
      case ENTER:
      case SOUND:
        go ();
        if ( useai == turn + 1 )
        {
          ai ();
          go ();
          if ( useai == turn + 1 )
          {
            ai ();
            go ();
          }
        }
        break;
      case ESC:
      case MENU:
        go_menu ();
        break;
      case PGUP:
      case TAB:
        redo ();
        if ( useai )
          redo ();
        break;
      case PGDOWN:
      case SPACE:
        undo ();
        if ( useai )
          undo ();
        break;
      case STOP:
        quit_main = 1;
    }
  }
  recover ();
}
