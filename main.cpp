#include "DES.h"
#include <string>
#include <iostream>
#include <bitset>
#include <list> 
using namespace std;
#define ENCRY true
#define DECRY false
int main(){
    DES des;

    string s = "ILoveYou", k = "IFuckYou";

    // string s, k;
    // list<string> s_list;
    // while(s.size() != 8){
    //     cout << "Please Enter Your Words(length = 8): ";
    //     cin >> s;
    // }
    
    // while(k.size() != 8){
    //     cout << "Please Enter Your Key  (length = 8): ";
    //     cin >> k;
    // }
    
    des.SETKEY(k);
    bitset<64> in, encry, decry;
    in = des.ByteToBit<64>(s);
    encry = des.des_run<64>(in, ENCRY);

    decry = des.des_run<64>(encry, DECRY);

    cout << "Origin Word: " << s << endl;
    cout << "Keys       : " << k << endl;
    cout << "Encrypted  : " << encry << endl;
    cout << "Decrypted  : " << des.BitToByte<64>(decry) << endl;

    return 0;
}