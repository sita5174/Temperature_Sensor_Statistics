#include <stdio.h>
#include "temp_api.h"

// добавление записи
void add_rec(struct Sensor *data, int number, uint16_t year, uint8_t month,\
	     uint8_t day, uint8_t hour, uint8_t min, int8_t t){
	data[number].year = year;
	data[number].month = month;
	data[number].day = day;
	data[number].hour = hour;
	data[number].min = min;
	data[number].t = t;
}

// сбор данных и заполнение массива
unsigned int fill_data(struct Sensor *data, char *name){
	FILE *open = fopen(name, "r");
	FILE *err = NULL;
	uint16_t Y;
	uint8_t M, D, H, m;
	int8_t T;
	int n_scan;
	int line_c = 0, err_c = 0;
	unsigned int size = 0;

	while((n_scan = fscanf(open, "%hd;%hhd;%hhd;%hhd;%hhd;%hhd", &Y, &M, &D, &H, &m, &T)) > 0){
		if (n_scan < 6){
			char s[20], c;
			n_scan = fscanf(open, "%[^\n]%c", s, &c);
			if (err == NULL)
				err = fopen("sensor_errors.txt", "w");
			fprintf(err, "Wrong format in line %d: %s\n", line_c, s);
			++err_c;
		} else
			add_rec(data, size++, Y, M, D, H, m, T);
		++line_c;
	}

	// вывод информации об обработке файла
	printf("File \"%s\" was successfully loaded!\n", name);
	printf("%d lines processed, %d approved, %d rejected.\n", size + err_c, size, err_c);
	printf("(check \"sensor_errors.txt\" file for errors details)\n\n");

	fclose(open);
	if (err != NULL)
		fclose(err);
	return size;
}

// поменять записи местами
void swap_rec(struct Sensor *data, int i, int j){
	struct Sensor tmp;
	tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

// рассчет и вывод статистики по месяцам либо за один месяц
void report_month(struct Sensor *data, unsigned int size, uint8_t month){
	char *m[12] = {"Jan", "Feb", "Mar", "Apr", "May","Jun",\
		       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	int i, start, end;
	int8_t min, max;
	unsigned int j = 0, count = 0;
	int flag = 0;
	float avg = 0;

	// выясняем за какой период выводить статистику
	// и есть ли информация по указанному месяцу
	if (month == 0){
		start = 1;
		end = 13;
	}
	else {
		start = month;
		end = month + 1;
		while(data[j].month != month && j < size)
			++j;
		if (data[j].month != month){
			printf("Sorry, no data for month %hhd =(\n", month);
			return;
		}
	}

	print_tab_head(data[0].year);

	for (i = start; i < end; ++i){
		min = data[j].t;
		max = data[j].t;
		while (data[j].month == i){
			if (flag == 0)
				flag = 1;
			if (data[j].t > max)
				max = data[j].t;
			if (data[j].t < min)
				min = data[j].t;
			avg += data[j++].t;
			++count;
		}
		if (flag != 0){
			printf("%s%9s |", "|", m[i - 1]);
			print_line(count, (float)avg / count, min, max);
		}
		avg = 0;
		count = 0;
		flag = 0;
	}

	printf("------------------------------------------------------------");
}

// рассчет и вывод годовой статистики
void report_year(struct Sensor *data, unsigned int size){
	int i = 0;
	int avg = 0;
	int min = data[0].t;
	int max = data[0].t;
	for (i = 0; i < size; ++i){
		if (data[i].t > max)
			max = data[i].t;
		if (data[i].t < min)
			min = data[i].t;
		avg += data[i].t;
	}
	printf("%s%11s", "|", "Year |");
	print_line(size, (float)avg / size, min, max);
	printf("------------------------------------------------------------\n");
}

// печать информации по одному месяцу
void print_line(unsigned int count, float avg, int8_t min, int8_t max){
	printf("%10d |", count);
	printf("%10.1lf |", avg);
	printf("%10hhd |", min);
	printf("%10hhd |\n", max);
}

// печать мануала
void print_help(void){
	printf("\nThis application displays temperature change statistics\n");
	printf("based on sensor readings.\n\n");
	printf("-f <file_name>\n\tuse *.csv file to provide data to the application\n");
	printf("-m <month_number>\n\tspecify the number of the month to display\n");
	printf("statistics for this month only\n");
	printf("-h\n\tsee help\n\n");
}

// печать шапки программы
void print_head(void){
	printf("------------------------------------------------------------\n");
	printf("               TEMPERATURE SENSOR STATISTICS                \n");
	printf("------------------------------------------------------------\n\n");
}

// печать шапки таблицы
void print_tab_head(uint16_t year){
	printf("------------------------------------------------------------\n");
	printf("|                  Report for %hd year                    |\n", year);
	printf("------------------------------------------------------------\n");
	printf("%s%11s%12s%12s%12s%12s\n", "|", "Month |", "Lines |", "Avg |", "Min |", "Max |");
	printf("------------------------------------------------------------\n");
}

