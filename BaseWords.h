#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
#include <Windows.h>

//Расстояние Левенштейна
int levenshtein(const char *s, int ls, const char *t, int lt)
{
        int a, b, c;

        if (!ls) return lt;
        if (!lt) return ls;

        if (s[ls - 1] == t[lt - 1])
                return levenshtein(s, ls - 1, t, lt - 1);

        a = levenshtein(s, ls - 1, t, lt - 1);
        b = levenshtein(s, ls,     t, lt - 1);
        c = levenshtein(s, ls - 1, t, lt    );

        if (a > b) a = b;
        if (a > c) a = c;

        return a + 1;
}

//Установка длины базы данных
int SetLenMas(char *Track, int len)
{
        long long lenArr = 0;
        FILE *RTextFile;
        RTextFile = fopen(Track, "r");

        char temp[len];
        while (fscanf(RTextFile, "%s", &temp[0]) != EOF) lenArr++;

        fclose(RTextFile);
        return lenArr;
};

//Установка максимальной длины слова в базе данных
int SetLenWords(char *Track)
{
        long long len = 0;
        FILE *RTextFile;
        RTextFile = fopen(Track, "r");

        char temp[256];
        while (fscanf(RTextFile, "%s", &temp[0]) != EOF)
        if (strlen(temp) > len) {
            len = strlen(temp);
        };

        fclose(RTextFile);
        return len;
};

// T9
long long SearchT9(std::vector<std::vector<char>> *Arr, int lenArr, int len ,char Slovo[len])
{
        bool Flag = 0;
        int lenWord = strlen(Slovo);
        bool exFlag = 0;
        int Right = lenArr;
        int Left = 0;
        long long Run = (Right + Left) / 2;
        do
        {
            if ((Run == lenArr) || (Run < 1) || (abs(Left - Right) <= 1)) exFlag = 1;
            if (strncmp(&(*Arr)[Run][0], Slovo, lenWord) == 0) Flag = 1;

            if (!Flag)
            {
              if (strncmp(&(*Arr)[Run][0], Slovo, lenWord) > 0) Right = Run;
              else Left = Run;
            };

            Run = (Right + Left) / 2;
        }
        while ((exFlag == 0) && (Flag == 0));

	return Run;
};

//Поиск слов по базе данных
int Search(std::vector<std::vector<char>> *Arr, long long lenArr, int len ,char Slovo[len])
{
        bool Flag = 0;
        bool exFlag = 0;
        long long Right = lenArr;
        long long Left = 0;
        long long Run = (Right + Left) / 2;
        do
        {
            if ((Run == lenArr) || (Run < 1) || (abs(Left - Right) <= 1)) exFlag = 1;
            if (strcmp(&(*Arr)[Run][0], Slovo) == 0) Flag = 1;

            if (!Flag)
            {
              if (strcmp(&(*Arr)[Run][0], Slovo) > 0) Right = Run;
              else Left = Run;
            };

            Run = (Right + Left) / 2;
        }
        while ((exFlag == 0) && (Flag == 0));

	return Flag;
};

//Сортировка базы данных
void Sort(std::vector<std::vector<char>> *Arr, long long lenArr, long long len)
{
        long long d = lenArr / 2;
        char temp[len];
        while (d > 0)
        {
            for (int i = d; i < lenArr; i++)
            {
                long long j = i;
                strcpy(temp, &(*Arr)[i][0]);
                while ((j >= d) && (strcmp(temp, &(*Arr)[j - d][0]) < 0))
                {
                    strcpy(&(*Arr)[j][0], &(*Arr)[j - d][0]);
                    j = j - d;
                };
                strcpy(&(*Arr)[j][0], temp);
            };
            d = d / 2;
        };
};

//Включение в базу данных редактора
void IncludeNewBase(char *Track, std::vector<std::vector<char>> *Arr, long long len)
{
        FILE *TextFile;
	TextFile = fopen(Track, "w");
        fprintf(TextFile, "%s", &(*Arr)[0][0]);
        for (long long i = 1; i < len; i++)
        {
            fprintf(TextFile, "\n%s", &(*Arr)[i][0]);
        };
        fclose(TextFile);
};

