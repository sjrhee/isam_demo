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
 *  Title:	ex4.c
 *  Sccsid:	@(#)upd_file.c	9.1	1/7/93  15:56:52
 *  Description:
 *  		This program updates the employee file.  If the delete
 *		option is requested, all performance records are removed
 * 		along with the employee record.
 *
 ***************************************************************************
 */
#include <isam.h>
#include <stdio.h>

#define WHOLEKEY 0
#define SUCCESS 0
#define TRUE 1
#define FALSE 0
#define DELETE 1
#define UPDATE 2

char emprec[85];
char perfrec[51];
char line[82];
int empnum;

struct keydesc pkey;
int fdemploy, fdperform;
int finished = FALSE;

/*  This program updates the employee file.
    If the delete option is requested, all
    performance records are removed along
    with the employee record.
*/
main()
{
   int cc;
   int cmd;

   fdemploy = cc = isopen("employee", ISMANULOCK + ISINOUT);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for employee file\n", iserrno);
      fatal();
   }

   fdperform = cc = isopen("perform", ISMANULOCK + ISINOUT);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for performance file\n", iserrno);
      fatal();
   }
   /* Set up key description structure for isstart */
   pkey.k_flags = ISDUPS + DCOMPRESS;
   pkey.k_nparts = 2;
   pkey.k_part[0].kp_start = 0;
   pkey.k_part[0].kp_leng = 4;
   pkey.k_part[0].kp_type = LONGTYPE;
   pkey.k_part[1].kp_start = 4;
   pkey.k_part[1].kp_leng = 6;
   pkey.k_part[1].kp_type = CHARTYPE;

   cmd = getinstr();

   while (!finished)
   {
      if (cmd == DELETE)
         delrec();
      else
      {
         getemployee();
         updatemp();
      }
      cmd = getinstr();
   }
   isclose(fdemploy);
   isclose(fdperform);
}
updatemp()
{
   int cc;

   cc = isrewrite(fdemploy, emprec);
   if (cc != SUCCESS)
   {
      printf("isrewrite error %d for employee\n", iserrno);
      fatal();
   }
}

delrec()
{
   int cc;

   cc = isdelete(fdemploy, emprec);
   if (cc != SUCCESS)
   {
      printf("isdelete error %d for performance\n", iserrno);
      fatal();
   }

   cc = isstart(fdperform, &pkey, 4, perfrec, ISEQUAL);
   if (cc < SUCCESS)
      fatal();
   cc = isread(fdperform, perfrec, ISCURR + ISLOCK);
   if (cc < SUCCESS)
      fatal();

   while (cc == SUCCESS)
   {
      cc = isdelcurr(fdperform);
      if (cc < SUCCESS)
      {
         printf("isdelcurr error %d for perform\n", iserrno);
         fatal();
      }
      cc = isstart(fdperform, &pkey, 4, perfrec, ISEQUAL);
      if (cc == SUCCESS)
         cc = isread(fdperform, perfrec, ISCURR + ISLOCK);
   }
   if (iserrno != ENOREC && iserrno != EENDFILE)
   {
      printf("isread error %d for perform\n", iserrno);
      fatal();
   }

   isrelease(fdemploy);
   isrelease(fdperform);
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

getinstr()

{
   int cc;
   char instr[2];

tryagain:
   printf("Employee number (enter 0 to exit): ");
   fgets(line, 80, stdin);
   sscanf(line, "%d", &empnum);
   if (empnum == 0)
   {
      finished = TRUE;
      return (0);
   }

   stlong(empnum, emprec);
   stlong(empnum, perfrec);
   cc = isread(fdemploy, emprec, ISEQUAL + ISLOCK);
   if (cc < SUCCESS)
   {
      if (iserrno == ENOREC || iserrno == EENDFILE)
      {
         printf("Employee No. Not Found\n");
         goto tryagain;
      }
      else

      {
         printf("isread error %d for employee file\n", iserrno);
         fatal();
      }
   }
   showemployee();
   printf("Delete? (y/n): ");
   fgets(line, 80, stdin);
   sscanf(line, "%1s", instr);
   if (strcmp(instr, "y") == 0)
      return (DELETE);
   else
   {
      printf("Update? (y/n): ");
      fgets(line, 80, stdin);
      sscanf(line, "%1s", instr);
      if (strcmp(instr, "y") == 0)
         return (UPDATE);
   }
   goto tryagain;
}

getemployee()
{
   int len;

   printf("Last name: ");
   fgets(line, 80, stdin);
   len = strlen(line);
   if (len > 1)
      ststring(line, emprec + 4, 20);

   printf("First name: ");
   fgets(line, 80, stdin);
   len = strlen(line);
   if (len > 1)
      ststring(line, emprec + 24, 20);

   printf("Address: ");
   fgets(line, 80, stdin);
   len = strlen(line);
   if (len > 1)
      ststring(line, emprec + 44, 20);

   printf("City: ");
   fgets(line, 80, stdin);
   len = strlen(line);
   if (len > 1)
      ststring(line, emprec + 64, 20);

   printf("\n\n\n");
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
fatal()
{
   isclose(fdemploy);
   isclose(fdperform);
   exit(1);
}
