#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

void encrypt(std::string fileName){
    std::fstream fs(fileName, std::ios::in | std::ios::out | std::ios::binary);
    if(!fs.is_open()){
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Use cereal to serialize the data in the file
    cereal::BinaryInputArchive input(fs);
    std::string data;
    input(data);

    // Encrypt the data
    for(char &c : data) c += 100;

    // Use cereal to serialize the encrypted data back to the file
    fs.seekp(0);
    cereal::BinaryOutputArchive output(fs);
    output(data);

    fs.close();
}

void decrypt(std::string fileName){
    std::fstream fs(fileName, std::ios::in | std::ios::out | std::ios::binary);
    if(!fs.is_open()){
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Use cereal to serialize the data in the file
    cereal::BinaryInputArchive input(fs);
    std::string data;
    input(data);

    // Decrypt the data
    for(char &c : data) c -= 100;

    // Use cereal to serialize the decrypted data back to the file
    fs.seekp(0);
    cereal::BinaryOutputArchive output(fs);
    output(data);

    fs.close();
}