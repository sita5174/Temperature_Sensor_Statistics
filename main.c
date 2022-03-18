#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "temp_api.h"

int main(int argc, char **argv){
	int arg;
	uint8_t month_n = 0;
	int flag_m = 0, flag_f = 0, flag_h = 0;
	unsigned int size;
	char *name = {0};
	FILE *open;
	struct Sensor *data;

	while ((arg = getopt(argc, argv, "hf:m:")) != -1){
        	switch (arg){
			case '?':
				return 0;
			case 'm':
				flag_m = 1;
				month_n = atoi(optarg);
			break;
			case 'f':
				flag_f = 1;
				name = optarg;
			break;
			case 'h':
				flag_h = 1;
			break;
		}
	}

	
	if (flag_h == 0 && flag_f == 0 && flag_m == 0){
		printf("The application requires options.\nTo see help: ./prog -h\n");
		return 0;
	}

	if (flag_h){
		print_help();
		return 0;
	}

	if (flag_m && flag_f == 0){
		printf("The application requires option 'f'.\nTo see help: ./prog -h\n");
		return 0;
	}

	if (flag_m && !(month_n > 0 && month_n < 13)){
		printf("Wrong month number in option \'m\'.\nTo see help: ./prog -h\n");
		return 0;
	}

	data = malloc(366*24*60*sizeof(struct Sensor));
	open = fopen(name, "r");
	if (open == NULL){
		printf("File is not found: ");
		printf("\"%s\"\nTo see help: ./prog -h\n", name);
		return 0;
	}
	fclose(open);
	print_head();
	size = fill_data(data, name); // заполняем массив
	report_month(data, size, month_n); // выводим месячную статистику
	printf("\n");
	if (month_n == 0)
		report_year(data, size); // выводим годовую статистику

	free(data);
	return 0;
}
