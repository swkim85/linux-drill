// pattern.hip
#include <stdio.h>
#include <string.h>

// KMP 알고리즘을 위한 접두사 테이블 생성 함수
void computeLPSArray(const char* pattern, int M, int* lps) {
    int length = 0;
    lps[0] = 0;
    int i = 1;

    while (i < M) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP 알고리즘을 사용하여 텍스트에서 패턴 검색
void KMPSearch(const char* pattern, const char* text) {
    int M = strlen(pattern);
    int N = strlen(text);

    int lps[M];
    computeLPSArray(pattern, M, lps);

    int i = 0; // text의 인덱스
    int j = 0; // pattern의 인덱스
    while (i < N) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        if (j == M) {
            printf("패턴이 인덱스 %d에서 발견되었습니다.\n", i - j);
            j = lps[j - 1];
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

int main() {
    const char* text = "ABABDABACDABABCABAB";
    const char* pattern = "ABABCABAB";

    printf("텍스트: %s\n", text);
    printf("패턴: %s\n", pattern);

    KMPSearch(pattern, text);

    return 0;
}
