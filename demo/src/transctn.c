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
 *  Title:	ex7.c
 *  Sccsid:	@(#)transctn.c	9.1	1/7/93  15:58:18
 *  Description:
 * 		This program adds a new employee record to the employee
 *   		file.  It also adds that employee's first employee
 *   		performance record to the performance file.
 *
 ***************************************************************************
 */
#include <isam.h>
#include <stdio.h>
#include <errno.h>

#define SUCCESS 0
#define LOGNAME "recovery.log"

char emprec[85];
char perfrec[51];
char line[82];
int  empnum;
int fdemploy, fdperform;


/* This program adds a new employee record to the employee
   file.  It also adds that employee's first employee
   performance record to the performance file.
*/

main()
{
int cc;
int cc1;
int cc2;
if (access(LOGNAME, 0) == -1)			/* log file exist? */
    if ((cc = creat(LOGNAME, 0660)) == -1)
	{
	printf("Cannot create log file \"%s\", system error %d.\n",
		LOGNAME, errno);
	iscleanup();
	exit(1);
	}
/* open log file */
cc = islogopen (LOGNAME);
if (cc < SUCCESS)
   {
   printf ("Cannot open log file, islogopen error %d\n", iserrno);
   iscleanup();
   exit (1);
   }

while(!getemployee())
   {

/* Transaction begins after terminal input has been collected.
   Either both employee and performance record will be added
   or neither will be added.                                */

/* Files must be opened and closed within the transaction */

   isbegin();    /* start of transaction */

   fdemploy = cc = isopen("employee", ISMANULOCK+ISOUTPUT+ISTRANS);
   if (cc < SUCCESS)
      { isrollback();
	break; }

   fdperform = cc = isopen("perform", ISMANULOCK+ISOUTPUT+ISTRANS);
   if (cc < SUCCESS)
      { isclose(fdemploy);
	isrollback();
        break; }

   cc1 =addemployee();
   if (cc1 == SUCCESS)
      cc2 =addperform();

  isclose(fdemploy);
  isclose(fdperform);

   if ((cc1 < SUCCESS) || (cc2 < SUCCESS)) /* transaction failed */
      {
      isrollback();
      }
   else	
      {
      iscommit();      /* transaction okay   */
      printf ("new employee entered\n");
      }
   }

/*  Finished */
islogclose();
iscleanup();
exit (0);
}

getperform()
{
double new_salary;

printf("Start Date: ");
fgets(line, 80, stdin);
ststring(line, perfrec+4, 6);

ststring("g", perfrec+10, 1);

printf("Starting salary: ");
fgets(line, 80, stdin);
sscanf(line, "%lf", &new_salary);
stdbl(new_salary, perfrec+11);

printf("Title : ");
fgets(line, 80, stdin);
ststring(line, perfrec+19, 30);

printf("\n\n\n");
}

addemployee()
{
int cc;
cc = iswrite(fdemploy, emprec);
if (cc != SUCCESS)
   {
   printf("iswrite error %d for employee\n", iserrno);
   }
return (cc);
}

addperform()
{
int cc;
cc = iswrite(fdperform, perfrec);
if (cc != SUCCESS)
   {
   printf("iswrite error %d for performance\n", iserrno);
   }
return (cc);
}

getemployee()
{
printf("Employee number (enter 0 to exit): ");
fgets(line, 80, stdin);
sscanf(line, "%d", &empnum);

if (empnum == 0)
   return(1);

stlong(empnum, emprec);

printf("Last name: ");
fgets(line, 80, stdin);
ststring(line, emprec+4, 20);

printf("First name: ");
fgets(line, 80, stdin);
ststring(line, emprec+24, 20);

printf("Address: ");
fgets(line, 80, stdin);
ststring(line, emprec+44, 20);

printf("City: ");
fgets(line, 80, stdin);
ststring(line, emprec+64, 20);

getperform();
printf("\n\n\n");

return (0);
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
   *dest++ = *src++;                   /* returns or nulls    */
   while (i++ <= num)   /* pad remaining characters in blanks */
   *dest++ = ' ';
}
