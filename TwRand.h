#pragma once

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

class CTwRand
{
public:
	CTwRand(void);
	~CTwRand(void);

	unsigned long CTwRand::genrand_int32(void);

protected:
	void init_genrand(unsigned long s);
	void init_by_array(unsigned long init_key[], int key_length);

	unsigned long mt[N]; /* the array for the state vector  */
	int mti;
};
