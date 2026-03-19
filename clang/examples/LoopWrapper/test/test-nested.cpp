// Nested loops - each should get its own loop_start/loop_end pair

void loop_start();
void loop_end();

void bar() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // inner loop
        }
    }
}
