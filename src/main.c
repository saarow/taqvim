#include "utils.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Date current_date = get_current_date();
    Date greg = {.day = current_date.day,
                 .month = current_date.month,
                 .year = current_date.year};
    Date jalali = gregorian_to_jalali(greg);
    printf("%d-%d-%d\n", jalali.year, jalali.month, jalali.day);
    char *holidays = fetch_holidays();
    printf("%s\n", holidays);
    return 0;
}
