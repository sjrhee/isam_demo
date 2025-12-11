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
 *  C-ISAM  --  Indexed Sequential Access Method
 *
 *  Title:  add_rcrd.c
 *  Description:
 *      This program adds a new employee record to the employee
 *      file.  It also adds that employee's first employee
 *      performance record to the performance file.
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <isam.h>

#define SUCCESS 0
#define TRUE 1
#define FALSE 0

/* Employee Record Offsets and Size */
#define EMP_REC_SIZE 84
#define EMP_ID_OFF 0
#define EMP_LNAME_OFF 4
#define EMP_FNAME_OFF 24
#define EMP_ADDR_OFF 44
#define EMP_CITY_OFF 64

/* Performance Record Offsets and Size */
#define PERF_REC_SIZE 49
#define PERF_ID_OFF 0
#define PERF_DATE_OFF 4
#define PERF_GRADE_OFF 10
#define PERF_SAL_OFF 11
#define PERF_TITLE_OFF 19

/* Global variables */
char emprec[EMP_REC_SIZE + 1];
char perfrec[PERF_REC_SIZE + 1];
char line[82];
long empnum;
int fdemploy, fdperform;
int finished = FALSE;

/* Function Prototypes */
void getemployee(void);
void getperform(void);
void addemployee(void);
void addperform(void);
void ststring(char *src, char *dest, int num);

/*
 * Main Entry Point
 */
int main(void)
{
   int cc;

   /* Open Employee File */
   fdemploy = cc = isopen("employee", ISAUTOLOCK + ISOUTPUT);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for employee file\n", iserrno);
      exit(1);
   }

   /* Open Performance File */
   fdperform = cc = isopen("perform", ISAUTOLOCK + ISOUTPUT);
   if (cc < SUCCESS)
   {
      printf("isopen error %d for performance file\n", iserrno);
      /* Close employee input before exit */
      isclose(fdemploy);
      exit(1);
   }

   /* Initial reads */
   getemployee();
   getperform();

   /* Loop until finished */
   while (!finished)
   {
      addemployee();
      addperform();
      getemployee();
      getperform();
   }

   /* Cleanup */
   isclose(fdemploy);
   isclose(fdperform);

   return 0;
}

/*
 * Get Performance Record Input
 */
void getperform(void)
{
   double new_salary;

   if (empnum == 0)
   {
      finished = TRUE;
      return;
   }

   /* Store Employee Number (Long) */
   stlong(empnum, perfrec + PERF_ID_OFF);

   printf("Start Date: ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, perfrec + PERF_DATE_OFF, 6);
   }

   /* Grade is fixed as 'g' in this demo */
   ststring("g", perfrec + PERF_GRADE_OFF, 1);

   printf("Starting salary: ");
   if (fgets(line, sizeof(line), stdin))
   {
      sscanf(line, "%lf", &new_salary);
      stdbl(new_salary, perfrec + PERF_SAL_OFF);
   }

   printf("Title : ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, perfrec + PERF_TITLE_OFF, 30);
   }

   printf("\n\n\n");
}

/*
 * Write Employee Record to ISAM file
 */
void addemployee(void)
{
   int cc;

   cc = iswrite(fdemploy, emprec);
   if (cc != SUCCESS)
   {
      printf("iswrite error %d for employee\n", iserrno);
      isclose(fdemploy);
      isclose(fdperform);
      exit(1);
   }
}

/*
 * Write Performance Record to ISAM file
 */
void addperform(void)
{
   int cc;

   cc = iswrite(fdperform, perfrec);
   if (cc != SUCCESS)
   {
      printf("iswrite error %d for performance\n", iserrno);
      isclose(fdemploy);
      isclose(fdperform);
      exit(1);
   }
}

/*
 * Get Employee Record Input
 */
void getemployee(void)
{
   printf("Employee number (enter 0 to exit): ");
   if (fgets(line, sizeof(line), stdin))
   {
      sscanf(line, "%ld", &empnum); /* Changed %d to %ld for long */
   }

   if (empnum == 0)
   {
      finished = TRUE;
      return;
   }

   stlong(empnum, emprec + EMP_ID_OFF);

   printf("Last name: ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, emprec + EMP_LNAME_OFF, 20);
   }

   printf("First name: ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, emprec + EMP_FNAME_OFF, 20);
   }

   printf("Address: ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, emprec + EMP_ADDR_OFF, 20);
   }

   printf("City: ");
   if (fgets(line, sizeof(line), stdin))
   {
      ststring(line, emprec + EMP_CITY_OFF, 20);
   }

   printf("\n\n\n");
}

/*
 * move NUM sequential characters from SRC to DEST
 * Note: Pads with spaces if src is shorter than num.
 * Stops copying at first newline or null.
 */
void ststring(char *src, char *dest, int num)
{
   int i;

   /* Copy logical characters */
   for (i = 1; i <= num && *src != '\n' && *src != '\0'; i++)
   {
      *dest++ = *src++;
   }

   /* Pad remaining with spaces */
   while (i++ <= num)
   {
      *dest++ = ' ';
   }
}
