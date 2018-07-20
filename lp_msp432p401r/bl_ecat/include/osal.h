/******************************************************************************
 *                *          ***                    ***
 *              ***          ***                    ***
 * ***  ****  **********     ***        *****       ***  ****          *****
 * *********  **********     ***      *********     ************     *********
 * ****         ***          ***              ***   ***       ****   ***
 * ***          ***  ******  ***      ***********   ***        ****   *****
 * ***          ***  ******  ***    *************   ***        ****      *****
 * ***          ****         ****   ***       ***   ***       ****          ***
 * ***           *******      ***** **************  *************    *********
 * ***             *****        ***   *******   **  **  ******         *****
 *                           t h e  r e a l t i m e  t a r g e t  e x p e r t s
 *
 * http://www.rt-labs.com
 * Copyright (C) 2009. rt-labs AB, Sweden. All rights reserved.
 *------------------------------------------------------------------------------
 * $Id: osal.h 134 2012-04-27 07:33:57Z rtlaka $
 *------------------------------------------------------------------------------
 */

#ifndef _osal_
#define _osal_

extern int printf(const char *pcString, ...);

#define OSAL_PRINT printf

#ifdef _DEBUG
	#define DPRINT(...) OSAL_PRINT ("dbg: "__VA_ARGS__)
	#define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
	#define DPRINT(...)
	#define DEBUG_ASSERT(expression)
#endif  /* DEBUG */

#endif
