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
    void tampilkanCart(){
        cout << "=================================\n";
        cout << "merek\t\t: " << merek << "\n";
        cout << "model\t\t: " << model << "\n";
        cout << "plat nomor\t: " << platNomor << "\n";
        cout << "harga sewa\t: " << std::showbase << std::put_money(hargaSewa) << "\n";
        cout << "jumlah hari sewa: " << hariSewa << "\n";
        cout << "harga sewa total: " << std::showbase << std::put_money(hargaSewa*hariSewa) << "\n";
    }
    string getPlat(){
        return platNomor;
    }
    void setHariSewa(int hariSewa){
        mobil::hariSewa = hariSewa;
    }
    int getHariSewa(){
        return hariSewa;
    }
    int getHargaSewa(){
        return hargaSewa;
    }
private:
    friend class cereal::access;

    template <typename Archive>
    void serialize(Archive &ar) {
        ar(merek, model, platNomor, hargaSewa, hariSewa);
    }
    
    string merek;
    string model;
    string platNomor;
    int hargaSewa;
    int hariSewa;
};

mobil car;

bool cariMobil(string plat){

        fs.open("mobil.dat", std::ios::in | std::ios::out | std::ios::binary);
        fs.seekg(0, ios::beg);

        cereal::BinaryInputArchive input(fs);

        while(true){
            try{
                input(car);
            }
            catch (cereal::Exception &e){
                fs.close();
                return false;
                // break;
            }

            if(car.getPlat() == plat){
                fs.close();
                return true;
            }
        }   
}

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

    void pemasukan(int pemasukan){
        pendapatan += pemasukan;
    }

    void resetPendapatanP(){
        pendapatan = 0;
    }

    void menu(){
        cout << "==========MENU==========\n";
        cout << "1. input mobil\n";
        cout << "2. hapus mobil\n";
        cout << "3. tampilkan mobil\n";
        cout << "4. tampilkan pendapatan\n";
        cout << "5. ubah username/password\n";
        cout << "6. reset pendapatan\n";
        cout << "7. keluar\n";
        cout << "========================\n";
        cout << "pilih menu : ";
    }

    void saveMobil(){
        char ch;
        std::ofstream fs("mobil.dat", std::ios::app | std::ios::binary);

        if(!fs.is_open()){
            cout << "tidak dapat membuka file!";
            pause();
            return;

        }else{
            do{
                system("CLS");
                car.tambahMobil();

                cereal::BinaryOutputArchive oar(fs);
                oar(car);

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

        fs.open("mobil.dat", std::ios::in | std::ios::out | std::ios::binary);

        std::fstream fs2;
        fs2.open("temp.dat", std::ios::out | std::ios::binary);

        fs.seekg(0, ios::beg);

        while(true){
            try{
                cereal::BinaryInputArchive input(fs);
                input(car);
            }
            catch (cereal::Exception &e){
                break;
            }

            if(car.getPlat() != nomorMobil){
                cereal::BinaryOutputArchive output_archive(fs2);
                output_archive(car);
            }
        }
            
        fs2.close();
        fs.close();

        remove("mobil.dat");
        rename("temp.dat", "mobil.dat");

        cout << "mobil dihapus";
        pause();
    }

    void ttlPendapatan(){
        system("CLS");
        cout << "pendapatan owner adalah : " << std::showbase << std::put_money(pendapatan) << "\n";
        pause();
    }

private:
    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(username, password, pendapatan);
    }

    string username;
    string password;
    int pendapatan;
};

pemilik owner;

void changeCredential(){
        string temp;
        system("CLS");
        decrypt("credential.dat");

        fs.open("credential.dat", std::ios::out | std::ios::binary);
        cout << "masukkan username : ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        getline(std::cin, temp);
        owner.setUsername(temp);

        cout << "masukkan password : ";
        temp = hiddenChar();
        owner.setPassword(temp);

        {
            cereal::BinaryOutputArchive oar(fs);
            oar(owner);
        }
        fs.close();

        owner.setUsername("");
        owner.setPassword("");

        fs.open("credential.dat", std::ios::in | std::ios::binary);
        {
            cereal::BinaryInputArchive iar(fs);
            iar(owner);
        }
        fs.close();

        std::cout << "\nusername adalah = " << owner.getUsername();
        std::cout << "\npassword adalah = " << owner.getPassword();
        
        encrypt("credential.dat");   
}

void resetPendapatan(){
    decrypt("credential.dat");

    std::ifstream fs2("credential.dat", ios::in | std::ios::binary);
    cereal::BinaryInputArchive input(fs2);
            
    input(owner);
    owner.resetPendapatanP();
    fs2.close();

    fs.open("credential.dat", std::ios::out | std::ios::binary);

    cereal::BinaryOutputArchive oar(fs);
    oar(owner);

    fs.close();
    encrypt("credential.dat");
}

class pengguna{
public:
    void menu(){
        cout << "=============MENU=============\n";
        cout << "1. pilih mobil untuk dirental\n";
        cout << "2. hapus mobil yang dirental\n";
        cout << "3. list mobil yang tersedia\n";
        cout << "4. list mobil yang dipilih\n";
        cout << "5. check out\n";
        cout << "6. keluar\n";
        cout << "==============================\n";
        cout << "Pilih menu : ";
    }

