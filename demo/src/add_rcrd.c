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
 *  Title:	ex3.c
 *  Sccsid:	@(#)add_rcrd.c	9.1	1/7/93  15:56:23
 *  Description:
 * 		This program adds a new employee record to the employee
 * 		file.  It also adds that employee's first employee
 * 		performance record to the performance file.
 *
 ***************************************************************************
 */
#include <isam.h>
#include <stdio.h>

#define WHOLEKEY 0
#define SUCCESS 0
#define TRUE 1
#define FALSE 0

char emprec[85];
char perfrec[51];
char line[82];
int  empnum;

struct keydesc key;
int fdemploy, fdperform;
int finished = FALSE;

/* This program adds a new employee record to the employee
   file.  It also adds that employee's first employee
   performance record to the performance file.
*/

main()
{
int cc;

fdemploy = cc = isopen("employee", ISAUTOLOCK+ ISOUTPUT);
if (cc < SUCCESS)
   {
   printf("isopen error %d for employee file\n", iserrno);
   exit(1);
   }
fdperform = cc = isopen("perform", ISAUTOLOCK + ISOUTPUT);
if (cc < SUCCESS)
   {
   printf("isopen error %d for performance file\n", iserrno);
   exit(1);
   }
getemployee();
getperform();

while(!finished)
   {
   addemployee();
   addperform();
   getemployee();
   getperform();
   }
isclose(fdemploy);
isclose(fdperform);
}

getperform()
{
double new_salary;

if (empnum == 0)
   {
   finished = TRUE;
   return(0);
   }
stlong(empnum, perfrec);

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
   isclose(fdemploy);
   exit(1);
   }
}
addperform()
{
int cc;

cc = iswrite(fdperform, perfrec);
if (cc != SUCCESS)
   {
   printf("iswrite error %d for performance\n", iserrno);
   isclose(fdperform);
   exit(1);
   }
}

putnc(c,n)
char *c;
int n;
{
while (n--) putchar(*(c++));
}

getemployee()
{
printf("Employee number (enter 0 to exit): ");
fgets(line, 80, stdin);
sscanf(line, "%d", &empnum);
if (empnum == 0)
   {
   finished = TRUE;
   return(0);
   }
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
   *dest++ = *src++;         /* returns or nulls    */
while (i++ <= num)           /* pad remaining characters in blanks */
   *dest++ = ' ';
}
