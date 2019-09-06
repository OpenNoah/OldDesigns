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

char * chess[2] = { FIRST "��", LAST "��" };
char * unit[2] = { "��", "��" };
char * menu[6] = { " ��  �� ", " ��  �� ", " ��  �� ", " ��  �� ", " ��  �� ", " ��  �� " };
char * menumsg[6] = { TEXT "ѡ��1: " LIGHT "����һ��", TEXT "ѡ��2: " LIGHT "���¿�ʼ", TEXT "ѡ��3: " LIGHT "����״̬", TEXT "ѡ��4: " LIGHT "��ȡ״̬", TEXT "ѡ��5: " LIGHT "����ѡ��", TEXT "ѡ��6: " LIGHT "�˳���Ϸ" };
char * counts[5] = { LIGHT "�غ�" MSG "����: " LIGHT, LIGHT "����" MSG "����: " LIGHT, LIGHT "����" MSG "����: " LIGHT , FIRST "��" MSG "��ʤ����: " LIGHT, LAST "��" MSG "��ʤ����: " LIGHT };
char * selectcolor[2] = { UNSELECT, SELECT };
char yesno[2][3] = { "��", "��" };

void MsgShow ( const char * message )
{
  printf ( "\e[16;1H\e[K%s" ESC_END, message );
}

void TurnShow ()
{
  char tmp[40];
  sprintf ( tmp, TEXT "��%s" TEXT "��...", chess[turn] );
  MsgShow ( tmp );
}

void WinShow ( const int type )
{
  char tmp[40];
  if ( type )
    sprintf ( tmp, "%s" TEXT "��ʤ��!", chess[turn] );
  else
    strcpy ( tmp, TEXT "��������,����" );
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
" BOARD "�X�j�j�j�j�j�j�j�j�j�j�j�j�j�[" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|" NAME "   ***������***\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p" SPOT "�p" BOARD "�p�p�p�p�p�p�p" SPOT "�p" BOARD "�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p" SPOT "�p" BOARD "�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p" SPOT "�p" BOARD "�p�p�p�p�p�p�p" SPOT "�p" BOARD "�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�d�p�p�p�p�p�p�p�p�p�p�p�p�p�g" TEXT "|\n\
" BOARD "�^�m�m�m�m�m�m�m�m�m�m�m�m�m�a" TEXT "|" );
  for ( i = 0; i < 15; i++ )
    for ( j = 0; j < 15; j++ )
      if ( board[j][i] != 0 )
        printf ( "\e[%d;%dH%s", j + 1, i + 1, chess[board[j][i] - 1] );
  MenuShow ( num );
  CountShow ();
}