    void addMobil(){
        char ch = 'Y';
        string plat;
        int temp;

        std::ofstream fs("cart.dat", std::ios::app | std::ios::binary);
        std::ifstream fs2("mobil.dat", std::ios::in | std::ios::binary);

        do{
            system("CLS");
            cout << "masukkan plat mobil yang ingin ditambahkan!\n";
            cout << "plat : ";
            cin >> plat;

            try{
                cereal::BinaryInputArchive oarc(fs2);
                oarc(car);

                if(cariMobil(plat)){
                    car.tampilkanMobil();
                    cout << "\nmasukkan hari penyewaan : ";
                    cin >> temp;

                    car.setHariSewa(temp);

                    cereal::BinaryOutputArchive oar(fs);
                    oar(car);
                    
                    cout << "\nmobil berhasil ditambahkan ke keranjang!";
                    cout << "\ntambahkan mobil lainnya?..(y/n?)";
                    cin >> ch;
                }
            }
            catch (cereal::Exception &e){
                cout << "\nplat tidak ditemukan!";
                Sleep(2000);
                break;
            }

            }while(ch=='y'||ch=='Y');
            fs.close();
    }

    void delMobil(){
        string nomorMobil;
        system("CLS");

        cout << "masukkan plat mobil yang ingin dihapus : ";
        cin >> nomorMobil;

        fs.open("cart.dat", std::ios::in | std::ios::out | std::ios::binary);

        std::fstream fs2;
        fs2.open("temp.dat", std::ios::out | std::ios::binary);

        fs.seekg(0, ios::beg);

        while(true){
            try{
                cereal::BinaryInputArchive input(fs);
                input(car);
            }
            catch (cereal::Exception &e){
                break;
            }

            if(car.getPlat() != nomorMobil){
                cereal::BinaryOutputArchive output_archive(fs2);
                output_archive(car);
            }
        }
            
        fs2.close();
        fs.close();

        remove("cart.dat");
        rename("temp.dat", "cart.dat");

        cout << "mobil dihapus";
        pause();
    
    }

    void tampilCart(){
        system("CLS");
        std::ifstream fs("cart.dat", std::ios::binary);
        cereal::BinaryInputArchive input(fs);

        if(!fs.is_open()){
            cout << "tidak dapat membuka file!";
            pause();
            return;

        }else{
            while(true)
            {
                try
                {
                    input(car);
                    car.tampilkanCart();
                    total += (car.getHariSewa()*car.getHargaSewa());
                }
                catch (cereal::Exception &e)
                {
                    break;
                }
            }
            fs.close();
        }
    }

    void checkout(){
        int pilihan;
        total = 0;

        tampilCart();
        cout << "\n\nharga yang harus dibayar adalah : "<< std::showbase << std::put_money(total) << "\n";
        cout << "\n\n1.bayar";
        cout << "\n2.kembali";
        cout << "\n\npilihan : ";
        cin >> pilihan;
        if(pilihan == 1){
            remove("cart.dat");
            decrypt("credential.dat");

            std::ifstream fs2("credential.dat", ios::in | std::ios::binary);
            cereal::BinaryInputArchive input(fs2);
            
            input(owner);
            owner.pemasukan(total);
            fs2.close();

            fs.open("credential.dat", std::ios::out | std::ios::binary);

            cereal::BinaryOutputArchive oar(fs);
            oar(owner);
            fs.close();

            cout << "\npembayaran berhasil!";
            encrypt("credential.dat");   
            pause();
        }
    }
private:
    int total;
};

pengguna user;

void displayMobil(){
    system("CLS");
    std::ifstream fs("mobil.dat", std::ios::binary);
    cereal::BinaryInputArchive input(fs);

    if(!fs.is_open()){
        cout << "tidak dapat membuka file!";
        pause();
        return;

    }else{
        while(true)
        {
            try
            {
                input(car);
                car.tampilkanMobil();
            }
            catch (cereal::Exception &e)
            {
                break;
            }
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
    
    encrypt("credential.dat");

    if((owner.getUsername() == username) && 
    (owner.getPassword() == password)){
        owner.setUsername("");
        owner.setPassword("");
        return;

    }else{
        system("CLS");
        cout << "password atau username salah!!";
        owner.setUsername("");
        owner.setPassword("");

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
            case 4 : owner.ttlPendapatan(); break;
            case 5 : changeCredential(); break;
            case 6 : resetPendapatan(); break;
            case 7 : exit(1);
            default : cout << "\npilihan salah !!"; Sleep(2000);
        }
    }
}

void runtimePengguna(){
    int pilihan;
    while(1){
        system("CLS");
        user.menu();
        cin >> pilihan;
        
        switch(pilihan){
            case 1 : user.addMobil(); break;
            case 2 : user.delMobil(); break;
            case 3 : displayMobil(); break;
            case 4 : user.tampilCart(); pause(); break;
            case 5 : user.checkout(); break;
            case 6 : exit(1);
            default : cout << "\npilihan salah !!"; Sleep(2000);
        }
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

    // encrypt("credential.dat");

    return 0;
}