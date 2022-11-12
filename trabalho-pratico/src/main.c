#include <stdio.h>
#include "catalog.h"
#include "file_util.h"
#include "parser.h"

int main(int argc, char **argv) {
    parse_driver("000000000001;Luísa Almeida;19/09/1990;F;green;EV-54-07;Setúbal;05/07/2021;active");

    return 0;
}


