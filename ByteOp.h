//
// Created by oli_d on 11/26/2022.
//

#ifndef LAB3_SYSEXP_BYTEOP_H
#define LAB3_SYSEXP_BYTEOP_H

#include <fstream>

////////////////////////////////////////////////////////////////
//Cette fonction retourne la valeur du byte signé
//Créé par:
//Date:
//Modifié par:
//Description:
////////////////////////////////////////////////////////////////
int fct_SignedByte(int page, int offset);
////////////////////////////////////////////////////////////////
//Cette fonction créé un masque afin de lire les bits nécessaires. NE PAS MODIFIER ET UTILISER TEL QUEL DANS LE MAIN
//Créé par: Sara Séguin
//Modifié par:
//Description:
////////////////////////////////////////////////////////////////
unsigned createMask(unsigned a, unsigned b);

#endif //LAB3_SYSEXP_BYTEOP_H
