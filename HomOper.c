#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

///////////////////////////////     Addition     ////////////////////////////
//res = a + b

void HomAdd(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* c = new_gate_bootstrapping_ciphertext_array(length, bk->params);	    
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    

	bootsCONSTANT(&c[0], 0, bk);
    
	for(int i = 0; i < length -1; i++){
		bootsXOR(&temp[0], &a[i], &b[i], bk);
		bootsAND(&temp[1], &a[i], &b[i], bk);
		bootsXOR(&res[i], &temp[0], &c[i], bk);
		bootsAND(&temp[0], &temp[0], &c[i], bk);
		bootsOR(&c[i+1], &temp[0], &temp[1], bk);
	}

	bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
	bootsXOR(&res[length-1], &temp[0], &c[length-1], bk);

	delete_gate_bootstrapping_ciphertext_array(length, c);    
	delete_gate_bootstrapping_ciphertext_array(2, temp);    
}


///////////////////////////////     2's complement     ////////////////////////////
//res = -a

void HomTwosComplement(LweSample* res, const LweSample* a, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);	    
	LweSample* b = new_gate_bootstrapping_ciphertext_array(length, bk->params);	    

	bootsCONSTANT(&b[0], 1, bk);
    

	for(int i = 0; i < length - 2; i++){
		bootsNOT(&temp[0], &a[i], bk);
		bootsXOR(&res[i], &temp[0], &b[i], bk);
		bootsAND(&b[i+1], &temp[0], &b[i], bk);}

	bootsNOT(&temp[0], &a[length-2], bk);
	bootsXOR(&res[length-2], &temp[0], &b[length-2], bk);

	bootsNOT(&res[length-1], &a[length-1], bk);
	

	delete_gate_bootstrapping_ciphertext_array(length, b);    
	delete_gate_bootstrapping_ciphertext_array(1, temp);    
}



///////////////////////////////     subtract     ////////////////////////////
//res = a - b

void HomSubt(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

    LweSample* c = new_gate_bootstrapping_ciphertext_array(length, bk->params);       
    LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);       

    bootsCONSTANT(&c[0], 0, bk);
    
    for(int i = 0; i < length -1; i++){
        bootsXOR(&temp[0], &a[i], &b[i], bk);
        bootsANDNY(&temp[1], &a[i], &b[i], bk);
        bootsXOR(&res[i], &temp[0], &c[i], bk);
        bootsANDNY(&temp[0], &temp[0], &c[i], bk);
        bootsOR(&c[i+1], &temp[1], &temp[0], bk);}

    bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
    bootsXOR(&res[length-1], &temp[0], &c[length-1], bk);

    delete_gate_bootstrapping_ciphertext_array(length, c);   
    delete_gate_bootstrapping_ciphertext_array(2, temp);   
} 



///////////////////////////////     left shift     ////////////////////////////

void HomLShift(LweSample* res, const LweSample* a, const int length, const int k, const TFheGateBootstrappingCloudKeySet* bk) {

	for(int i = 0; i < length - k; i++){
		bootsCOPY(&res[i], &a[i+k], bk);}
	for(int i = length-k; i < length; i++){
		bootsCOPY(&res[i], &a[length-1], bk);}
}



///////////////////////////////     right shift     ////////////////////////////

// void HomRShift(LweSample* res, const LweSample* a, const int length, const int k, const TFheGateBootstrappingCloudKeySet* bk) {

// 	for(int i = 0; i < k; i++){
// 		bootsCONSTANT(&res[i], 0, bk);}
// 	for(int i = k; i < length; i++){
// 		bootsCOPY(&res[i], &a[i-k], bk);}
// }


 ///////////////////////////////     right shift     ////////////////////////////

void HomRShift(LweSample* res, const LweSample* a, const int length, const int k, const TFheGateBootstrappingCloudKeySet* bk) {

    bootsCOPY(&res[length-1], &a[length-1], bk);
    for(int i = length-2; i > k-1; i--){
        bootsCOPY(&res[i], &a[i-k], bk);}
    for(int i = 0; i < k; i++){
        bootsCONSTANT(&res[i], 0, bk);}
} 

