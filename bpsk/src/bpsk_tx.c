/*
Copyright (c) 2015, Robert Christian Taylor
All rights reserved.
*/

#include "bpsk_internal.h"
#include <string.h>



static int get_next_bit(uint32_t word, int32_t index);
void inline bpsk_set_initial_conditions(obj_bpsk_tx * r);
static inline uint32_t generate_sample(obj_bpsk_tx * r) __attribute__((hot));

static int get_next_bit(uint32_t word, int32_t index) 
{
	return (((uint32_t)word >> (index)) & (uint32_t)1);
}

obj_bpsk_tx * bpsk_init_tx()
{
	obj_bpsk_tx * r = (obj_bpsk_tx *)malloc(sizeof(obj_bpsk_tx));
	memset((void *)r, 0, sizeof(obj_bpsk_tx));
	bpsk_reset_tx_state(r);
	r->symbol_length = 12;
	r->accumulator_increment = 17873; /*12Khz at 44Ksps*/

	return r;

}

void bpsk_free_tx(obj_bpsk_tx * r)
{
	free(r);
}
enum t_bpsk_bool bpsk_set_symbol_length(int32_t length, obj_bpsk_tx * r)
{
	if(length <= 0)
	{
		r->error = TRUE;
		return FALSE;
	}
	r->symbol_length = length;
		return TRUE;
}



void inline bpsk_set_initial_conditions(obj_bpsk_tx * r) 
{

	r->symbol_index = 0;
	r->data_index = 0;
	

	r->first_symbol = TRUE;
	r->need_next_word = FALSE;
	r->will_need_next_word = FALSE;
	r->need_next_symbol = FALSE;

}
enum t_bpsk_bool bpsk_reset_tx_state(obj_bpsk_tx * r)
{

	bpsk_set_initial_conditions(r);
	r->error = FALSE;
	r->tx_idle = TRUE;
	r->accumulator = 0;

	return TRUE;
}
enum t_bpsk_bool bpsk_make_tx_inactive(obj_bpsk_tx * r)
{
	if(r->error == TRUE)
		return FALSE;

	bpsk_set_initial_conditions(r);
	r->tx_idle = TRUE;

	return TRUE;
}
enum t_bpsk_bool bpsk_make_tx_active(obj_bpsk_tx * r)
{
	if(r->error == TRUE)
		return FALSE;

	bpsk_set_initial_conditions(r);
	r->tx_idle = FALSE;


	return TRUE;
}
enum t_bpsk_bool bpsk_set_accumulator_increment(int32_t incr, obj_bpsk_tx * r)
{
	if(incr < 0)
		return FALSE;
	if(incr > 65536)
		return FALSE;
	r->accumulator_increment = incr;
	return TRUE;
}
enum t_bpsk_bool set_data_word(uint32_t data, int32_t size, obj_bpsk_tx * r)
{
	if(size < 0)
		return FALSE;
	if(size > 0)
	{
		r->will_need_next_word = FALSE;
		r->need_next_word = FALSE;
	}

	r->data_length = size;
	r->data = data;
	r->data_index = 0;

	return TRUE;

}
int32_t bpsk_get_sent_word_length(obj_bpsk_tx * r) 
{
	return r->data_index;
}

enum t_bpsk_bool bpsk_need_data(obj_bpsk_tx * r)
{
	if(r->will_need_next_word == TRUE)
		return TRUE;

	return r->need_next_word;
}

static inline uint32_t generate_cos(int32_t phase)
{
	if(phase < 0)
		return 0;
	int32_t p = phase;

	if(phase < 16384)
		return (cos_table[((uint32_t)phase >> 7)] + 32768);
	else if (phase < 32768)
	{
		p = 32768 - p;
		return (32768 - cos_table[((uint32_t)p >> 7)]);
	}
	else if (phase < 49152)
	{
		p = p - 32768;
		return (32768 - cos_table[((uint32_t)p >> 7)]);
	}
	else if (phase < 65536)
	{
		p = 65536 - p;
		return (cos_table[((uint32_t)p >> 7)] + 32768);
	}
	else
		return 0;
}
static inline void increment_accumulator(obj_bpsk_tx * r)
{
	int32_t acc;
	int32_t incr;

	incr = r->accumulator_increment;
	acc = r->accumulator;


	if(acc < 0)
	{
		r->error = TRUE;
		return ;
	}

	acc = acc - incr;

	if(acc < 0)
		acc = acc + 65536;

	r->accumulator = acc;

}
static inline uint32_t generate_sample(obj_bpsk_tx * r)  
{

	int32_t acc = r->accumulator;
	uint32_t wave;

	if(r->symbol == 1)
	{
		acc = acc - 32768; 
		if(acc < 0)
			acc = acc + 65536;
	}


	if(acc < 0)
	{
		r->error = TRUE;
		return 0;
	}
	wave = generate_cos(acc);


	return wave;


}


enum t_bpsk_tx_return bpsk_get_next_sample(uint32_t * out, obj_bpsk_tx * r)  
{
	uint32_t bit; 

	if(r->error == TRUE)
		return ERROR;


	if(r->tx_idle == TRUE)
	{
		increment_accumulator(r);
		return NO_SAMPLE_GIVEN;
	}

	if(r->need_next_word == TRUE)
		return NEED_DATA;

	if(r->first_symbol == TRUE)
	{
		r->symbol = ZERO;
		r->symbol_index = 0;
		r->first_symbol = FALSE;

		if(r->data_index >=  r->data_length)
			r->will_need_next_word = TRUE;

	} else if(r->need_next_symbol == TRUE) {
		if(r->data_index >= r->data_length)
		{
			r->error = TRUE;
			return ERROR;
		}
		bit = get_next_bit(r->data, r->data_index);
		if(bit == 1)
		{
			if(r->symbol == ZERO)
				r->symbol = ONE;
			else
				r->symbol = ZERO;
		} else if(bit!=0) {
			r->error = TRUE;
			return ERROR;
		}
		r->symbol_index = 0;
		
		r->data_index++;
	
		if(r->data_index >=  r->data_length)
		{
			r->will_need_next_word = TRUE;	
		}

		r->need_next_symbol = FALSE;
		
	}

	*out = generate_sample(r);
	increment_accumulator(r);

	if(r->error == TRUE)
	{
		return ERROR;
	}

	r->symbol_index++;
	if((r->symbol_index) >= (r->symbol_length))
	{
		if(r->will_need_next_word == TRUE)
		{
			r->will_need_next_word = FALSE;
			r->need_next_word = TRUE;
			
		}
		r->need_next_symbol = TRUE;
	}

	return SAMPLE_GIVEN;
}


