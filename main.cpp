/*
main.cpp
@author: Olivier Dion
Date: ***
Date de modification: ***
Description: ***
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <bitset>
#include <fstream>
#include <string>
#include <deque>
#include "ByteOp.h"

std::vector<int>addresseLog;
std::vector<int>addressePhysique; //Adresses Logiques
std::vector<std::bitset<16>>nombreBinaire; //Adresses Logiques

struct virt {
    int address;
    int bits_page;
    int bits_offset;
};
struct TLB{
    unsigned char page;
    unsigned char segment;
};

void ChargerAdressesPhysiques(){
    std::fstream myfile;
    myfile.open("../addresses.txt");
    unsigned int a;
    if (myfile.is_open())
        while (myfile >> a)
        {
            addresseLog.push_back(a);
        }
    else
        printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
}
std::deque<TLB> TLBuffer;
bool checkTLB(int bitspage){

    for (auto i = TLBuffer.begin(); i != TLBuffer.end(); ++i) {
        if (i->page == bitspage) {
            std::cout << "Page chargée dans la table" << std::endl;
            TLBuffer.erase(i);
            TLBuffer.emplace_back(*i);
            return true;
        }
    }
    return false;
}
void pageFault(virt v){
    char memPhysique[256] = {0}; //Mémoire physique
    static int cpt = 0;
    //std::cout << "Page non-chargée dans la table" << std::endl;
    std::fstream myfile;
    myfile.open("../simuleDisque.bin");
    if (myfile.is_open()){
        myfile.seekg(v.bits_page * 256 + v.bits_offset, std::ios::beg); //Trouver l'endroit correspondant au byte signé dans le fichier
        myfile.read(reinterpret_cast<char *>(&memPhysique[v.bits_page]),1); //Lire cet emplacement
        //tablePage[v.bits_page][1] = 1;
        //tablePage[v.bits_page][0] = cpt*256+v.bits_offset;
        TLB temp{};
        temp.page = v.bits_page;
        TLBuffer.emplace_back(temp);
        cpt++;
        printf("addresseVirtuelle: %i addresse physique: %i valeur:  %i bits_page: %i \n",  v.address , 0,memPhysique[v.bits_page]);
    }
    else
        printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
    //Fermer le fichier
    myfile.close();
}
int main()
{
    int MaxTLBSize = 16;
    //Initialisation et déclarations
    char memPhysique[256] = {0}; //Mémoire physique
    unsigned short tablePage[256][2]={0}; //Table de page

    std::vector<virt> virtuelle;

    ChargerAdressesPhysiques(); //Lire le fichier d'adresses à traduire

    //Traduire l'adresse physique en adresse logique
    for(auto i : addresseLog){
        nombreBinaire.emplace_back(i); // Traduire et Stocker les nombres binaires dans un vecteur
    }
    std::bitset<32> OffsetMask = createMask(0,7); //Crééer un masque pour lire juste les bits 0 à 7 (offset)
    std::bitset<32> PageMask = createMask(8,15); //Créer un masque pour lire juste les bits 8 à 15 (page)

    //Boucler sur les x adresses
    for(auto add : nombreBinaire)
    {
        std::string a = add.to_string();
        std::bitset<8> page( a, 0,8 ); //applique un masque qui dévoile la page
        std::bitset<8> offset(a, 8); //applique un masque qui dévoile l'offset'
        virt temp{(int)(add.to_ulong()),(int)(page.to_ulong()),(int)(offset.to_ulong())};
        virtuelle.emplace_back(temp);
    }
    //Table de pages
    //Une adresse à la fois, vérifier si elle est dans la table de page
    int cpt = 0;
    for(virt v : virtuelle)
    {
        // Est ce que la page est dans la TLB
        if(checkTLB(v.bits_page)) continue;
        // Est ce que la TLB est pleine
        if(TLBuffer.size() >= MaxTLBSize){
            TLBuffer.pop_front();
        }
        //ajouter la page a la TLB
        pageFault(v);



    }


    //Calcul de l'adresse physique
//    for(int i=0;i<bits_page.size();i++)
//    {
//        //Construire en bits et traduire en décimal
//
//        //Obtenir la valeur du byte signé
//        int b = fct_SignedByte(bits_page[i],bits_offset[i]);
//
//    }



    //Ecrire le fichier de sortie



    return 0;

}


