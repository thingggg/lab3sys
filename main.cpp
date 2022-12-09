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
#include <map>
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
    unsigned char frame;
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

//std::map<unsigned char, char [256]> MemPhysique; //Mémoire physique
char MemPhysique[256][256];
std::deque<TLB> TLBuffer;
unsigned short tablePage[256][2]={0}; //Table de page
double PageRequest = 0;
double PageFault = 0;
double PageAlreadyInTlb = 0;
bool checkTLB(virt v){

    for (auto i = TLBuffer.begin(); i != TLBuffer.end(); ++i) {
        if (i->page == v.bits_page) {
            ++PageAlreadyInTlb;
            printf("addresseVirtuelle: %i addresse physique: %i valeur %i \n",  v.address , tablePage[v.bits_page][0] +v.bits_offset, MemPhysique[(tablePage[v.bits_page][0] - v.bits_offset)/256][v.bits_offset]);
            TLBuffer.emplace_back(*i);
            TLBuffer.erase(i);
            return true;
        }
    }
    return false;
}
void pageFault(virt v){
    static int cpt = 0;
    char ch[256] = {};

    //std::cout << "Page non-chargée dans la table" << std::endl;
    std::fstream myfile;
    myfile.open("../simuleDisque.bin");
    if (tablePage[v.bits_page][1] != 1){
        myfile.seekg(v.bits_page * 256, std::ios::beg); //Trouver l'endroit correspondant au byte signé dans le fichier
        myfile.read(MemPhysique[cpt],256); //Lire cet emplacement
        tablePage[v.bits_page][1] = 1;
        tablePage[v.bits_page][0] = cpt*256;
        TLB temp{};
        temp.page = v.bits_page;
        TLBuffer.emplace_back(temp);
        printf("addresseVirtuelle: %i addresse physique: %i valeur %i \n",  v.address , tablePage[v.bits_page][0] +v.bits_offset , MemPhysique[cpt][v.bits_offset]);
        cpt++;
        PageFault++;
    }
    else{
        TLB temp{};
        temp.page = v.bits_page;
        TLBuffer.emplace_back(temp);
        printf("addresseVirtuelle: %i addresse physique: %i valeur %i \n",  v.address , tablePage[v.bits_page][0] +v.bits_offset, MemPhysique[(tablePage[v.bits_page][0] - v.bits_offset)/256][v.bits_offset]);
    }

    myfile.close();
}
int main()
{
    int MaxTLBSize = 16;
    //Initialisation et déclarations
    std::vector<virt> virtuelle;

    ChargerAdressesPhysiques(); //Lire le fichier d'adresses à traduire

    //Traduire l'adresse physique en adresse logique
    for(auto i : addresseLog){
        nombreBinaire.emplace_back(i); // Traduire et Stocker les nombres binaires dans un vecteur
    }
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
        PageRequest++;
        // Est ce que la page est dans la TLB
        if(checkTLB(v)) continue;
        // Est ce que la TLB est pleine
        if(TLBuffer.size() >= MaxTLBSize){
            TLBuffer.pop_front();
        }
        //ajouter la page a la TLB
        pageFault(v);
    }
    printf("Page fault to request: %f%%      page Already in TLB: %f%%     ", PageFault/10, PageAlreadyInTlb/10);
    //Ecrire le fichier de sortie

    return 0;

}


