#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <conio.h>

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

class custom_moneypunct : public std::moneypunct<char>
{
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

class pemilik{
public:
    static pemilik& get_instance(){
        static pemilik instance;
        return instance;
    }

    pemilik(const pemilik&) = delete;
    pemilik(pemilik&&) = delete;
    pemilik& operator = (const pemilik&) = delete;
    pemilik& operator = (pemilik&&) = delete;

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

protected:
    pemilik() = default;

private:
    string username = "admin";
    string password = "admin";
};

class mobil{
public:
    // static mobil& get_instance(){
    //     static mobil instance;
    //     return instance;
    // }

    // mobil(const mobil&) = delete;
    // mobil(mobil&&) = delete;
    // mobil& operator = (const mobil&) = delete;
    // mobil& operator = (mobil&&) = delete;

    void menu(){
        cout << "======MENU======\n";
        cout << "1.input mobil\n";
        cout << "2.hapus mobil\n";
        cout << "3.tampilkan mobil\n";
        cout << "4.tampilkan pendapatan\n";
        cout << "5.keluar\n";
        cout << "================\n";
        cout << "pilih menu : ";
    }
    void menuMobil(){
        cout << "masukkan ";
    }
    void tambahMobil(){
        cout << "masukkan merek mobil : ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        //cin.getline(merek, 100);
        getline(cin, merek);
        cout << "masukkan model mobil : ";
        //cin.getline(model, 100);
        getline(cin, model);
        cout << "masukkan plat nomor : ";
        cin >> platNomor;
        //cin.getline(platNomor, 100);
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

// protected:
//     mobil() = default;

private:
    string merek;
    string model;
    string platNomor;
    int hargaSewa;
};

mobil car;

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

void loginCheck(){
    string username, password;
    
    system("CLS");
    cout << "masukkan username : ";
    cin >> username;

    cout << "masukkan password : ";
    password = hiddenChar();

    if((pemilik::get_instance().getUsername() == username) && 
    (pemilik::get_instance().getPassword() == password)){

        return;

    }else{
        system("CLS");
        cout << "password atau username salah!!";

        pause();
        exit(1);
    } 
}

int main(){
    int pilihan;

    locale loc(locale::classic(), new custom_moneypunct());
    cout.imbue(loc);

    loginCheck();

    while(1){
        system("CLS");
        car.menu();
        cin >> pilihan;

        switch(pilihan){
            case 1 : saveMobil(); break;
            case 2 : hapusMobil(); break;
            case 3 : displayMobil(); break;
            case 4 : break;
            case 5 : exit(1);
        }
    }
    return 0;
}