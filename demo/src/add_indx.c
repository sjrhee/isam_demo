/***************************************************************************
 *
 *		  INTERNATIONAL BUSINESS MACHINES CORPORATION 
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	IBM CORPORATION.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF IBM CORPORATION.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *  C-ISAM  --  Indexed Sequential Access Method
 *
 *  Title:	ex2.c
 *  Sccsid:	@(#)add_indx.c	9.1	1/7/93  15:56:11
 *  Description:
 * 		This program adds secondary indexes for the last name
 * 		field in the employee file, and the salary field in
 * 		the performance file.
 *
 ***************************************************************************
 */
#include <isam.h>

#define SUCCESS 0

struct keydesc lnkey, skey;
int fdemploy, fdperform;

/* This program adds secondary indexes for the last name
   field in the employee file, and the salary field in
   the performance file.
*/

main()
{
int cc;
fdemploy = cc = isopen("employee", ISINOUT + ISEXCLLOCK);
if (cc < SUCCESS)
   {
   printf("isopen error %d for employee file\n", iserrno);
   exit(1);
   }

/* Set up Last Name Key */
lnkey.k_flags = ISDUPS + COMPRESS;
lnkey.k_nparts = 1;
lnkey.k_part[0].kp_start = 4;
lnkey.k_part[0].kp_leng = 20;
lnkey.k_part[0].kp_type = CHARTYPE;

cc = isaddindex(fdemploy, &lnkey);
if (cc != SUCCESS)
   {
   printf("isaddindex error %d for employee lname key\n", iserrno);
   exit(1);
   }
isclose(fdemploy);

fdperform = cc = isopen("perform", ISINOUT + ISEXCLLOCK);
if (cc < SUCCESS)
   {
   printf("isopen error %d for performance file\n", iserrno);
   exit(1);
   }

/* Set up Salary Key */
skey.k_flags = ISDUPS;
skey.k_nparts = 1;
skey.k_part[0].kp_start = 11;
skey.k_part[0].kp_leng = sizeof(double);
skey.k_part[0].kp_type = DOUBLETYPE;

cc = isaddindex(fdemploy, &skey);
if (cc != SUCCESS)
   {
   printf("isaddindex error %d for perform sal key\n", iserrno);
   exit(1);
   }
isclose(fdperform);
}
