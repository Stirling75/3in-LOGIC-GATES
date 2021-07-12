#include "add.h"
#include "add.cpp"
#include "Comparator.h"
#include <tfhe/tfhe_gate_bootstrapping_functions.h>
#include <cstdint>


#define CLOCKS_PER_MS 1000
extern float g_time;

int64_t encode_to (int64_t res, const int length) {
    switch(length) {
        case 8:
            res = (int8_t) res;
            break;

        case 16:
            res = (int16_t) res;
            break;

        case 32:
            res = (int32_t) res;
            break;

        case 64:
            res = (int64_t) res;
            break;
        
        default:
            cout << "Nothing happend! \n";
            break;
            
    }
return res;
}

int32_t getlength() {

    int32_t len;

    cout << "Enter bit representation length n (8/16/32/64): ";
    cin >> len;

    if (cin.fail()){
        cout << "Wrong Number. Retry! \n"; 
        cin.clear(); 
        cin.ignore(32, '\n'); 
        return getlength(); 
    }
    if (!((len == 8) || (len == 16) || (len == 32) || (len == 64))){
        cout << "Sorry! We provide only n = 8, 16, 32, 64! \n";
        return getlength();
    }
    return len;
}

int32_t getNumber() {

    int32_t retNum;

    cout << "Enter Number: ";
    cin >> retNum;

    if (cin.fail()){
        cout << "Wrong Number. Retry! \n"; 
        cin.clear(); 
        cin.ignore(32, '\n'); 
        return getNumber(); 
    }
    if (retNum < INT32_MIN && retNum > INT32_MAX ){
        cout << "Wrong Number. We recommend your number should be inside " << INT32_MIN << "<= x <= " << INT32_MAX << "\n";
        cin.clear();
        cin.ignore(32, '\n');
        return getNumber();
    }
    return retNum;
}

int32_t getsecparam() {

    int32_t secparam;

    cout << "Enter Security Parameter! (80, 127) : ";
    cin >> secparam;

    if (cin.fail()){
        cout << "Wrong Number. Retry! \n"; 
        cin.clear(); 
        cin.ignore(32, '\n'); 
        return getsecparam(); 
    }
    if (secparam > 127) {
        cout << "TFHE only supports parameters under 127 bit of security! \n";
        return getsecparam();
    }
    else if (secparam > 80 and secparam <= 127) {
        cout << "Security level : 127 bit! \n";
        secparam = 127;
    }
    else if (secparam > 0 and secparam <= 80) {
        cout << "Security level : 80 bit! \n";
        secparam = 80;
    }
    else  {
        cout << "Wrong security parameter! Try again :) \n";
        return getsecparam();
    }

    return secparam;
}

void get_time (float time) {
    g_time = time;
} 

