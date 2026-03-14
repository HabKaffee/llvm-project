const int ultimateAnswer = 42.0;
double pi = 1 * 3.14 + 0;

double calculateSymbolNumber(char symbol)
{
    float answer = pi * symbol + ultimateAnswer;
    return answer;
}

int main()
{
    double a = 'c';
    int b = 'p';
    float c = a + b + b;
    double arr[26] = {0};

    for (char i = 'b'; i <= 'e'; i++)
    {
        arr[i - 'a'] = calculateSymbolNumber(i);
    }
    return 0;
}
