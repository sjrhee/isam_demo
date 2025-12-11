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
 *  Title:	ex6.c
 *  Sccsid:	@(#)chaining.c	9.1	1/7/93  15:57:40
 *  Description:
 * 		This program interactively reads data from stdin and adds
 *   		performance records to the "perform" file.
 *
 ***************************************************************************
 */
#include <isam.h>
#include <stdio.h>

#define WHOLEKEY 0
#define SUCCESS 0
#define TRUE 1
#define FALSE 0

char perfrec[51];
char operfrec[51];
char line[81];
int empnum;
double new_salary, old_salary;

struct keydesc key;
int fdemploy, fdperform;
int finished = FALSE;

/* This program interactively reads data from stdin and adds
   performance records to the "perform" file.  Depending on
   the rating given the employee on job performance, the
   following salary increases are placed in the salary field
   of the performance file.

         rating            percent increase
         ------            ----------------
         p (poor)                0.0 %
         f (fair)                4.5 %
         g (good)                7.5 %
*/

main()
{
   int cc;

   fdperform = cc = isopen("perform", ISINOUT + ISAUTOLOCK);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for performance file\n", iserrno);
      exit(1);
   }

   /* Set up key for isstart on performance file */
   key.k_flags = ISDUPS + DCOMPRESS;
   key.k_nparts = 2;
   key.k_part[0].kp_start = 0;
   key.k_part[0].kp_leng = 4;
   key.k_part[0].kp_type = LONGTYPE;
   key.k_part[1].kp_start = 4;
   key.k_part[1].kp_leng = 6;
   key.k_part[1].kp_type = CHARTYPE;

   getperformance();
   while (!finished)
   {
      if (get_old_salary())
      {
         finished = TRUE;
      }
      else
      {
         addperformance();
         getperformance();
      }
   }
   isclose(fdperform);
}

addperformance()
{
   int cc;

   cc = iswrite(fdperform, perfrec);
   if (cc != SUCCESS)
   {
      printf("iswrite error %d\n", iserrno);
      isclose(fdperform);
      exit(1);
   }
}

getperformance()
{
   printf("Employee number (enter 0 to exit): ");
   fgets(line, 80, stdin);
   sscanf(line, "%d", &empnum);
   if (empnum == 0)
   {
      finished = TRUE;
      return (0);
   }
   stlong(empnum, perfrec);

   printf("Review Date: ");
   fgets(line, 80, stdin);
   ststring(line, perfrec + 4, 6);

   printf("Job rating (p = poor, f = fair, g = good): ");
   fgets(line, 80, stdin);
   ststring(line, perfrec + 10, 1);

   printf("Salary After Review: ");
   printf("(Sorry, you don't get to add this)\n");
   new_salary = 0.0;
   stdbl(new_salary, perfrec + 11);

   printf("Title After Review: ");
   fgets(line, 80, stdin);
   ststring(line, perfrec + 19, 30);

   printf("\n\n\n");
}

get_old_salary()
{
   int mode, cc;

   bytecpy(perfrec, operfrec, 4);      /* get employee id no. */
   bytecpy("999999", operfrec + 4, 6); /* largest possible date */

   cc = isstart(fdperform, &key, WHOLEKEY, operfrec, ISGTEQ);
   if (cc != SUCCESS)
   {
      switch (iserrno)
      {
      case ENOREC:
      case EENDFILE:
         mode = ISLAST;
         break;
      default:
         printf("isstart error %d ", iserrno);
         return (1);
      }
   }
   else
   {
      mode = ISPREV;
   }

   cc = isread(fdperform, operfrec, mode);
   if (cc != SUCCESS)
   {
      printf("isread error %d in get_old_salary\n", iserrno);
      return (1);
   }
   if (cmpnbytes(perfrec, operfrec, 4))
   {
      printf("No performance record for employee number %d.\n",
             ldlong(operfrec));
      return (1);
   }
   else
   {
      printf("\nPerformance record found.\n\n");
      old_salary = new_salary = lddbl(operfrec + 11);
      printf("Rating: ");
      switch (*(perfrec + 10))
      {
      case 'p':
         printf("poor\n");
         break;
      case 'f':
         printf("fair\n");
         new_salary *= 1.045;
         break;
      case 'g':
         printf("good\n");
         new_salary *= 1.075;
         break;
      }
      stdbl(new_salary, perfrec + 11);
      printf("Old salary was %f\n", old_salary);
      printf("New salary is %f\n", new_salary);
      return (0);
   }
}

bytecpy(src, dest, n) register char *src;
register char *dest;
register int n;
{
   while (n-- > 0)
   {
      *dest++ = *src++;
   }
}

cmpnbytes(byte1, byte2, n) register char *byte1, *byte2;
register int n;
{
   if (n <= 0)
      return (0);
   while (*byte1 == *byte2)
   {
      if (--n == 0)
         return (0);
      ++byte1;
      ++byte2;
   }
   return (((*byte1 & BYTEMASK) < (*byte2 & BYTEMASK)) ? -1 : 1);
}

ststring(src, dest, num)
    /* move NUM sequential characters from SRC to DEST */
    char *src;
char *dest;
int num;
{
   int i;

   for (i = 1; i <= num && *src != '\n' && src != 0; i++)
      /* don't move carriage */
      *dest++ = *src++; /* returns or nulls    */
   while (i++ <= num)   /* pad remaining characters in blanks */
      *dest++ = ' ';
}
