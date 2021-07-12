#include "Comparator.cpp"
#include "Comparator.h"
#include <math.h>
#include <tfhe/tfhe_gate_bootstrapping_functions.h>

float g_time;

int main() {

    // const int minimum_lambda = 80;
    int minimum_lambda = getsecparam();
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);

    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);
    const TFheGateBootstrappingCloudKeySet* bk = &key->cloud;

    int32_t length = 0;
    length = getlength();

    int64_t plain1 = 0;
    int64_t plain2 = 0;

    cout << "Give us two integers! \n"; 
    plain1 = getNumber();
    encode_to(plain1, length);
    plain2 = getNumber();
    encode_to(plain2, length);


    LweSample* cipher1 = new_gate_bootstrapping_ciphertext_array(length, params);
    LweSample* cipher2 = new_gate_bootstrapping_ciphertext_array(length, params);

    for (int i = 0; i < length; i++){
        bootsSymEncrypt(&cipher1[i], (plain1>>i)&1, key);
        bootsSymEncrypt(&cipher2[i], (plain2>>i)&1, key);
    }

    // cout << "\n                  plaintext 1  :  " << plain1 << "\n";
    // cout << "                  plaintext 2  :  " << plain2 << "\n\n";
    boot_eval(minimum_lambda, length, bk, key);
    comp_ft(plain1, plain2, cipher1, cipher2, minimum_lambda, length, bk, key);
    // boot_eval(minimum_lambda, length, bk, key);
    // comp_add(plain1, plain2, cipher1, cipher2, length, g_time, bk, key);

}