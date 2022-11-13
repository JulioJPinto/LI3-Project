#include <glib.h>

#include "parser.h"
#include "ride.h"
#include <stdio.h>

typedef void* (*parse_line_function_pointer)(char* string);

void read_file(const char* file_path, parse_line_function_pointer parse_line_function){
    FILE *csv_file = fopen(file_path,"r");

    static char buffer[10000];
    for(int i = 0; fgets(buffer, 10000, csv_file); i++){
        if (i == 0) fgets(buffer, 10000, csv_file);
        parse_line_function(buffer);
    }

}

int parse_int(char* string){
    return strtol(string,NULL, 10); 

}

double parse_double(char* string){
    return strtod(string,NULL);
}

Date *parse_date(char* string){
    Date *date_parsed = malloc(sizeof(struct date));

    char* svptr;

    date_parsed->day = parse_int(strtok_r(string,"/",&svptr)); 
    date_parsed->month = parse_int(strtok_r(NULL,"/",&svptr));
    date_parsed->year = parse_int(strtok_r(NULL,";",&svptr));

    return date_parsed;
}

void *print_date(Date *date){

    printf("%d/%d/%d;", date->day, date->month, date->year);
}

Gender parse_gender(char* string){

    if (strcmp(string, "F")){
        return F;
    } else {
        return M;
    }

}


void *print_gender(Gender gender){
    if (gender == F) printf("F;");
    else printf("M;");
}

CarClass parse_car_class(char* string){

    if(strcmp(string,"green") == 0){
        return GREEN;
    } else if(strcmp(string,"premium") == 0){
        return PREMIUM;
    } else {
        return BASIC;
    }
}

void *print_car_class(CarClass class){

    if(class == GREEN) printf("GREEN;");
    else if (class == PREMIUM) printf("PREMIUM;");
    else if (class == BASIC) printf("BASIC;");
}


AccountStatus parse_acc_status(char* string){

    if (strcmp(string, "active")){
        return ACTIVE;
    }
    
    return INACTIVE;  
}

void *print_acc_status(AccountStatus status){
    if (status == ACTIVE) printf("ACTIVE;\n");
    else printf("INACTIVE;\n");
}

PaymentMethod parse_pay_method(char* string){

    if(strcmp(string,"cash") == 0){
        return CASH;
    } else if(strcmp(string,"credit_card") == 0){
        return CREDIT;
    } else {
        return DEBIT;
    }

}


void* parse_driver(char* line){
    char* saveptr;

    char* idString = strtok_r(line,";",&saveptr); 
    int id = parse_int(idString);

    char* name = strtok_r(NULL,";",&saveptr);

    char* dateString = strtok_r(NULL,";",&saveptr);
    Date* date = parse_date(dateString);

    char* genderString = strtok_r(NULL,";",&saveptr);
    Gender gender = parse_gender(genderString);

    char* carClassString = strtok_r(NULL,";",&saveptr);
    CarClass car_class = parse_car_class(carClassString);

    char* license_plate = strtok_r(NULL,";",&saveptr);
    
    char* city = strtok_r(NULL,";",&saveptr);

    char* creationDateString = strtok_r(NULL,";",&saveptr);
    Date* creation_date = parse_date(creationDateString);

    char* accStatusString = strtok_r(NULL,";",&saveptr);
    AccountStatus acc_status = parse_acc_status(accStatusString);

    Driver *driver = create_driver(id, name, *date, gender, car_class, license_plate, city, *creation_date, acc_status);
}

void* parse_ride(char* line){
    char *saveptr;
    
    char* idString = strtok_r(line,";",&saveptr);
    int id = parse_int(idString);

    char* dateString = strtok_r(NULL,";",&saveptr);
    Date* date = parse_date(dateString);

    char* driverString = strtok_r(NULL,";",&saveptr);
    int driver_id = parse_int(driverString);

    char* user = strtok_r(NULL,";",&saveptr);

    char* city = strtok_r(NULL,";",&saveptr);

    char* distanceString = strtok_r(NULL,";",&saveptr);
    int distance = parse_int(distanceString);

    char* userScoreString = strtok_r(NULL,";",&saveptr);
    int user_score = parse_int(userScoreString);

    char* driverScoreString = strtok_r(NULL,";",&saveptr);
    int driver_score = parse_int(driverScoreString);

    char* tipString = strtok_r(NULL,";",&saveptr);
    double tip = parse_double(tipString);

    char* comment = strtok_r(NULL,";",&saveptr);

    Ride *ride = create_ride(id, *date, driver_id, user, city, distance, user_score, driver_score, tip);

}

void* parse_user(char* line){
    char* saveptr;

    char* username = strtok_r(line,";",&saveptr);

    char* name = strtok_r(NULL,";",&saveptr);

    char* genderString = strtok_r(NULL,";",&saveptr);
    Gender gender = parse_gender(genderString);

    char* birthDateString = strtok_r(NULL,";",&saveptr);
    Date* birth_date = parse_date(birthDateString);

    char* accCreationString = strtok_r(NULL,";",&saveptr);
    Date* acc_creation = parse_date(accCreationString);

    char* payMethodString = strtok_r(NULL,";",&saveptr);
    PaymentMethod pay_method = parse_pay_method(payMethodString);

    char* accStatusString = strtok_r(NULL,";",&saveptr);
    AccountStatus acc_status = parse_acc_status(accStatusString);

    User *user = create_user(username, name, gender, *birth_date, *acc_creation, pay_method, acc_status);

}



