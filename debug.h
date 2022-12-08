//
// Created by oli_d on 12/5/2022.
//
#include <vector>
#include <fstream>
#include <bitset>

#pragmaonce

#ifndef MAIN_CPP_DEBUG_H
#define MAIN_CPP_DEBUG_H

class virt;

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
#endif //MAIN_CPP_DEBUG_H
