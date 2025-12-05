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
 *  Title:	ex1.c
 *  Sccsid:	@(#)bld_file.c	9.1	1/7/93  15:55:38
 *  Description:
 * 			This program builds the C-ISAM file systems for the
 *			employee and perform files
 *
 ***************************************************************************
 */
#include <isam.h>

#define SUCCESS 0

struct keydesc ekey, pkey;
int cc, fdemploy, fdperform;

/*
   This program builds the C-ISAM file systems for the
   employee and perform files
*/

main()
{
/* Set up Employee Key */
ekey.k_flags = ISNODUPS;
ekey.k_nparts = 1;
ekey.k_part[0].kp_start = 0;
ekey.k_part[0].kp_leng = 4;
ekey.k_part[0].kp_type = LONGTYPE;

fdemploy = cc = isbuild("employee", 84, &ekey,
                         ISINOUT + ISEXCLLOCK);
if (cc < SUCCESS)
   {
   printf("isbuild error %d for employee file\n",
           iserrno);
   exit(1);
   }
isclose(fdemploy);

/* Set up Performance Key */
pkey.k_flags = ISDUPS+DCOMPRESS;
pkey.k_nparts = 2;
pkey.k_part[0].kp_start = 0;
pkey.k_part[0].kp_leng = 4;
pkey.k_part[0].kp_type = LONGTYPE;
pkey.k_part[1].kp_start = 4;
pkey.k_part[1].kp_leng = 6;
pkey.k_part[1].kp_type = CHARTYPE;

fdperform = cc = isbuild("perform", 49, &pkey, ISINOUT + ISEXCLLOCK);
if (cc < SUCCESS)
   {
   printf("isbuild error %d for performance file\n", iserrno);
   exit(1);
   }
isclose(fdperform);
}
