/*
Copyright (c) 2015, Robert Christian Taylor
All rights reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bpsk.h"






int main()
{
	obj_bpsk_tx * tx;
	char data[] = "Hello World!";
	uint32_t in;
	size_t i, s;
	enum t_bpsk_tx_return ret;
	uint32_t sample;
	ret = SAMPLE_GIVEN;
	s = strlen(data);
	i = 0;

	tx = bpsk_init_tx();
	
	bpsk_set_symbol_length(20, tx);
	bpsk_set_accumulator_increment(11916, tx);

	if(bpsk_make_tx_active(tx)!=TRUE)
	{
		//printf("System Failure\r\n");
		return -1;
	}
	in = data[0];
	set_data_word(in, 8, tx);
	
	while(i < s) {
		in = data[i];

		if(ret == NEED_DATA)
		{
			set_data_word(in, 8, tx);
		}

		while(1==1)
		{
			ret = bpsk_get_next_sample(&sample, tx);
				if(ret!=SAMPLE_GIVEN){
					break;}
			fwrite((const void *)&(sample), sizeof(uint32_t), 1, stdout);
			//printf("%i", sample); 
		
		}
		
		if(ret==ERROR)
		{
			return -1;
		}
	
		i++;	
	}
	bpsk_free_tx(tx);
	
	return 0;
}
