/* An example for symbolic loop */
#define END_DESCRIPTOR 0xFF

int verify_file(char* path)
{
    FILE* file = fopen(path, "r");
    if (file == NULL)
        exit(-1);
    uint32_t length;
    if (!fread(&length, sizeof(uint32_t), 1, file)) {
        fclose(file);
        exit(-1);
    }
    // Verify the format by checking the end descriptor.
    while (--length) {
        if (read_1_byte(file) == END_DESCRIPTOR) {
             printf("Wrong Format!");
             return 0;
        }
    }
    char end = read_1_byte(file);
    fclose(file);
    return (end == END_DESCRIPTOR);
}