///////////////////////////////     equivalent or not     ////////////////////////////
// if a = b then res = E(1)
// else res = E(0)

void HomEqui(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    

	bootsCONSTANT(&temp[0], 1, bk);
	for(int i = 0; i < length; i++){		
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsAND(&temp[0], &temp[0], &temp[1], bk);
		
	}
	bootsCOPY(&res[0], &temp[0], bk);

	delete_gate_bootstrapping_ciphertext_array(2, temp);
}



///////////////////////////////     big comparison     ////////////////////////////
// if a > b then res = E(1)
// else res = E(0)

void HomCompB(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &a[i], bk);}

	bootsCOPY(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}




///////////////////////////////     small comparison     ////////////////////////////
// if a < b then res = E(1)
// else res = E(0)

void HomCompS(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &b[i], bk);}

	bootsCOPY(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}

///////////////////////////////     smaller than or equal to comparison     ////////////////////////////

void HomCompSE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) { // a <= b -> 1

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &a[i], bk);}

	bootsNOT(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}

///////////////////////////////     larger than or equal to comparison     ////////////////////////////

void HomCompLE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {  // a >= b -> 1

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &b[i], bk);}

	bootsNOT(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}



///////////////////////////////     maximum     ////////////////////////////
// if a > b then res = a

void HomMax(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	
	HomCompB(&temp[0], a, b, length, bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &temp[0], &a[i], &b[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(1, temp);
}

void HomsignedMax(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	LweSample* tmp = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* temp = new_gate_bootstrapping_ciphertext(bk->params);

	HomSubt(tmp, b, a, length, bk);
	bootsCOPY(temp, &tmp[length-1], bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], temp, &a[i], &b[i], bk);
	}

	delete_gate_bootstrapping_ciphertext_array(length, tmp);
	delete_gate_bootstrapping_ciphertext(temp);
}

///////////////////////////////     minimum     ////////////////////////////
// if a < b then res = a

void HomMin(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	
	HomCompS(&temp[0], a, b, length, bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &temp[0], &a[i], &b[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(1, temp);
}

void HomsignedMin(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	LweSample* tmp = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* temp = new_gate_bootstrapping_ciphertext(bk->params);

	HomSubt(tmp, a, b, length, bk);
	bootsCOPY(temp, &tmp[length-1], bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], temp, &a[i], &b[i], bk);
	}

	delete_gate_bootstrapping_ciphertext_array(length, tmp);
	delete_gate_bootstrapping_ciphertext(temp);
}


///////////////////////////////     absolute value     ////////////////////////////
// if a < b then res = a

