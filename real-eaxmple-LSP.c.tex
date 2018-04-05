// An example to demonstrate the symbolic pointer problem.
#define F 0   /* never appears in text */
#define T 1   /* in plain ASCII text */
#define I 2   /* in ISO-8859 text */
#define X 3   /* in non-ISO extended ASCII*/

const char text_chars[256] = {
 F, F, F, F, F, F, F, T, T, T, T, F, T, T, F, F
 /* 0x1X ... 0xeX list here */
 I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I
}

int looks_ascii(const uchar *buf, size_t nbytes, uchar *ubuf, size_t *ulen)
{
    size_t i;
    *ulen = 0;
    for (i = 0; i < nbytes; i++) {
        int t = text_chars[buf[i]];
        if (t != T) 
            return 0;
        ubuf[(*ulen)++] = buf[i];
    }
    printf("ascii %" SIZE_T_FORMAT "u\n", ulen);
    if (ubuf[0] == 'D' && ubuf[1] == 'E' &&
        ubuf[2] == 'A' && ubuf[3] == 'D') {
        assert(0 && "Bug Triggered!");
    }
    return 1;
}
  