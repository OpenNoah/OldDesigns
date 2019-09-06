/**************************************
 *FILE    :state.c
 *PROJECT :Five
 *AUTHOR  :zhiyb
 *CREATED :9/27/2011
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "display.h"

#define FSAVE DATA "/five"

FILE * savefile = NULL;

int opensettings ( const char * type )
{
  char tmp[50];
  sprintf ( tmp, FSAVE "/Five%d%d.conf", useai, help_go );
  savefile = fopen ( tmp, type );
  if ( savefile == NULL )
  {
    system ( "mkdir -p " FSAVE );
    savefile = fopen ( tmp, type );
    if ( savefile == NULL )
      return 0;
  }
  return 1;
}

int openstate ( const int num, const char * type )
{
  char tmp[50];
  sprintf ( tmp, FSAVE "/save%d%d%d.dat", useai, help_go, num );
  savefile = fopen ( tmp, type );
  if ( savefile == NULL )
  {
    system ( "mkdir -p " FSAVE );
    savefile = fopen ( tmp, type );
    if ( savefile == NULL )
      return 0;
  }
  return 1;
}

void removestate ( int num )
{
  char tmp[50];
  sprintf ( tmp, "rm -rf %s/save%d.dat", FSAVE, num );
  system ( tmp );
}

void savesettings ()
{
  if ( ! opensettings ( "wb" ) )
    return;
  fwrite ( &state, sizeof ( state ), 1, savefile );
  fclose ( savefile );
}

void savestate ( const int num )
{
  if ( ! openstate ( num, "wb" ) )
    return;
  fwrite ( board, sizeof ( board[0][0] ), sizeof ( board ) / sizeof ( board[0][0] ), savefile );
  fwrite ( save_x, sizeof ( save_x[0] ), sizeof ( save_x ) / sizeof ( save_x[0] ), savefile );
  fwrite ( save_y, sizeof ( save_y[0] ), sizeof ( save_y ) / sizeof ( save_y[0] ), savefile );
  fwrite ( count, sizeof ( count[0] ), sizeof ( count ) / sizeof ( count[0] ), savefile );
  fwrite ( &step, sizeof ( step ), 1, savefile );
  fwrite ( &turn_init, sizeof ( turn_init ), 1, savefile );
  fwrite ( &turn, sizeof ( turn ), 1, savefile );
  fwrite ( &x, sizeof ( x ), 1, savefile );
  fwrite ( &y, sizeof ( y ), 1, savefile );
  fclose ( savefile );
}

void loadsettings ()
{
  if ( ! opensettings ( "rb" ) )
  {
    state = 1;
    savesettings ();
    return;
  }
  fread ( &state, sizeof ( state ), 1, savefile );
  fclose ( savefile );
}

int loadstate ( const int num )
{
  if ( ! openstate ( num, "rb" ) )
    return 0;
  fread ( board, sizeof ( board[0][0] ), sizeof ( board ) / sizeof ( board[0][0] ), savefile );
  fread ( save_x, sizeof ( save_x[0] ), sizeof ( save_x ) / sizeof ( save_x[0] ), savefile );
  fread ( save_y, sizeof ( save_y[0] ), sizeof ( save_y ) / sizeof ( save_y[0] ), savefile );
  fread ( count, sizeof ( count[0] ), sizeof ( count ) / sizeof ( count[0] ), savefile );
  fread ( &step, sizeof ( step ), 1, savefile );
  fread ( &turn_init, sizeof ( turn_init ), 1, savefile );
  fread ( &turn, sizeof ( turn ), 1, savefile );
  fread ( &x, sizeof ( x ), 1, savefile );
  fread ( &y, sizeof ( y ), 1, savefile );
  fclose ( savefile );
  return 1;
}