//Запись новой базы
void WriteNewBase(std::vector<std::vector<char>> *Arr, long long *len,
		    long long *lenArr, char *link, char *mainLink)
{
        long long currLen = SetLenWords(link);
        long long currLenArr = SetLenMas(link, currLen);
        if (currLen > (*len)) (*len) = currLen;
        (*lenArr) += currLenArr;
        (*Arr).resize((*lenArr), std::vector<char>((*len) + 1));
        FILE *f;
        f = fopen(link, "r");

        long long pos = (*lenArr) - currLenArr;
        while (fscanf(f, "%s", &(*Arr)[pos][0]) != EOF) pos++;
        fclose(f);
        Sort(Arr, *lenArr, *len);
        IncludeNewBase(mainLink, Arr, *lenArr);
}

// Заменить возможно искомым словом
void ReplaceError(std::vector<std::vector<char>> *Arr, int lenArr, int len,
    char Slovo[len], int *Left, int *Right, std::vector<std::vector<char>> *Replace,
    int lenArrWords)
{

        len = strlen(Slovo);
	bool SearchFlag = false;
        int Pos = 0;

        while (Pos < len)
        {
            int prevRight = (*Right);
            int prevLeft = (*Left);
            bool Flag = 0;
            bool exFlag = 0;
            long long Run = ((*Right) + (*Left)) / 2;
            do
            {
                if ((Run == lenArr) || (Run < 1) || (abs((*Left) - (*Right)) <= 1)) exFlag = 1;
                if ((*Arr)[Run][Pos] == Slovo[Pos]) Flag = 1;

                if (!Flag)
                {
                  if ((*Arr)[Run][Pos] > Slovo[Pos]) (*Right) = Run;
                  else (*Left) = Run;
                };

                Run = ((*Right) + (*Left)) / 2;
            }
            while ((exFlag == 0) && (Flag == 0));

            if (!Flag) {
                (*Left) = prevLeft;
                (*Right) = prevRight;
            }
            else
            {
                int step = 1000;
                (*Left) = Run;
                (*Right) = Run;

                while (step > 0)
                {
                    if ((*Left) - step < 1) Run = 0;
                    if (((*Left) - step >= prevLeft) && ((*Arr)[(*Left) - step][Pos] == Slovo[Pos]))
                    	(*Left) -= step;
                    else
                    	step /= 2;
                };

                step = 1000;
                while (step > 0)
                {
                    if ((*Right) + step >= lenArr) Run = 0;
                    if (((*Right) + step <= prevRight) && ((*Arr)[(*Right) + step][Pos] == Slovo[Pos]))
                    	(*Right) += step;
                    else
                    	step /= 2;
                };
            };

            if ((*Right) == (*Left)) {
                (*Left) = prevLeft;
                (*Right) = prevRight;
            }

            if ((*Right) - (*Left) < 1000) break;

            Pos++;
        };

        if ((*Right) - (*Left) > 1000) (*Right) = (*Left) + 999;
        int lenReplace = (*Right) - (*Left);
        int Mas[1000][2];

        for (Pos = (*Left); Pos <= (*Right); Pos++)
        {
                Mas[Pos - (*Left)][0] = Pos;
                Mas[Pos - (*Left)][1] = levenshtein(Slovo, len, &(*Arr)[Pos][0], strlen(&(*Arr)[Pos][0]));
        };

        int step = lenReplace / 2;
        int temp[2];
        while (step > 0)
        {
            for (int i = step; i < lenReplace; i++)
            {
                int j = i;
                temp[0] = Mas[i][0];
                temp[1] = Mas[i][1];
                while ((j >= step) && (temp[1] < Mas[j-step][1]))
                {
                    Mas[j][0] = Mas[j - step][0];
                    Mas[j][1] = Mas[j - step][1];
                    j = j - step;
                };
                Mas[j][0] = temp[0];
                Mas[j][1] = temp[1];
            };
            step = step / 2;
        };

        (*Replace).resize(lenReplace, std::vector<char>(lenArrWords + 1));
        (*Left) = 0;
        (*Right) = lenReplace;
        for (Pos = 0; Pos < lenReplace; Pos++)
        {
                strcpy(&(*Replace)[Pos][0], &(*Arr)[Mas[Pos][0]][0]);
        };
}

