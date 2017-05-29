typedef struct Header {
    uint16_t magic;     // magic number
    uint32_t checksum;  // pseudo checksum
    uint32_t elements;  // number of chunks
} header_t;

int main(int argc, char* argv[]) {
    header_t *header = read_header();
    if (!header) {
        printf("Cannot read header!\n");
        return -1;
    }
    if (header->magic != 0xFFD8) {
        printf("Bad magic number!\n");
        return 1;
    }
    if (header->checksum != bswap_32(header->checksum)) {
        printf("Checksum incorrect!\n");
        return 1;
    }
    path_explosion(); // path explosion here
    if (header->flag == CRASH) {
        abort(); // bug here
    }
    printf("Safe!\n");
    return 0;
}

