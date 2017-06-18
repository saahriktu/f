/*
 *  F - ncurses fifteen puzzle with hexadecimal numbering (from 1 to f)
 *  and random puzzles.
 *  Copyright (C) 2010-2011 Kurashov Artem Konstantinovich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <curses.h>
#include <stdlib.h>
#include <time.h>

int a[16], mmask[4], zls;
char fsym[] =
  { ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
  'e', 'f'
};
WINDOW *gw;

void
init_board ()
{
  int i;
  for (i = 0; i < 16; i++)
    if (i != 15)
      a[i] = i + 1;
    else
      a[i] = 0;
}

int
check_board ()
{
  int i;
  for (i = 0; i < 16; i++)
    if (i != 15 && a[i] != i + 1)
      return 0;
    else if (i == 15 && a[i] != 0)
      return 0;
  return 1;
}

void
draw_board ()
{
  int i = 0, j = 0;
  while (j * 4 + i < 16)
    {
      mvwaddch (gw, j + 1, i + 1, fsym[a[j * 4 + i]]);
      i++;
      if (i > 3)
	{
	  i = 0;
	  j++;
	}
    }
  wrefresh (gw);
}

void
mmask_buid ()
{
  int lz = 0;
  while (a[lz] != 0)
    lz++;
  if (lz < 4)
    mmask[2] = 0;
  else
    mmask[2] = 1;
  if (lz > 11)
    mmask[1] = 0;
  else
    mmask[1] = 1;
  if (lz != 0 && (lz + 1) % 4 == 0)
    mmask[3] = 0;
  else
    mmask[3] = 1;
  if (lz == 0 || lz % 4 == 0)
    mmask[0] = 0;
  else
    mmask[0] = 1;
  zls = lz;
}

void
swapvalues (int x, int y)
{
  int sv;
  sv = a[x];
  a[x] = a[y];
  a[y] = sv;
}

void
randomize_board ()
{
  int i, imax, j, mmk, mmd;
  imax = 1 + (int) (1024.0 * (rand () / (RAND_MAX + 1.0)));
  for (i = 0; i < imax; i++)
    {
      mmask_buid ();
      mmk = 0;
      for (j = 0; j < 4; j++)
	mmk += mmask[j];
      mmd = (int) (mmk * (rand () / (RAND_MAX + 1.0)));
      for (j = 0; j < 4; j++)
	{
	  if (mmask[j] == 1)
	    mmd--;
	  if (mmd == -1)
	    break;
	}
      switch (j)
	{
	case 0:
	  swapvalues (zls, zls - 1);
	  break;
	case 1:
	  swapvalues (zls, zls + 4);
	  break;
	case 2:
	  swapvalues (zls, zls - 4);
	  break;
	case 3:
	  swapvalues (zls, zls + 1);
	  break;
	}
    }
}

void
put_win_msg ()
{
  WINDOW *wmw;
  wmw = newwin (3, 13, LINES / 2 - 1, COLS / 2 - 7);
  box (wmw, 0, 0);
  mvwaddstr (wmw, 1, 1, "U did it!!!");
  wrefresh (wmw);
  wgetch (wmw);
  delwin (wmw);
}

int main ()
{
  srand (time (0));
  initscr ();
  clear ();
  curs_set (0);
  noecho ();
  gw = newwin (6, 6, LINES / 2 - 3, COLS / 2 - 3);
  box (gw, 0, 0);
  init_board ();
  randomize_board ();
  for (;;)
    {
      draw_board ();
      mmask_buid ();
      switch (wgetch (gw))
	{
	case 'q':
	  endwin ();
	  return 0;
	  break;
	case 'h':
	  if (!mmask[0])
	    break;
	  swapvalues (zls, zls - 1);
	  break;
	case 'j':
	  if (!mmask[1])
	    break;
	  swapvalues (zls, zls + 4);
	  break;
	case 'k':
	  if (!mmask[2])
	    break;
	  swapvalues (zls, zls - 4);
	  break;
	case 'l':
	  if (!mmask[3])
	    break;
	  swapvalues (zls, zls + 1);
	  break;
	}
      if (check_board ())
	{
	  put_win_msg ();
	  endwin ();
	  return (0);
	}
    }
}