void HomAbs(LweSample* res, const LweSample* a, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* na = new_gate_bootstrapping_ciphertext_array(length, bk->params);

	HomTwosComplement(na, a, length, bk);
	
	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &a[length-1], &na[i], &a[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(length, na);
}

///////////////////////////////   new  multiplication     ////////////////////////////

void HomMultiReal(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	LweSample* A = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* AA = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* B = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* C = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* D = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* E = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* F = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	
	HomAbs(A, a, length, bk);
	HomAbs(B, b, length, bk);
	for(int i = 0; i < length; i++){
		bootsAND(&AA[i], &A[i], &B[0], bk);}
	HomLShift(AA, AA, length, length/2, bk);	


	
	for(int i = 1; i < length-1; i++){
		if(i < length/2){
			HomLShift(C, A, length, length/2-i, bk);

			for(int j = 0; j < length; j++)
				bootsAND(&D[j], &C[j], &B[i], bk);

			HomAdd(AA, AA, D, length, bk);
		}
		else if(i == length/2){
			for(int j = 0; j < length; j++)
				bootsAND(&D[j], &A[j], &B[i], bk);

			HomAdd(AA, AA, D, length, bk);
		}

		else {
			HomRShift(C, A, length, i-length/2, bk);
			for(int j = 0; j < length; j++){
				bootsAND(&D[j], &C[j], &B[i], bk);}

			HomAdd(AA, AA, D, length, bk);
		}

	}
	bootsCONSTANT(&AA[length-1], 0, bk);

	HomTwosComplement(D, AA, length, bk);
	
	bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
	bootsNOT(&temp[1], &temp[0], bk);

	for(int i = 0; i < length; i++){
		bootsAND(&E[i], &AA[i], &temp[1], bk);
		bootsAND(&F[i], &D[i], &temp[0], bk);}

	HomAdd(res, E, F, length, bk);

	delete_gate_bootstrapping_ciphertext_array(2, temp);
	delete_gate_bootstrapping_ciphertext_array(length, A);
	delete_gate_bootstrapping_ciphertext_array(length, AA);
	delete_gate_bootstrapping_ciphertext_array(length, B);
	delete_gate_bootstrapping_ciphertext_array(length, C);
	delete_gate_bootstrapping_ciphertext_array(length, D);
	delete_gate_bootstrapping_ciphertext_array(length, E);
	delete_gate_bootstrapping_ciphertext_array(length, F);
}


void HomRealDiv(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(3, bk->params);
	LweSample* A = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* B = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* QR = new_gate_bootstrapping_ciphertext_array(2*length, bk->params);
	LweSample* D = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* C = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* Q = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* DD = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* R = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* E0 = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	
	HomAbs(A, a, length, bk);
	HomAbs(B, b, length, bk);
	//printf("absolute value clear\n");

	for(int i = 0; i < length; i++){
		bootsCOPY(&QR[i], &A[i], bk);
		bootsCOPY(&D[i], &B[i], bk);
		bootsCONSTANT(&QR[length + i], 0, bk);}
	

	HomRShift(QR, QR, 2*length, 1, bk);
//	printf("stting clear\n");
	for(int s = 1; s < length; s++){
		HomRShift(QR, QR, 2*length, 1, bk);

		for(int i = 0; i < length; i++){
			bootsCOPY(&R[i], &QR[length+i], bk);}

		HomCompS(&temp[0], R, D, length, bk);
		bootsNOT(&temp[1], &temp[0], bk);
		bootsCOPY(&QR[0], &temp[1], bk);

		for(int i = 0; i < length; i++){
			bootsAND(&DD[i], &D[i], &temp[1], bk);}

		HomSubt(R, R, DD, length, bk);

		for(int i = 0; i < length; i++){
			bootsCOPY(&QR[length+i], &R[i], bk);}
		
//		printf("%d loop clear\n",s);
	}
		


	for(int s = length; s < length*3/2; s++){
		HomRShift(QR, QR, 2*length, 1, bk);
		HomRShift(R, R, length, 1, bk);

		HomCompS(&temp[0], R, D, length, bk);
		bootsNOT(&temp[1], &temp[0], bk);
		bootsCOPY(&QR[0], &temp[1], bk);

		for(int i = 0; i < length; i++){
			bootsAND(&DD[i], &D[i], &temp[1], bk);}

		HomSubt(R, R, DD, length, bk);

		
//		printf("%d loop clear\n",s);
	}


	for(int i = 0; i < length; i++){
		bootsCOPY(&Q[i], &QR[i], bk);}
//		bootsCOPY(&res[i], &QR[i], bk);}



	HomTwosComplement(C, Q, length, bk);
	
	bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
	for (int i = 0; i < length; i++){
		bootsMUX(&res[i], &temp[0], &C[i], &Q[i], bk);
	}
	
	
	
	delete_gate_bootstrapping_ciphertext_array(3, temp);
	delete_gate_bootstrapping_ciphertext_array(length, A);
	delete_gate_bootstrapping_ciphertext_array(length, B);
	delete_gate_bootstrapping_ciphertext_array(2*length, QR);
	delete_gate_bootstrapping_ciphertext_array(length, D);
	delete_gate_bootstrapping_ciphertext_array(length, DD);
	delete_gate_bootstrapping_ciphertext_array(length, Q);
	delete_gate_bootstrapping_ciphertext_array(length, R);
	delete_gate_bootstrapping_ciphertext_array(length, C);
	delete_gate_bootstrapping_ciphertext_array(length, E0);
}


void HomMult(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
    LweSample* tmp = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* A = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* B = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* C = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* D = new_gate_bootstrapping_ciphertext_array(length, bk->params);
    LweSample* sign = new_gate_bootstrapping_ciphertext(bk->params);

	HomAbs(A, a, length, bk);
	HomAbs(B, b, length, bk);
    for(int i = 0; i < length; i++){
        bootsAND(&C[i], &A[i], &B[0], bk);
    }

    for(int i = 1; i < length; i++){
        for (int j = 0; j < length-i; j++){
            bootsAND(&tmp[j], &A[j], &B[i], bk);
        }
        HomAdd(C+i, C+i, tmp, length-i, bk);
    }

    // bootsCONSTANT(&res[length-1], 0, bk);

	bootsXOR(sign, &a[length-1], &b[length-1], bk);
	HomTwosComplement(D, C, length, bk);


    for(int i = 0; i < length; i++){
        // bootsXOR(&res[i], &res[i], sign, bk);
        // bootsCONSTANT(&tmp[i], 0, bk);
		bootsMUX(&res[i], sign, &D[i], &C[i], bk);
    }
    // bootsCOPY(&tmp[0], sign, bk);
    // HomAdd(res, res, tmp, length, bk);

	delete_gate_bootstrapping_ciphertext_array(length, tmp);
	delete_gate_bootstrapping_ciphertext_array(length, A);
	delete_gate_bootstrapping_ciphertext_array(length, B);
	delete_gate_bootstrapping_ciphertext(sign);
}


void is_negative(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  bootsCOPY(&result[0], &a[nb_bits-1], bk);
}

void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
  bootsXNOR(tmp, a, b, bk);
  bootsMUX(result, tmp, lsb_carry, a, bk);
}

