#define NOMINMAX

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include "encryption.h"

using std::cout;
using std::cin;
using std::vector;
using std::string;
using std::ios;
using std::locale;

void pause(){
    cout << "\n\nTekan enter untuk melanjutkan";
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}

enum IN {
    IN_BACK = 8,
    IN_RET = 13
};

string hiddenChar(char sp = '*'){
    string password = "";
    char ch_ipt;

    while (true) {
        ch_ipt = getch();

        if (ch_ipt == IN::IN_RET){
            cout << "\n";
            return password;
        }
        else if (ch_ipt == IN::IN_BACK && password.length() != 0){
            password.pop_back();
            cout << "\b \b";
  
            continue;
        }
        else if (ch_ipt == IN::IN_BACK && password.length() == 0){
            continue;
        }

        password.push_back(ch_ipt);
        cout << sp;
    }
}

class custom_moneypunct : public std::moneypunct<char>{
    virtual char do_thousands_sep() const {
        return '.';
    }
    
    virtual moneypunct::string_type do_grouping() const {
        return "\003";
    }
    
    virtual moneypunct::string_type do_negative_sign() const {
        return "-";
    }
    
    virtual moneypunct::string_type do_curr_symbol() const {
        if(moneypunct::intl == false){
            return "Rp";
        }else{
            return "IDR";
        }
    }
    
    virtual pattern do_pos_format() const {
        return {{symbol, space, sign, value}};
    }
    
    virtual pattern do_neg_format() const {
        return {{symbol, space, sign, value}};
    }
};

std::fstream fs;

class mobil{
public:
    void tambahMobil(){
        cout << "masukkan merek mobil : ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        getline(cin, merek);

        cout << "masukkan model mobil : ";
        getline(cin, model);

        cout << "masukkan plat nomor : ";
        cin >> platNomor;
        
        cout << "masukkan harga sewa : ";
        cin >> hargaSewa;
    }
    void tampilkanMobil(){
        cout << "=================================\n";
        cout << "merek\t\t: " << merek << "\n";
        cout << "model\t\t: " << model << "\n";
        cout << "plat nomor\t: " << platNomor << "\n";
        cout << "hargaSewa\t: " << std::showbase << std::put_money(hargaSewa) << "\n";
    }
    string getPlat(){
        return platNomor;
    }

private:
    string merek;
    string model;
    string platNomor;
    int hargaSewa;
};

mobil car;

class pemilik{
public:
    void setUsername(string username){
        pemilik::username = username;
    }

    void setPassword(string password){
        pemilik::password = password;
    }

    string getUsername(){
        return username;
    }

    string getPassword(){
        return password;
    }

    void menu(){
        cout << "======MENU======\n";
        cout << "1. input mobil\n";
        cout << "2. hapus mobil\n";
        cout << "3. tampilkan mobil\n";
        cout << "4. tampilkan pendapatan\n";
        cout << "5. ubah username\n";
        cout << "6. ubah password\n";
        cout << "7. keluar\n";
        cout << "================\n";
        cout << "pilih menu : ";
    }

    void saveMobil(){
        char ch;
        fs.open("mobil.dat", ios::app | ios::binary);

        if(!fs.is_open()){
            cout << "tidak dapat membuka file!";
            pause();
            return;

        }else{
            do{
                system("CLS");
                car.tambahMobil();

                fs.write((char*)&car, sizeof(mobil));
                cout << "tambahkan mobil lainnya?..(y/n?)";
                cin >> ch;

            }while(ch=='y'||ch=='Y');
            fs.close();
        }
        
    }

    void hapusMobil(){
        string nomorMobil;
        system("CLS");

        cout << "masukkan plat mobil yang ingin dihapus : ";
        cin >> nomorMobil;

        fs.open("mobil.dat", ios::in | ios::out);

        std::fstream fs2;
        fs2.open("temp.dat", ios::out);

        fs.seekg(0, ios::beg);

        while(fs.read((char*)&car, sizeof(mobil)))
        {
            if(car.getPlat() != nomorMobil){
                fs2.write((char*)&car, sizeof(mobil));
            }
        }
            
        fs2.close();
        fs.close();

        remove("mobil.dat");
        rename("temp.dat", "mobil.dat");

        cout << "mobil dihapus";
        pause();
    }

private:
    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(username, password);
    }

    string username;
    string password;
};

pemilik owner;

void displayMobil(){
    system("CLS");
    fs.open("mobil.dat", ios::in | ios::binary);

    if(!fs.is_open()){
        cout << "tidak dapat membuka file!";
        pause();
        return;

    }else{
        while(fs.read((char*)&car, sizeof(mobil)))
        {
            car.tampilkanMobil();
        }

        fs.close();
        pause();
    }
}

void loginCheck(){
    string username, password;
    
    system("CLS");
    cout << "masukkan username : ";
    cin >> username;

    cout << "masukkan password : ";
    password = hiddenChar();

    decrypt("credential.dat");

    std::ifstream fs2("credential.dat", ios::in | std::ios::binary);
    cereal::BinaryInputArchive input(fs2);
    input(owner);
    
    fs2.close();
    // cout << "\n\nits here!";
    encrypt("credential.dat");

    if((owner.getUsername() == username) && 
    (owner.getPassword() == password)){
        
        return;

    }else{
        system("CLS");
        cout << "password atau username salah!!";
        cout << "\nuser : "<< owner.getUsername();
        cout << "\npass : "<< owner.getPassword();

        pause();
        exit(1);
    } 
}

void runtimePemilik(){
    int pilihan;

    loginCheck();

    while(1){
        system("CLS");
        owner.menu();
        cin >> pilihan;

        switch(pilihan){
            case 1 : owner.saveMobil(); break;
            case 2 : owner.hapusMobil(); break;
            case 3 : displayMobil(); break;
            case 4 : break;
            case 5 : exit(1);
            case 7 : exit(1);
            default : cout << "\npilihan salah !!"; Sleep(2000);
        }
    }
}


void runtimePengguna(){
    while(1){
        exit(1);
    }
}

int main(){
    locale loc(locale::classic(), new custom_moneypunct());
    cout.imbue(loc);

    int login;
    string temp;

    system("CLS");

    cout << "=====program rental mobil=====";
    cout << "\n1. owner";
    cout << "\n2. user";
    cout << "\nlogin sebagai ? : ";
    cin >> login;
    
    if(login == 1){
        runtimePemilik();
    }else if(login == 2){
        runtimePengguna();
    }else{
        system("CLS");
        cout << "\npilihan salah !!";
        cout << "\nprogram keluar";
        Sleep(2000);
        exit(1);
    }
    



    // fs.open("credential.dat", std::ios::out | std::ios::binary);
    // std::cout << "masukkan username : ";
    // std::getline(std::cin, temp);
    // owner.setUsername(temp);

    // std::cout << "masukkan password : ";
    // std::getline(std::cin, temp);
    // owner.setPassword(temp);

    // {
    //     cereal::BinaryOutputArchive oar(fs);
    //     oar(owner);
    // }
    // fs.close();

    // encrypt("credential.dat");
    // decrypt("credential.dat");

    // owner.setUsername("");
    // owner.setPassword("");

    // fs.open("credential.dat", std::ios::in | std::ios::binary);
    // {
    //     cereal::BinaryInputArchive iar(fs);
    //     iar(owner);
    // }
    // fs.close();

    // std::cout << "\nusername adalah = " << owner.getUsername();
    // std::cout << "\npassword adalah = " << owner.getPassword();

    return 0;
}