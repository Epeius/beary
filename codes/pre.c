int check(int magic, int checksum, int flag) {
    if (magic != 0xFFD8) {
        perror("Bad magic number!\n");
    }
    if (checksum != bswap_32(checksum)) {
        perror("Checksum incorrect!\n");
    }
    state_explosion(); // state explosion here
    if (flag == CRASH) {
        abort(); // bug here
    }
    printf("Safe!\n");
    return 0;
}

