/*************************************
 *FILE    :msgbox.c
 *PROJECT :All
 *AUTHOR  :zhiyb
 *CREATED :9/25/2011
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "keyboard.h"
#include "msgbox.h"

#define BOARDER "\e[96;45m"
#define TITLE "\e[91;40m"
#define LINE "\e[32;40m"
#define ESC_END "\e[0m"

#define TOPLEFT BOARDER "¨X" ESC_END
#define TOP BOARDER "¨T" ESC_END
#define TOPRIGHT BOARDER "¨[" ESC_END
#define LEFTLINE BOARDER "¨U" ESC_END
#define RIGHTLINE BOARDER "¨U" ESC_END
#define BOTTOMLEFT BOARDER "¨^" ESC_END
#define BOTTOM BOARDER "¨T" ESC_END
#define BOTTOMRIGHT BOARDER "¨a" ESC_END

char * selectshow[2] = { "\e[34;40m  ", "\e[92;44m> \e[91;102m" };
char msgv[LINES][CHARSIZE], chov[LINES][CHARSIZE];

int size_x = 0, size_y = 0;

void getsize ( void )
{
  int input_fd = 0;
  struct winsize size;
  if ( size_x )
    return;
  if ( ioctl ( input_fd, TIOCGWINSZ, &size ) == -1 )
    fprintf ( stderr, "Read Terminal Size Error!" );
  else {
    size_x = size.ws_col;
    size_y = size.ws_row;
  }
}

int msgbox ( int index )
{
  int i, j, quit = 0, ret, len = 0;
  int pos_x, pos_y, msgc = LINES, choc = LINES;
  if ( ! size_x )
    return -1;
  for ( i = 0; i < msgc; i++ )
    if ( ! strlen ( msgv[i] ) )
      msgc = i;
    else if ( strlen ( msgv[i] ) > len )
      len = strlen ( msgv[i] );
  for ( i = 0; i < choc; i++ )
    if ( ! strlen ( chov[i] ) )
      choc = i;
    else if ( strlen ( chov[i] ) + 2 > len )
      len = strlen ( chov[i] ) + 2;
  pos_x = ( size_x - len ) / 2 + 1;
  pos_y = ( size_y - msgc - choc - 1 ) / 2 + 1;
  len = ( len + 1 ) / 2 * 2;
  printf ( "\e[2J\e[1;1H\e[?25l" );
  while ( ! quit )
  {
    printf ( "\e[%d;%dH" TOPLEFT, pos_y - 1, pos_x - 3 );
    for ( i = 0; i < len / 2 + 1; i++ )
      printf ( TOP );
    printf ( TOPRIGHT );
    for ( i = 0; i < msgc; i++ )
    {
      printf ( "\e[%d;%dH" LEFTLINE " " TITLE "%s", pos_y + i, pos_x - 3, msgv[i] );
      for ( j = 0; j < len - strlen ( msgv[i] ) + 1; j++ )
        printf ( " " );
      printf ( RIGHTLINE );
    }
    printf ( "\e[%d;%dH" LEFTLINE " " LINE, pos_y + msgc, pos_x - 3 );
    for ( i = 0; i < len; i++ )
      printf ( "#" );
    printf ( " " RIGHTLINE );
    for ( i = 0; i < choc; i++ )
    {
      printf ( "\e[%d;%dH" LEFTLINE " %s%s" ESC_END, pos_y + msgc + i + 1, pos_x - 3, selectshow[index == i], chov[i] );
      for ( j = 0; j < len - strlen ( chov[i] ) - 1; j++ )
        printf ( " " );
      printf ( RIGHTLINE );
    }
    printf ( "\e[%d;%dH" BOTTOMLEFT, pos_y + msgc + choc + 1, pos_x - 3 );
    for ( i = 0; i < len / 2 + 1; i++ )
      printf ( BOTTOM );
    printf ( BOTTOMRIGHT );
    switch ( getchar () )
    {
      case UP:
        index -= index != 0;
        break;
      case DOWN:
        index += index != choc - 1;
        break;
      case LEFT:
      case PGUP:
        index = 0;
        break;
      case RIGHT:
      case PGDOWN:
        index = choc - 1;
        break;
      case ENTER:
      case SOUND:
        ret = index;
        quit = 1;
        break;
      case ESC:
      case MENU:
        ret = -2;
        quit = 1;
        break;
      case STOP:
        ret = -3;
        quit = 1;
        break;
    }
  }
  printf ( "\e[?25h" );
  return ret;
}