void comp_ft(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, int sec_param, int32_t length, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    int sel;
    cout.precision(6);
    
    while(1) {
        cout << "-------------------------------------------------------------------" << "\n";
        cout << "|           0. Gate Bootstrapping Parameters & Runtime            |" << "\n";
        cout << "|                           1. Addition                           |" << "\n";
        cout << "|                          2. Subtraction                         |" << "\n";
        cout << "|                         3. Compare Small                        |" << "\n";
        cout << "|                         4. Compare Large                        |" << "\n";
        cout << "|                     5. Compare Small or Equal                   |" << "\n";
        cout << "|                     6. Compare Large or Equal                   |" << "\n";
        cout << "|                            7. Maximum                           |" << "\n";
        cout << "|                            8. Minimum                           |" << "\n";
        cout << "|                         9. Absolute Value                       |" << "\n";
        cout << "|                        10. Multiplication                       |" << "\n";
        cout << "|                           11. Division                          |" << "\n";
        cout << "|                            -1. Exit                             |" << "\n";
        cout << "-------------------------------------------------------------------" << "\n\n";

        cout << " Please Select the operation number you want to proceed (0 ~ 11, -1) : ";
        cin >> sel;
        if (cin.fail()){
            cout << "Wrong Number. Retry! \n"; 
            cin.clear(); 
            cin.ignore(32, '\n'); 
        }
        
        switch(sel) {
            case 0:
                boot_eval(sec_param, length, bk, key);
                break;
            
            case 1:
                comp_add(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 2:
                comp_subt(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 3:
                comp_CompS(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 4:
                comp_CompB(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;

            case 5:
                comp_CompSE(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 6:
                comp_CompLE(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 7:
                comp_MAX(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 8:
                comp_Min(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 9:
                comp_Abs(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 10:
                comp_Mult(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case 11:
                comp_Div(plain_a, plain_b, a, b, length, g_time, bk, key);
                break;
            
            case -1:
                exit(1);
                
            default:
                cout << "\n Please select a number inside 0 ~ 11, -1 ! \n";
                break;
        }
    }
}

void boot_eval(int sec_param, int32_t length, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key){
    LweSample* tmp1 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* tmp2 = new_gate_bootstrapping_ciphertext(bk->params);
    bootsSymEncrypt(tmp1, 0, key);
    bootsSymEncrypt(tmp2, 1, key);
    int mes = 30;
    // LweSample* tmp2 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* res = new_gate_bootstrapping_ciphertext(bk->params);
    Torus32 mu = modSwitchToTorus32(1, 8);

    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|           0. Gate Bootstrapping Parameters & Runtime            |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n\n";

    cout << "            Security Parameter :  " << sec_param << "-bits of Security                " << "\n";
    cout << "                   Ciphertext Length :  " << length << "             " << "\n";
    cout << "\n";
    cout << "                         --LWE PARAMS--                                              " << "\n";
    cout << "                             n :  " << bk->params->in_out_params->n << "              " << "\n";
    cout << "                       LWE std :  " << bk->params->in_out_params->alpha_min << "      " << "\n";
    cout << "\n";
    cout << "                        --TLWE PARAMS--                                              " << "\n";
    cout << "                             N :  " << bk->params->tgsw_params->tlwe_params->N << "   " << "\n";
    cout << "                             k :  " << bk->params->tgsw_params->tlwe_params->k << "   " << "\n";
    cout << "                      TLWE std :  " << bk->params->tgsw_params->tlwe_params->alpha_min << "   " << "\n";
    cout << "\n";
    cout << "                        --TGSW PARAMS--                                              " << "\n";
    cout << "                            Bg :  " << bk->params->tgsw_params->Bg << "               " << "\n";
    cout << "                             l :  " << bk->params->tgsw_params->l << "                " << "\n";
    cout << "\n";
    float time = -clock();
    for(int i = 0; i < mes; i++){
        bootsAND(res, tmp1, tmp2, bk);
    }
    time += clock();
    time = time/(mes * CLOCKS_PER_MS);
    cout << "        Time per Bootstrapping :  " << time << " ms \n";
    get_time(time);


    delete_gate_bootstrapping_ciphertext(tmp1);
    delete_gate_bootstrapping_ciphertext(tmp2);
    delete_gate_bootstrapping_ciphertext(res);
}

void comp_add(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                           1. Addition                           |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "       We will evaluate the time taken for " << length << "-bit Addition! \n\n";
    
    cout << "                newADD           |              HomADD                 " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |              5n - 3                 \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(5*length - 3) << " ms \n";

    float time = -clock();
    newADD(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomAdd(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int64_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }

    int64_t res_ans = 0;
    res_ans = encode_to(plain_a + plain_b, length);
    answer = encode_to(answer, length);
    answer2 = encode_to(answer2, length);
    
    cout << "              Plaintext Addition :  " << plain_a << " + "<< plain_b << " = " << res_ans << "                " << "\n";
    cout << "                 newADD Addition :  " << answer << " \n";
    cout << "                 HomADD Addition :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_subt(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                          2. Subtraction                         |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "       We will evaluate the time taken for " << length << "-bit Subtraction! \n\n";
    
    cout << "                newSUB           |              HomSUB                 " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |              5n - 3                 \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(5*length - 3) << " ms \n";

    float time = -clock();
    newSUB(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomSubt(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int64_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }

    int64_t res_ans = 0;
    res_ans = encode_to(plain_a - plain_b, length);
    answer = encode_to(answer, length);
    answer2 = encode_to(answer2, length);

    cout << "           Plaintext Subtraction :  " << plain_a << " - "<< plain_b << " = " << res_ans << "                " << "\n";
    cout << "              newADD Subtraction :  " << answer << " \n";
    cout << "              HomADD Subtraction :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_CompS(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext(bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                         3. Compare Small                        |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "    We will evaluate the time taken for " << length << "-bit Small-Comparison! \n\n";
    
    cout << "               newCompS          |        HomCompS (Unsigned)                  " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |                3n                \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(3*length) << " ms \n";

    float time = -clock();
    newCompS(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomCompS(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    answer = bootsSymDecrypt(res1, key);
    
    // answer = bootsSymDecrypt(compres1, key);

    int64_t answer2 = 0;
    answer2 = bootsSymDecrypt(res2, key);
    cout << "            Plaintext Comparison :  " << plain_a << " < "<< plain_b << " ? = " << boolalpha <<(plain_a < plain_b) << "                " << "\n";
    cout << "             newCompS Comparison :  " <<  boolalpha << (bool) answer << " \n";
    cout << "             HomCompS Comparison :  " <<  boolalpha << (bool) answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext(res1);
    delete_gate_bootstrapping_ciphertext(res2);
}

void comp_CompB(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext(bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                         4. Compare Large                        |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "    We will evaluate the time taken for " << length << "-bit Large-Comparison! \n\n";
    
    cout << "               newCompB          |        HomCompB (Unsigned)                 " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |                3n                \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(3*length) << " ms \n";

    float time = -clock();
    newCompB(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomCompB(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int32_t answer = 0;
    answer = bootsSymDecrypt(res1, key);
    
    // answer = bootsSymDecrypt(compres1, key);

    int32_t answer2 = 0;
    answer2 = bootsSymDecrypt(res2, key);
    cout << "            Plaintext Comparison :  " << plain_a << " > "<< plain_b << " ? = " << boolalpha <<(plain_a > plain_b) << "                " << "\n";
    cout << "             newCompB Comparison :  " <<  boolalpha << (bool) answer << " \n";
    cout << "             HomCompB Comparison :  " <<  boolalpha << (bool) answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext(res1);
    delete_gate_bootstrapping_ciphertext(res2);
}

void comp_CompSE(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext(bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                     5. Compare Small or Equal                   |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "We will evaluate the time taken for " << length << "-bit Small or Equal-Comparison! \n\n";
    
    cout << "              newCompSE          |        HomCompSE (Unsigned)                " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |                3n                \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(3*length) << " ms \n";

    float time = -clock();
    newCompSE(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomCompSE(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int32_t answer = 0;
    answer = bootsSymDecrypt(res1, key);
    
    // answer = bootsSymDecrypt(compres1, key);

    int32_t answer2 = 0;
    answer2 = bootsSymDecrypt(res2, key);
    cout << "            Plaintext Comparison :  " << plain_a << " <= "<< plain_b << " ? = " << boolalpha <<(plain_a <= plain_b) << "                " << "\n";
    cout << "            newCompSE Comparison :  " <<  boolalpha << (bool) answer << " \n";
    cout << "            HomCompSE Comparison :  " <<  boolalpha << (bool) answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext(res1);
    delete_gate_bootstrapping_ciphertext(res2);
}

void comp_CompLE(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext(bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext(bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                     6. Compare Large or Equal                   |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "We will evaluate the time taken for " << length << "-bit Large ore Equal-Comparison! \n\n";
    
    cout << "               newCompLE         |        HomCompLE (Unsigned)                " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     2n - 1           |                3n                \n";  
    cout << "     Est  :   " << bs_time*(2*length-1) << " ms         |            " << bs_time*(3*length) << " ms \n";

    float time = -clock();
    newCompLE(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomCompLE(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int32_t answer = 0;
    answer = bootsSymDecrypt(res1, key);
    
    // answer = bootsSymDecrypt(compres1, key);

    int32_t answer2 = 0;
    answer2 = bootsSymDecrypt(res2, key);
    cout << "            Plaintext Comparison :  " << plain_a << " >= "<< plain_b << " ? = " << boolalpha <<(plain_a >= plain_b) << "                " << "\n";
    cout << "             newCompS Comparison :  " <<  boolalpha << (bool) answer << " \n";
    cout << "             HomCompS Comparison :  " <<  boolalpha << (bool) answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext(res1);
    delete_gate_bootstrapping_ciphertext(res2);
}

void comp_MAX(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                            7. Maximum                           |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "       We will evaluate the time taken for " << length << "-bit Maximum! \n\n";
    
    cout << "                newMAX           |              HomMAX                " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     4n - 1           |              7n - 3                  \n";  
    cout << "     Est  :   " << bs_time*(4*length-1) << " ms         |            " << bs_time*(7*length - 3) << " ms \n";

    float time = -clock();
    newMax(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomsignedMax(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int32_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int32_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }
    cout << "               Plaintext Maximum :  max(" << plain_a << ", "<< plain_b << ") = " << (int32_t) max(plain_a, plain_b) << "                " << "\n";
    cout << "                  newMAX Maximum :  " << answer << " \n";
    cout << "                  HomMAX Maximum :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_Min(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                            8. Minimum                           |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "       We will evaluate the time taken for " << length << "-bit Minimum! \n\n";
    
    cout << "                newMin           |              HomMin                " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     4n - 1           |              7n - 3                  \n";  
    cout << "     Est  :   " << bs_time*(4*length-1) << " ms         |            " << bs_time*(7*length - 3) << " ms \n";

    float time = -clock();
    newMin(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomsignedMin(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int32_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int32_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }
    cout << "               Plaintext Minimum :  min(" << plain_a << ", "<< plain_b << ") = " << (int32_t) min(plain_a, plain_b) << "                " << "\n";
    cout << "                  newMin Minimum :  " << answer << " \n";
    cout << "                  HomMin Minimum :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_Abs(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                         9. Absolute Value                       |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext  :  " << plain_a << "\n\n";
    cout << " We will evaluate the time taken for " << length << "-bit Absolute Value! \n\n";
    
    cout << "                newABS           |              HomABS                " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :     3n - 1           |              4n - 3                  \n";  
    cout << "     Est  :   " << bs_time*(3*length-1) << " ms         |            " << bs_time*(4*length - 3) << " ms \n";

    float time = -clock();
    newABS(res1, a, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomAbs(res2, a, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }

    int64_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }

    int64_t res_ans = 0;
    res_ans = encode_to(abs(plain_a), length);
    
    cout << "              Plaintext Absolute :  abs(" << plain_a << ") = " << res_ans << "                " << "\n";
    cout << "                 newABS Absolute :  " << answer << " \n";
    cout << "                 HomABS Absolute :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_Mult(int64_t plain_a, int64_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                        10. Multiplication                       |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "     We will evaluate the time taken for " << length << "-bit Multiplication! \n\n";
    
    cout << "                newMult          |             HomMult                 " << "\n";
    cout << "                                 |\n";
    cout << "  # of op : 3/2n^2 + 15/2n - 1   |          3n^2 + 6n - 5                 \n";  
    cout << "     Est  :   " << bs_time*((double) 1.5*pow(length, 2) + 7.5*length -1) << " ms         |            " << bs_time*((double) 3*pow(length, 2) + 6*length - 5) << " ms \n";

    float time = -clock();
    newMult(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomMult(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :   " << time << " ms         |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int64_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }

    int64_t res_ans = 0;
    res_ans = encode_to(plain_a*plain_b, length);
    answer = encode_to(answer, length);
    answer2 = encode_to(answer2, length);

    cout << "        Plaintext Multiplication :  " << plain_a << " * "<< plain_b << " = " << res_ans << "                " << "\n";
    cout << "          newMult Multiplication :  " << answer << " \n";
    cout << "          HomMult Multiplication :  " << answer2 << " \n\n";

    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}

void comp_Div(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key) {
    LweSample* res1 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* res2 = new_gate_bootstrapping_ciphertext_array(length, bk->params);

    cout << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "|                           11. Division                          |" << "\n";
    cout << "-------------------------------------------------------------------" << "\n";
    cout << "\n                    plaintext 1  :  " << plain_a << "\n";
    cout << "                    plaintext 2  :  " << plain_b << "\n\n";
    cout << "     We will evaluate the time taken for " << length << "-bit Division! \n\n";
    
    cout << "                 newDiv          |             HomDiv                 " << "\n";
    cout << "                                 |\n";
    cout << "  # of op :   15/2n^2 + n        |       27/2n^2 - 3/2n - 5        \n";  
    cout << "     Est  :    " << bs_time*((double) 7.5*pow(length, 2) + length) << " ms        |            " << bs_time*((double) 13.5*pow(length, 2) - 1.5*length - 5) << " ms \n";

    float time = -clock();
    newRealDiv(res1, a, b, length, bk);
    time += clock();
    time = time/CLOCKS_PER_MS;

    float time2 = -clock();
    HomRealDiv(res2, a, b, length, bk);
    time2 += clock();
    time2 = time2/CLOCKS_PER_MS;
    
    cout << "    Real  :    " << time << " ms        |            " << time2 << " ms \n";
    cout << "\n";
    cout << "                     " << time2/time << "x times of Speedup! \n";
    cout << "\n";
    int64_t answer = 0;
    for (int i=0; i<length; i++) {
        int64_t ai = bootsSymDecrypt(&res1[i], key);
        answer |= (ai<<i);
    }
    
    // answer = bootsSymDecrypt(compres1, key);

    int64_t answer2 = 0;
    for (int i=0; i<length; i++) {
        int64_t ai2 = bootsSymDecrypt(&res2[i], key);
        answer2 |= (ai2<<i);
    }

    cout.precision(10);
    cout << "              Plaintext Division :  " << plain_a << " / "<< plain_b << " = " << fixed <<(double) plain_a/plain_b << "                " << "\n";
    cout << "                 newDiv Division :  " << fixed << (double)answer/pow(2, length/2) << " \n";
    cout << "                 HomDiv Division :  " << fixed << (double)answer2/pow(2, length/2) << " \n\n";
    cout.unsetf(ios::fixed);
    cout.precision(6);
    delete_gate_bootstrapping_ciphertext_array(length, res1);
    delete_gate_bootstrapping_ciphertext_array(length, res2);
}
