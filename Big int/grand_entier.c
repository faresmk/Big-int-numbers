#include <stdlib.h>
#include "grand_entier.h"
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

grand_entier_t *ge_cree(void) { 
	grand_entier_t *number = malloc(sizeof(grand_entier_t));
	int i;
	if(number!=NULL){ 
	    for (i = 0; i<32; i++)
	        number->value[i]= 0u; 
	    return number;
	}
	else{
		exit(65);
	}
}

void ge_libere(grand_entier_t *e) {
   free(e);
}

grand_entier_t *ge_set_bit(grand_entier_t *e, uint32_t x) {
	// Moving from a O(N^2) complexity to a O(1) complexity
	e->value[x/32] = e->value[x/32] | (1 << x%32);
	return e;
}

grand_entier_t *ge_clr_bit(grand_entier_t *e, uint32_t x) {
	// Moving from a O(N^2) complexity to a O(1) complexity
	e->value[x/32] = e->value[x/32] & ~(1 << x%32);
	return e;
}

char ge_get_bit(grand_entier_t *e, uint32_t x) {
    int l=0;
    char charValue = (char) l;
	// Moving from a O(N^2) complexity to a O(1) complexity
	if (((e->value[x/32] >> x%32) & 1)== 1)
        l=1;
	else
		l=0;
        charValue = (char) l;
        return charValue;

}
int ge_nb_bits(grand_entier_t *e) {
	// a bit better since it stops at the byte where it finds the most significant bit
	// in the worst case it becomes O(N^2)
	int i, co;
	uint32_t temp;
	for(i=31; i>=0; i--){
		co= 0;
		temp = e->value[i];
		while(temp != 0){
			temp >>= 1;
			co+= 1;
		}
		if(co!= 0)
			return i*32 + co;
	}
	return 0;
}

void ge_clear(grand_entier_t *e){
	int i;
	for(i =0; i<32; i++)
	  e->value[i]=0;
}

grand_entier_t *ge_add(grand_entier_t *b, grand_entier_t *a) {
	// since + complexity is O(1) then the complexity of this function is O(N) where N is nbr of bytes == 32
	int i;
	uint32_t temp, c=0u; //c: carry
	for(i=0; i<32; i++){ // since + complexity is O(1) then the complexity of this function is O(N) where N is nbr of bytes == 32
		temp = b->value[i];
		b->value[i] += a->value[i] + c;
		c=0u;
		if ((UINT_MAX - temp) < a->value[i])
	  		c=1u;
	}
	return b;
}

grand_entier_t *ge_shift(grand_entier_t *a, int nb_bits) {
	int i, k= nb_bits/32; // or u can use k= b>>5
    if(nb_bits >= 1024 || nb_bits <= -1024)
        ge_clear(a);
    else{
        if(nb_bits%32 == 0)
            for(i= 31; i>=k; i--)
                a->value[i] = ((a->value[i-k]) << (nb_bits%32));
        else{
            for(i= 31; i>k; i--)
                a->value[i] = ((a->value[i-k]) << (nb_bits%32)) + ((a->value[i-k-1]) >> (32-(nb_bits%32)));
            a->value[i] = (a->value[i-k]) << (nb_bits%32);
        }
        for(i= 0; i<k; i++)
            a->value[i]= 0;
    }
    return a;
}


grand_entier_t *ge_mul(grand_entier_t *b, grand_entier_t *a) {
	int i;
	uint32_t k = ge_nb_bits(a) - 1;  // position of most significant bit
	
	grand_entier_t *e = ge_cree();
	grand_entier_t *copy = ge_cree();
	
	while(k>0){
		for(i=0; i<32; i++)
			copy->value[i] = b->value[i];
		if(ge_get_bit(a,k))
			ge_add(e, ge_shift(copy, k));
		k--;
	}
	
	if(ge_get_bit(a,k)){
		for(i=0; i<32; i++)
			copy->value[i] = b->value[i];
		ge_add(e, copy);
	}
	
	for(i=0; i<32; i++)
		b->value[i] = e->value[i];
	ge_libere(e);
	ge_libere(copy);
	return b;
}




