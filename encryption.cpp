#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

void encrypt(std::string fileName, std::string temp){
    char ch;
    std::fstream fps, fpt;
    
    fps.open(fileName, std::fstream::in);
    if(!fps){
        std::cout << "\nError : tidak dapat membuka file " << fileName << " !!";
        Sleep(2000);
        exit(1);
    }

    fpt.open(temp, std::fstream::out);
    if(!fpt){
        std::cout << "\nError : tidak dapat membuka file tmp " << temp << " !!";
        Sleep(2000);
        exit(1);
    }

    while(fps >> std::noskipws >> ch){
        ch = ch + 100;
        fpt << ch;
    }

    fps.close();
    fpt.close();

    fps.open(fileName, std::fstream::out);
    if(!fps){
        std::cout << "\nError : tidak dapat membuka file " << fileName << " !!";
        Sleep(2000);
        exit(1);
    }

    fpt.open(temp, std::fstream::in);
    if(!fpt){
        std::cout << "\nError : tidak dapat membuka file tmp " << temp << " !!";
        Sleep(2000);
        exit(1);
    }

    while(fpt >> std::noskipws >> ch){
        fps << ch;
    }
    fps.close();
    fpt.close();
}

void decrypt(std::string fileName, std::string temp){
    char ch;
    std::fstream fps, fpt;
    
    fps.open(fileName, std::fstream::out);
    if(!fps){
        std::cout << "\nError : tidak dapat membuka file " << fileName << " !!";
        Sleep(2000);
        exit(1);
    }

    fpt.open(temp, std::fstream::in);
    if(!fpt){
        std::cout << "\nError : tidak dapat membuka file tmp " << temp << " !!";
        Sleep(2000);
        exit(1);
    }
    
    while(fpt >> std::noskipws >> ch){
        ch = ch - 100;
        fps << ch;
    }

    fps.close();
    fpt.close();
}