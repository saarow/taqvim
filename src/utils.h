#pragma onece

#include <stddef.h>

typedef struct {
        int day;
        int month;
        int year;
} Date;

Date gregorian_to_jalali(Date greg);
Date get_current_date(void);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
char *fetch_holidays(void);
