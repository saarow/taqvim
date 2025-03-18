#include "utils.h"
#include <curl/curl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Date gregorian_to_jalali(Date greg) {
    Date jalali;
    int days_sum[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    if (greg.year <= 1600) {
        greg.year -= 621;
        jalali.year = 0;
    } else {
        greg.year -= 1600;
        jalali.year = 979;
    }

    int temp = (greg.year > 2) ? (greg.year + 1) : greg.year;
    int days = ((int)((temp + 3) / 4)) + (365 * greg.year) -
               ((int)((temp + 99) / 100)) - 80 + days_sum[greg.month - 1] +
               ((int)((temp + 399) / 400)) + greg.day;

    jalali.year += 33 * ((int)(days / 12053));
    days %= 12053;
    jalali.year += 4 * ((int)(days / 1461));
    days %= 1461;

    if (days > 365) {
        jalali.year += (int)((days - 1) / 365);
        days = (days - 1) % 365;
    }

    jalali.month =
        (days < 186) ? 1 + (int)(days / 31) : 7 + (int)((days - 186) / 30);

    jalali.day = 1 + ((days < 186) ? (days % 31) : ((days - 186) % 30));

    return jalali;
}

Date get_current_date(void) {
    Date curr_time;

    time_t now = time(NULL);
    if (now == -1) {
        perror("Failed to get current time\n");
        return curr_time;
    }

    struct tm *local_time = localtime(&now);
    if (local_time == NULL) {
        perror("Failed to convert time to local time\n");
        return curr_time;
    }

    curr_time.year = local_time->tm_year + 1900;
    curr_time.month = local_time->tm_mon + 1;
    curr_time.day = local_time->tm_mday;

    return curr_time;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char **response = (char **)userp;

    *response = realloc(*response, strlen(*response) + realsize + 1);
    if (response == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }
    memcpy(*response + strlen(*response), contents, realsize);
    (*response)[strlen(*response) + realsize] = '\0';

    return realsize;
}

char *fetch_holidays(void) {
    char *api_url = "https://holidayapi.ir/jalali/1403/11/22";

    CURL *curl;
    CURLcode res;
    char *response = malloc(1);
    response[0] = '\0';

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, api_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            free(response);
            response = NULL;
        }

        curl_easy_cleanup(curl);
    }
    return response;
}
