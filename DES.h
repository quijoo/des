#ifndef DES_H
#define DES_H
#include <bitset>
#include "STATIC.h"
#include <iostream>
using namespace std;
class DES{
    private:
        bitset<64> key;
        bitset<48> subkey[16];

    public:

        void SETKEY(const string Key){
            // 是默认最后 8 位是校验位？
            // Check
            this->key = this->ByteToBit<64>(Key);
            string tmp = this->TRANSFORM<64, 56>(this->key, PC_1).to_string();

            // check
            string left, right;
            for (int i = 0; i < 16; i++){
                left = tmp.substr(0, 28);
                right = tmp.substr(28, 28);

                left = this->left_move(left, move_times[i]);
                right = this->left_move(right, move_times[i]);

                tmp = left + right;
                this->subkey[i] = this->TRANSFORM<56, 48>(bitset<56>(tmp), PC_2);
            }
        }

        string left_move(string In, int loop){
            bitset<28> tmp(In);
            return (tmp << loop | tmp >> (tmp.size() - loop)).to_string();
        }

        template <int in_l, int out_l>
        bitset<out_l> F_FUNCTION(bitset<in_l> right, bitset<48> ki){
            bitset<48> o1 = this->TRANSFORM<32 ,48>(right, E_table) ^ ki;
            bitset<32> o2 = this->S_BOXF<48, 32>(o1, S_Box);
            o2 = this->TRANSFORM<32 ,32>(o2, P_table);
            return o2;
        }

        template<int in_l, int out_l>
        bitset<out_l> S_BOXF(bitset<in_l> In, const int sbox[][4][16]){
            
            string tmp_s = "";
            for (size_t i = 0, j, k; i < In.size()/8; i++){
                j = (In[0] << 1) + In[5];
                k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
                bitset<4> tmp_b(sbox[i][j][k]);
                tmp_s += tmp_b.to_string();
            }
            bitset<out_l> res(tmp_s);
            return res;
        }

        template<int in_l, int out_l>
        bitset<out_l> TRANSFORM(bitset<in_l> In, const int *Table){// 变换函数
            bitset<out_l> Out;
            for (int i = 0; i < out_l; i++){
                Out[out_l - 1 - i] = In[Table[i] - 1];
            }
            return Out;
        }

        template<int len>
        bitset<len> ByteToBit(string In){
            bitset<len> Out;
            for (size_t i = 0; i < 8*In.size(); i++)
                Out[len - 1 - i] = ((int)In[i / 8] >> (7 - i % 8)) & 1;
            return Out;
        }
        
        template<int len>
        string BitToByte(bitset<len> In){
            int Out[8] = {0};
            string res = "";
            for (size_t i = 0; i < In.size(); i++)
                Out[i/8] |= (In[len - 1 - i] << (7 - i % 8));
            for (int i = 0; i < 8; i++){
                res += (char)(Out[i]);
            }
            return res;
        }

        template <int l>
        bitset<l> des_run(bitset<l> inp, bool encrypt){

            inp = TRANSFORM<64, 64>(inp, IP_Table);
            bitset<32> left(inp.to_string().substr(0, 32));
            bitset<32> right(inp.to_string().substr(32, 32));

            if (encrypt){
                cout << "Encrptying..." <<endl;
                for (int i = 0; i < 16; i++){

                    bitset<32> tmp = right;
                    right = this->F_FUNCTION<32, 32>(right, this->subkey[i]) ^ left;
                    left = tmp;
                }
            }
            else{
                cout << "Decrptying..." <<endl;
                for (int i = 15; i >= 0; i--){
                    
                    bitset<32> tmp = left;
                    left = this->F_FUNCTION<32, 32>(left, this->subkey[i]) ^ right;
                    right = tmp;
                }
            }
            
            
            bitset<64> res(left.to_string() + right.to_string());
            res = this->TRANSFORM<64, 64>(res, IPR_Table);
            return res;
        }
};
#endif