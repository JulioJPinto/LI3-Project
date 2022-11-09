#include <glib.h>


#include "include/struct_util.h"
#include "include/driver.h"
#include "include/user.h"
#include "include/ride.h"
#include "include/parser.h"

#include <stdio.h>

typedef void* (*parse_line_function_pointer)(char* string);

void read_file(const char* file_path, parse_line_function_pointer parse_line_function){
    FILE* csv_file = fopen(file_path,"r");

    char static buffer[1024];
    int i = 0;
    while(fgets(buffer, 1024, csv_file)){
        parse_line_function(buffer);
        printf("%d\n",i);
        i++;
    }

}

int parse_int(char* string){
    char** endstr;
    return strtol(string,endstr, 0);

}

double parse_double(char* string){
    char** endstr;
    return strtod(string,endstr);
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

    char* idString = strtok(line,";");
    int id = parse_int(idString);

    char* name = strtok(line,";");

    char* dateString = strtok(line,";");
    Date* date = parse_date(dateString);

    char* genderString = strtok(line,";");
    Gender gender = parse_gender(genderString);

    char* carClassString = strtok(line,";");
    CarClass car_class = parse_car_class(carClassString);

    char* license_plate = strtok(line,";");
    
    char* city = strtok(line,";");

    char* creationDateString = strtok(line,";");
    Date* creation_date = parse_date(creationDateString);

    char* accStatusString = strtok(line,";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

    create_driver(id, name, *date, gender, car_class, license_plate, city, *creation_date, acc_status);

}

void* parse_ride(char* line){
    
    char* idString = strtok(line,";");
    int id = parse_int(idString);

    char* dateString = strtok(line,";");
    Date* date = parse_date(dateString);

    char* driverString = strtok(line,";");
    int driver_id = parse_int(driverString);

    char* user = strtok(line,";");

    char* city = strtok(line,";");

    char* distanceString = strtok(line,";");
    int distance = parse_int(distanceString);

    char* userScoreString = strtok(line,";");
    int user_score = parse_int(userScoreString);

    char* driverScoreString = strtok(line,";");
    int driver_score = parse_int(driverScoreString);

    char* tipString = strtok(line,";");
    double tip = parse_double(tipString);

    char* comment = strtok(line,";");

    create_ride(id,*date,driver_id,user,city,distance,user_score,driver_score,tip); 

}

void* parse_user(char* line){

    char* username = strtok(line,";");

    char* name = strtok(line,";");

    char* genderString = strtok(line,";");
    Gender gender = parse_gender(genderString);

    char* birthDateString = strtok(line,";");
    Date* birth_date = parse_date(birthDateString);

    char* accCreationString = strtok(line,";");
    Date* acc_creation = parse_date(accCreationString);

    char* payMethodString = strtok(line,";");
    PaymentMethod pay_method = parse_pay_method(payMethodString);

    char* accStatusString = strtok(line,";");
    AccountStatus acc_status = parse_acc_status(accStatusString);

    create_user(username, name, gender, *birth_date, *acc_creation, pay_method, acc_status);

}



