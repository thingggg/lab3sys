//
// Created by oli_d on 11/26/2022.
//

#include "ByteOp.h"
unsigned createMask(unsigned a, unsigned b)
{
    unsigned r = 0;
    for(unsigned i=a;i<=b;i++)
    {
        r |= 1 << i;
    }
    return r;
}
int fct_SignedByte(int page, int offset)
{
    unsigned int LENGTH = 1; //Le byte signé a une longueur de 1 byte

    //Retourner la valeur du byte signé
    return 0;
}