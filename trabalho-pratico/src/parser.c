#include <glib.h>

#include "parser.h"
#include "ride.h"
#include <stdio.h>

typedef void* (*parse_line_function_pointer)(char* string);

void read_file(const char* file_path, parse_line_function_pointer parse_line_function){
    FILE *csv_file = fopen(file_path,"r");

    char buffer[10000];
    //int i = 0;
    while(fgets(buffer, 10000, csv_file)){
        parse_line_function(buffer);
        //i++;
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

    date_parsed->day = parse_int(strtok(string,"/")); 
    date_parsed->month = parse_int(strtok(string,"/"));
    date_parsed->year = parse_int(strtok(string,"/"));

    return date_parsed;
}

Gender parse_gender(char* string){

    if (strcmp(string, "F")){
        return F;
    } else {
        return M;
    }

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

AccountStatus parse_acc_status(char* string){

    if (strcmp(string, "active")){
        return ACTIVE;
    }
    
    return INACTIVE;
    
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

    char* idString = strsep(line,";"); 
    int id = parse_int(idString);

    char* name = strsep(line,";");

    char* dateString = strsep(line,";");
    Date* date = parse_date(dateString);

    char* genderString = strsep(line,";");
    Gender gender = parse_gender(genderString);

    char* carClassString = strsep(line,";");
    CarClass car_class = parse_car_class(carClassString);

    char* license_plate = strsep(line,";");
    
    char* city = strsep(line,";");

    char* creationDateString = strsep(line,";");
    Date* creation_date = parse_date(creationDateString);

    char* accStatusString = strsep(line,";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

}

void* parse_ride(char* line){
    
    char* idString = strtok(line,";");
    int id = parse_int(idString);

    char* dateString = strtok(NULL,";");
    Date* date = parse_date(dateString);

    char* driverString = strtok(NULL,";");
    int driver_id = parse_int(driverString);

    char* user = strtok(NULL,";");

    char* city = strtok(NULL,";");

    char* distanceString = strtok(NULL,";");
    int distance = parse_int(distanceString);

    char* userScoreString = strtok(NULL,";");
    int user_score = parse_int(userScoreString);

    char* driverScoreString = strtok(NULL,";");
    int driver_score = parse_int(driverScoreString);

    char* tipString = strtok(NULL,";");
    double tip = parse_double(tipString);

    char* comment = strtok(NULL,";");

    Ride *ride = create_ride(id,*date,driver_id,user,city,distance,user_score,driver_score,tip); 

}

void* parse_user(char* line){

    char* username = strtok(line,";");

    char* name = strtok(NULL,";");

    char* genderString = strtok(NULL,";");
    Gender gender = parse_gender(genderString);

    char* birthDateString = strtok(NULL,";");
    Date* birth_date = parse_date(birthDateString);

    char* accCreationString = strtok(NULL,";");
    Date* acc_creation = parse_date(accCreationString);

    char* payMethodString = strtok(NULL,";");
    PaymentMethod pay_method = parse_pay_method(payMethodString);

    char* accStatusString = strtok(NULL,";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

    User *user = create_user(username, name, gender, *birth_date, *acc_creation, pay_method, acc_status);

}



