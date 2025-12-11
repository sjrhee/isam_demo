/***************************************************************************
 *
 *                INTERNATIONAL BUSINESS MACHINES CORPORATION
 *
 *                            PROPRIETARY DATA
 *
 *      THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF
 *      IBM CORPORATION.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *      CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR
 *      DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT
 *      SIGNED BY AN OFFICER OF IBM CORPORATION.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE.
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *  C-ISAM  --  Indexed Sequential Access Method
 *
 *  Title:	ex5.c
 *  Sccsid:	@(#)sqntl_rd.c	9.1	1/7/93  15:57:00
 *  Description:
 * 		This program sequentially reads through the employee
 *  		 file by employee number printing each record
 *
 ***************************************************************************
 */
#include <isam.h>

#define WHOLEKEY 0
#define SUCCESS 0
#define TRUE 1
#define FALSE 0

char emprec[85];

struct keydesc key;
int fdemploy, fdperform;
int eof = FALSE;

/* This program sequentially reads through the employee
   file by employee number printing each record        */

main()
{
   int cc;

   fdemploy = cc = isopen("employee", ISMANULOCK + ISINOUT);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for employee file\n", iserrno);
      exit(1);
   }

   /* Set File to Retrieve using Last Name Index */
   key.k_flags = ISDUPS + COMPRESS;
   key.k_nparts = 1;
   key.k_part[0].kp_start = 4;
   key.k_part[0].kp_leng = 20;
   key.k_part[0].kp_type = CHARTYPE;
   cc = isstart(fdemploy, &key, WHOLEKEY, emprec, ISFIRST);
   if (cc != SUCCESS)
   {
      printf("isstart error %d\n", iserrno);
      isclose(fdemploy);
      exit(1);
   }

   getfirst();
   while (!eof)
   {
      showemployee();
      getnext();
   }
   isclose(fdemploy);
}

showemployee()
{
   printf("Employee number: %d", ldlong(emprec));
   printf("\nLast name: ");
   putnc(emprec + 4, 20);
   printf("\nFirst name: ");
   putnc(emprec + 24, 20);
   printf("\nAddress: ");
   putnc(emprec + 44, 20);
   printf("\nCity: ");
   putnc(emprec + 64, 20);
   printf("\n\n\n");
}

putnc(c, n) char *c;
int n;
{
   while (n--)
      putchar(*(c++));
}

getfirst()
{
   int cc;

   if (cc = isread(fdemploy, emprec, ISFIRST))
   {
      switch (iserrno)
      {
      case EENDFILE:
         eof = TRUE;
         break;
      default:
      {
         printf("isread ISFIRST error %d \n", iserrno);
         eof = TRUE;
         return (1);
      }
      }
   }
   return (0);
}

getnext()
{
   int cc;

   if (cc = isread(fdemploy, emprec, ISNEXT))
   {
      switch (iserrno)
      {
      case EENDFILE:
         eof = TRUE;
         break;
      default:
      {
         printf("isread ISNEXT error %d \n", iserrno);
         eof = TRUE;
         return (1);
      }
      }
   }
   return (0);
}
