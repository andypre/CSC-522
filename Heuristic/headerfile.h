// This code was taken from Brendan McKay's nauty
/*****************************************************************************
*                                                                            *
*   Conventions and Assumptions:                                             *
*                                                                            *
*       A 'longword' is the chunk of memory that is occupied by a long int   *
*       We assume that longwords have at least 32 bits.
*       The rightmost 32 bits of longwords are numbered 0..31, left to       *
*       right.                                                               *
*                                                                            *
*       The int variables m and n have consistent meanings througout.        *
*       Graphs have n vertices always, and sets have m longwords.            *
*                                                                            *
*       A 'set' consists of m contiguous longwords, whose bits are numbered  *
*       0,1,2,... from left (high-order) to right (low-order), using only    *
*       the rightmost 32 bits of each longword.  It is used to represent a   *
*       subset of {0,1,...,n-1} in the usual way - bit number x is 1 iff x   *
*       is in the subset.  Bits numbered n or greater, and unnumbered bits,  *
*       are assumed permanently zero.                                        *
*                                                                            *
*****************************************************************************/

#include <stdio.h> 
#include<stdlib.h>
#include <math.h>

#define NMAX 800
#define MMAX ((NMAX +31)/ 32)

/* set operations (setadd is its address, pos is the bit number): */

#define SETWD(pos) ((pos)>>5)    /* number of longword containing bit pos */
#define SETBT(pos) ((pos)&037)   /* position within longword of bit pos */
#define ADD_ELEMENT(setadd,pos) ((setadd)[SETWD(pos)] |= bit[SETBT(pos)])
#define DEL_ELEMENT(setadd,pos) ((setadd)[SETWD(pos)] &= ~bit[SETBT(pos)])
#define IS_ELEMENT(setadd,pos) ((setadd)[SETWD(pos)] & bit[SETBT(pos)])

/* number of 1-bits in longword x */
#define POP_COUNT(x) bytecount[(x)>>24 & 0377] + bytecount[(x)>>16 & 0377] \
                        + bytecount[(x)>>8 & 0377] + bytecount[(x) & 0377]

#if MAIN
int bytecount[] =        {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                          4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

int bit[] = {020000000000,010000000000,04000000000,02000000000,01000000000,
              0400000000,0200000000,0100000000,040000000,020000000,010000000,
              04000000,02000000,01000000,0400000,0200000,0100000,040000,020000,
              010000,04000,02000,01000,0400,0200,0100,040,020,010,04,02,01};

#else
    extern int bytecount[];
    extern int bit[];
#endif

// Functions in nkaur_h.cpp
int read_graph(int *n, int *m, int G[NMAX][MMAX]);
void print_graph(int n, int G[NMAX][MMAX]);
int set_size(int n, int set[]);
void print_set(int n, int set[]);
int find_dom_set(int n,int m,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size);
int random_dom_set(int n,int m,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size);
int better_dom(int n,int m,int u,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size);
int print_ver(int status, int *min_size,int n, int min_dom_set[]);
