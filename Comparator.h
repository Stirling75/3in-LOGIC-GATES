#include <tfhe/numeric_functions.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_core.h>
#include <tfhe/tfhe_io.h>
#include <math.h>
#include <tfhe/numeric_functions.h>
#include <tfhe/lweparams.h>
#include <tfhe/lwe-functions.h>
#include <tfhe/lwebootstrappingkey.h>
#include <iostream>
#include "add.h"
#include <type_traits>
#include <typeinfo>

using namespace std;

int64_t encode_to (int64_t res, const int length);
int32_t getNumber();
void get_time (float time);
int32_t getsecparam();

void boot_eval(int sec_param, int32_t length, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_add(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_subt(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_CompS(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_CompB(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_CompSE(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_CompLE(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_MAX(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_Min(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_Abs(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_Mult(int64_t plain_a, int64_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);
void comp_Div(int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, const int length, float bs_time, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);


void comp_ft (int32_t plain_a, int32_t plain_b, const LweSample* a, const LweSample* b, int sec_param, int32_t length, const TFheGateBootstrappingCloudKeySet* bk, TFheGateBootstrappingSecretKeySet* key);