void maximum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
    LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* aGreater = new_gate_bootstrapping_ciphertext_array(2, bk->params);

    LweSample* minimumMismoSigno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* minimumOneNegative = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* oneNegative = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* negativoA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* negativoB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

    is_negative(negativoA, a, nb_bits, bk);
    is_negative(negativoB, b, nb_bits, bk);
	bootsCONSTANT(&tmps[0], 0, bk);

    bootsXOR(&oneNegative[0], &negativoA[0], &negativoB[0], bk);

    // a > b = soloOneNegative & is_negative(b)
    bootsAND(&aGreater[0], &oneNegative[0], &negativoB[0], bk);
    for(int i = 0; i < nb_bits; i++){
      bootsMUX(&minimumOneNegative[i], &aGreater[0], &b[i], &a[i], bk);
    }

    //initialize the carry to 0
    

    //run the elementary comparator gate n times
    for (int i=0; i<nb_bits; i++) {
        compare_bit(&tmps[0], &a[i], &b[i], &tmps[0], &tmps[1], bk);
    }

    //tmps[0] is the result of the comparaison: 0 if a is larger, 1 if b is larger
    //select the max and copy it to the result
    for (int i=0; i<nb_bits; i++) {
        bootsMUX(&minimumMismoSigno[i], &tmps[0], &b[i], &a[i], bk);
        bootsMUX(&result[i], &oneNegative[0], &minimumMismoSigno[i], &minimumOneNegative[i], bk);
    }

    delete_gate_bootstrapping_ciphertext_array(2, tmps);
    delete_gate_bootstrapping_ciphertext_array(2, aGreater);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumMismoSigno);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumOneNegative);
    delete_gate_bootstrapping_ciphertext_array(2, oneNegative);
    delete_gate_bootstrapping_ciphertext_array(2, negativoA);
    delete_gate_bootstrapping_ciphertext_array(2, negativoB);

}

void HomMaxv2(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	LweSample* tmp = new_gate_bootstrapping_ciphertext_array(length, bk->params);

	HomSubt(tmp, b, a, length, bk);
	bootsCOPY(temp, &tmp[length-1], bk);

	for (int i = 0; i < length; i++){
		bootsMUX(&res[i], temp, &a[i], &b[i], bk);
	}
}