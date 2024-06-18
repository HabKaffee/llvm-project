
int fooFor() {
  int C = 0;
  for (int I = 0; I < 10; ++I) {
    C++;
  }
  return C;
}

int fooForEach() {
  int Arr[5] = {1, 2, 3, 4, 5};
  int Sum = 0;
  for (int &El : Arr) {
    Sum += El;
  }
  return Sum;
}

int fooWhile() {
  int N = 0;
  while (N < 10) {
    N++;
  }
  return N;
}

int fooDoWhile() {
  int N = 0;
  do {
    N++;
  } while (N < 10);
  return N;
}

int main() {

  fooFor();
  fooForEach();
  fooWhile();
  fooDoWhile();
}
