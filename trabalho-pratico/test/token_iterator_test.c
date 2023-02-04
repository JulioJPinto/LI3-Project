#include <glib.h>

/**
 * Tests token iterator (semicolon separated) functionality.
 */
void test_semicolon_separated_token_iterator(void) {
    TokenIterator *iterator = init_semicolon_separated_token_iterator();
    char target[] = "a;b;c";
    token_iterator_set_current(iterator, target);

    g_assert_cmpstr("a", ==, token_iterator_next(iterator));
    g_assert_cmpstr("b", ==, token_iterator_next(iterator));
    g_assert_cmpstr("c", ==, token_iterator_next(iterator));
    g_assert_cmpstr("\0", ==, token_iterator_next(iterator));

    token_iterator_free(iterator);
}
