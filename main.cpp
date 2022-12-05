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

std::vector<int>addresseLog;
std::vector<int>addressePhysique; //Adresses Logiques
std::vector<std::bitset<16>>nombreBinaire; //Adresses Logiques

struct virt {
    int address;
    int bits_page;
    int bits_offset;
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
void display(std::vector<virt> v){
    while(!v.empty()){
        printf("   page:   %d offset:  %d \n", v.back().bits_page, v.back().bits_offset);
        v.pop_back();
    }
}
void debugDisplayDisk(){
    std::fstream myfile;
    unsigned int r, cpt =0;
    myfile.open("../simuleDisque.bin");
    if (myfile.is_open()){
        while(!myfile.eof()){
            printf( "\n page %u ", cpt);
            for(int i =0; i<256; i++){
                myfile.read(reinterpret_cast<char *>(&r), 1); //Lire cet emplacement
                std::bitset<16> a = r;
                std::string aa = a.to_string();
                printf( "%s     ", aa.c_str());
            }
            cpt++;
        }
    }
    else
        printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
    myfile.close();
}

void debugDisplayDiskValues(){
    std::fstream myfile;
    unsigned int r, cpt =0;
    bool exit = false;
    myfile.open("../simuleDisque.bin");
    if (myfile.is_open()){
        while(!myfile.eof() && !exit){
            printf( "\n page %u ", cpt);
            for(int i =0; i<256; i++){
                myfile.read(reinterpret_cast<char *>(&r), 1); //Lire cet emplacement
                std::bitset<16> a = r;
                std::bitset<8> d(a.to_string(), 8);
                printf( "%i     ", (int)(d.to_ulong()));
            }
            cpt++;
        }
    }
    else
        printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
    myfile.close();
}
void debugseekAddress(int page, int offset){
    std::fstream myfile;
    unsigned int r, cpt =0;
    myfile.open("../simuleDisque.bin");
    if (myfile.is_open()){
                myfile.seekg(page * 256 + offset, std::ios::beg); //Trouver l'endroit correspondant au byte signé dans le fichier
                myfile.read(reinterpret_cast<char *>(&r), 1); //Lire cet emplacement
                std::bitset<16> a = r;
                std::string aa = a.to_string();
                printf( "%i     ", r);
            }
    else    printf("Erreur a la lecture du fichier, le fichier n'existe pas.");

    myfile.close();

}
int main()
{
    //Initialisation et déclarations
    int memPhysique[256] = {0}; //Mémoire physique
    char tablePage[256]={0}; //Table de page
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
    for(virt v : virtuelle)
    {
        if(tablePage[v.bits_page] != 1)
        {
            //std::cout << "Page non-chargée dans la table" << std::endl;
            std::fstream myfile;
            myfile.open("../simuleDisque.bin");
            if (myfile.is_open()){
                myfile.seekg(v.bits_page * 256 + v.bits_offset, std::ios::beg); //Trouver l'endroit correspondant au byte signé dans le fichier
                myfile.read(reinterpret_cast<char *>(&tablePage[v.bits_page]),sizeof(short)); //Lire cet emplacement
                printf("addresseVirtuelle: %i offset: %i valeur:  %i bits_page: %i",  v.address ,v.bits_offset, tablePage[v.bits_page], v.bits_page);
            }
            else
                printf("Erreur a la lecture du fichier, le fichier n'existe pas.");
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


