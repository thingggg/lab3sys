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
#include "ByteOp.h"
std::vector<int>addressePhysiques;
std::vector<int>addresseLogique; //Adresses Logiques
std::vector<std::bitset<16>>nombreBinaire; //Adresses Logiques

struct virt {
    int bits_page; //Adresses Logiques
    int bits_offset; //Adresses Logiques
};
void ChargerAdressesPhysiques(){
    std::fstream myfile;
    myfile.open("../addresses.txt");
    unsigned int a;
    if (myfile.is_open())
        while (myfile >> a)
        {
            addressePhysiques.push_back(a);
        }
    else
        printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
}
void display(std::vector<int> a, std::vector<int> b){
    while(!a.empty()){
        printf("   page:   %d offset:  %d \n", a.back(), b.back());
        a.pop_back();
        b.pop_back();
    }
}
int main()
{
    //Initialisation et déclarations
    int memPhysique[256] = {0}; //Mémoire physique
    char tablePage[256][2]={0}; //Table de page
    std::vector<virt> virtuelle;

    ChargerAdressesPhysiques(); //Lire le fichier d'adresses à traduire

    //Traduire l'adresse physique en adresse logique
    for(auto i : addressePhysiques){
        nombreBinaire.emplace_back(i); // Traduire et Stocker les nombres binaires dans un vecteur
    }
    std::bitset<16> addressMask = createMask(0,7); //Crééer un masque pour lire juste les bits 0 à 7 (offset)
    std::bitset<16> OffsetMask = createMask(8,15); //Créer un masque pour lire juste les bits 8 à 15 (page)

    //Boucler sur les x adresses
    for(auto add : nombreBinaire)
    {
        auto address = addressMask & add; //applique un masque qui dévoile la page
        auto offset = OffsetMask & add; //applique un masque qui dévoile l'offset'
        virt temp{(int)(address.to_ulong()),(int)(offset.to_ulong())};
        virtuelle.emplace_back(temp);
    }
    //Table de pages
    //Une adresse à la fois, vérifier si elle est dans la table de page

    for(virt v : virtuelle)
    {
        if(tablePage[v.bits_page][1] != 1)
        {
            std::cout << "Page non-chargée dans la table" << std::endl;
            std::fstream myfile;
            myfile.open("../simuleDisque.bin");
            if (myfile.is_open()){
                printf("addresseVirtuelle: %i addresse physique: %i",  v.bits_page,v.bits_offset);
                myfile.seekg(v.bits_page * sizeof(tablePage[v.bits_page]), std::ios::beg); //Trouver l'endroit correspondant au byte signé dans le fichier
                myfile.read(tablePage[v.bits_page],256); //Lire cet emplacement
            }
            else
                printf("Erreur a la lecture du fichier, le fichier n'existe pas.");

            printf( "%s", tablePage[v.bits_page]);
            //Fermer le fichier
            myfile.close();

        }
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


