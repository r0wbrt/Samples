/*
Copyright (c) 2015, Robert Christian Taylor
All rights reserved.
*/

#include <stdint.h>
enum t_bpsk_tx_return {NEED_DATA, ERROR, SAMPLE_GIVEN, NO_SAMPLE_GIVEN};


enum t_bpsk_bool {TRUE, FALSE};



typedef struct obj_bpsk_tx obj_bpsk_tx;


enum t_bpsk_tx_return bpsk_get_next_sample(uint32_t * out, obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_reset_tx_state(obj_bpsk_tx * r);
obj_bpsk_tx * bpsk_init_tx();
enum t_bpsk_bool bpsk_set_symbol_length(int32_t length, obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_reset_tx_state(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_make_tx_inactive(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_make_tx_active(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_set_accumulator_increment(int32_t incr, obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_set_amplitude(uint32_t amplitude, obj_bpsk_tx * r);
enum t_bpsk_bool set_data_word(uint32_t data, int32_t size, obj_bpsk_tx * r);
int32_t bpsk_get_sent_word_length(obj_bpsk_tx * r) ;
enum t_bpsk_bool bpsk_set_sample_size(int32_t size, obj_bpsk_tx * r);
int32_t bpsk_get_sample_size(obj_bpsk_tx * r);
uint32_t bpsk_get_state(obj_bpsk_tx * r);
enum t_bpsk_bool bpsk_need_data(obj_bpsk_tx * r);
void bpsk_free_tx(obj_bpsk_tx * r);

