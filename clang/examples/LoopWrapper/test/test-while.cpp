void loop_start();
void loop_end();

int baz() {
    int i = 0;
    while (i < 10) {
        i++;
    }

    do {
        i--;
    } while (i > 0);

    return i;
}