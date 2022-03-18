#include <stdint.h>

struct Sensor{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	int8_t t;
};

// добавление записи
void add_rec(struct Sensor *data, int number, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, int8_t t);

// сбор данных и заполнение массива
unsigned int fill_data(struct Sensor *data, char *name);

// поменять записи местами
void swap_rec(struct Sensor *data, int i, int j);

// рассчет и вывод статистики по месяцам либо за один месяц
void report_month(struct Sensor *data, unsigned int size, uint8_t month);

// рассчет и вывод годовой статистики
void report_year(struct Sensor *data, unsigned int size);

// печать информации по одному месяцу
void print_line(unsigned int count, float avg, int8_t min, int8_t max);

// печать мануала
void print_help(void);

// печать шапки программы
void print_head(void);

// печать шапки таблицы
void print_tab_head(uint16_t year);
