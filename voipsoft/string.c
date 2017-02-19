

#include "type.h"

void memcpy_c2d(PCCHAR pDst, PCCHAR pSrc, unsigned char iCount)
{
	while (iCount --)
	{
		* pDst ++ = * pSrc ++;
	}
}

unsigned char memcmp_c2d(PCCHAR pDst, PCCHAR pSrc, unsigned char iCount)
{
	while (-- iCount && * pDst == * pSrc) 
	{
		pDst ++;
		pSrc ++;
	}

	return(* pDst - * pSrc);
}

void memset_d(PCCHAR pDst, unsigned char iVal, unsigned char iCount)
{
	while (-- iCount) 
	{
		* pDst ++ = iVal;
	}
}
