/* An example for symbolic loop */
int verify_packet(PACKET* packet)
{
    int length = getLength(packet);
    // Checking the end descriptor.
    while (--length) {
        if (read_byte(packet) == 0xFF) {
             return 0;
        }
    }
    char end = read_byte(packet);
    return (int)(end == 0xFF);
}