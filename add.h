#include <tfhe/numeric_functions.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_core.h>
#include <tfhe/tfhe_io.h>
#include <math.h>
#include <tfhe/numeric_functions.h>
#include <tfhe/lweparams.h>
#include <tfhe/lwekey.h>
#include <tfhe/lwesamples.h>
#include <tfhe/lwekeyswitch.h>
#include <tfhe/lwe-functions.h>
#include <tfhe/lwebootstrappingkey.h>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// void HomAdd(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);

void tfhe_bootstrap_rotated_woKS_FFT(LweSample *result,
                                    const LweBootstrappingKeyFFT *bk,
                                    Torus32 mu,
                                    const LweSample *x);

void tfhe_rotate_bootstrap_FFT(LweSample *result,
                               const LweBootstrappingKeyFFT *bk,
                               Torus32 mu,
                               const LweSample *x);

void bootsCarry(LweSample* result, const LweSample* ca, const LweSample* cb, const LweSample* cc, const TFheGateBootstrappingCloudKeySet *bk);
void bootsSum(LweSample* result, const LweSample* ca, const LweSample* cb, const LweSample* cc, const TFheGateBootstrappingCloudKeySet *bk);
void newADD(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);

void bootsBorrow(LweSample* result, const LweSample* ca, const LweSample* cb, const LweSample* cc, const TFheGateBootstrappingCloudKeySet *bk);
void bootsCompS(LweSample* result, const LweSample* ca, const LweSample* cb, const LweSample* cc, const TFheGateBootstrappingCloudKeySet *bk);
void bootsCompB(LweSample* result, const LweSample* ca, const LweSample* cb, const LweSample* cc, const TFheGateBootstrappingCloudKeySet *bk);

void newSUB(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newABS(LweSample* res, const LweSample* a, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newCompS(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newCompB(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newCompSE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newCompLE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newMax(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newMin(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);


void newMultiReal(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newRealDiv(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);
void newMult(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk);



