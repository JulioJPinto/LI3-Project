/**
 * Test if the semicolon file output writer is working properly.
 */
void test_semicolon_file_output_writer(void) {
    FILE *file = tmpfile();

    OutputWriter *writer = create_semicolon_file_output_writer(file);
    writer_write_output_token(writer, "Hello");
    writer_write_output_token_end(writer, "World");
    close_output_writer(writer);

    rewind(file);
    char buffer[100];
    fgets(buffer, 100, file);
    g_assert_cmpstr("Hello;World\n", ==, buffer);

    fclose(file);
}

/**
 * Test if the array of semicolon strings output writer is working properly.
 */
void test_array_of_semicolon_strings_output_writer(void) {
    GPtrArray *array = g_ptr_array_new();

    OutputWriter *writer = create_array_of_semicolon_strings_output_writer(array);
    writer_write_output_token(writer, "Hello");
    writer_write_output_token_end(writer, "World");
    close_output_writer(writer);

    g_assert_cmpstr("Hello;World\n", ==, g_ptr_array_index(array, 0));
}
