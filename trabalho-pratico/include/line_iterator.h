#pragma once
#ifndef LI3_LINE_ITERATOR_H
#define LI3_LINE_ITERATOR_H

typedef struct LineIterator LineIterator;

LineIterator *init_csv_line_iterator(char *string);

char *line_iterator_current(LineIterator *iterator);

char *line_iterator_next(LineIterator *iterator);

void line_iterator_free(LineIterator *iterator);

#endif //LI3_LINE_ITERATOR_H
