/*
Copyright (c) 2015, Robert Christian Taylor
All rights reserved.
*/

#include <stdlib.h>
#include <stdint.h>
#include "costable.h"


enum t_bpsk_bool {TRUE, FALSE};
enum t_bpsk_tx_return {NEED_DATA, ERROR, SAMPLE_GIVEN, NO_SAMPLE_GIVEN};
enum t_bpsk_symbol {ONE, ZERO};



struct obj_bpsk_tx{
	enum t_bpsk_symbol symbol;
	int32_t symbol_index;
	int32_t symbol_length;
	int32_t data_index;
	int32_t data_length;	
	uint32_t data;
	enum t_bpsk_bool first_symbol, need_next_word, 
		will_need_next_word, need_next_symbol,
		error, tx_idle;

	int32_t accumulator;
	int32_t accumulator_increment;
};

typedef struct obj_bpsk_tx obj_bpsk_tx;


enum t_bpsk_tx_return bpsk_get_next_sample(uint32_t * out, obj_bpsk_tx * r)  __attribute__ ((hot));
enum t_bpsk_bool bpsk_reset_tx_state(obj_bpsk_tx * r);
obj_bpsk_tx * bpsk_init_tx();
enum t_bpsk_bool bpsk_set_symbol_length(int32_t length, obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_reset_tx_state(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_make_tx_inactive(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_make_tx_active(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_set_accumulator_increment(int32_t incr, obj_bpsk_tx * r);
enum t_bpsk_bool set_data_word(uint32_t data, int32_t size, obj_bpsk_tx * r);
int32_t bpsk_get_sent_word_length(obj_bpsk_tx * r) ;
int32_t bpsk_get_sample_size(obj_bpsk_tx * r) __attribute__((cold));
uint32_t bpsk_get_state(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_need_data(obj_bpsk_tx * r) __attribute__((hot));
void bpsk_free_tx(obj_bpsk_tx * r);
