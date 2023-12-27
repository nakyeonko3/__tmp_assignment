// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct
// {
//     int TotalDataSize;
//     int UserDataCount;
//     int ParkingSpaceDataCount;
//     int UserDataSize;
//     int ParkingSpaceSize;
// } Header;

// typedef struct
// {
//     int UserDataOffset;
//     int ParkingSpaceDataOffset;
// } Offset;

// typedef struct
// {
//     char Name[13];
//     char CarType[20];
//     char CarNumber[12];
//     int ParkingSpace;
// } UserData;

// typedef struct
// {
//     int ParkingSpace;
// } ParkingSpace;

// // 사용자 데이터 읽기 함수
// void readUserData(FILE *fp) {
//     // 헤더 정보 읽기
//     Header header;
//     Offset offset;

//     fseek(fp, 0, SEEK_SET);
//     fread(&header, sizeof(Header), 1, fp);
//     fread(&offset, sizeof(Offset), 1, fp);

//     printf("Total Data Size: %d\n", header.TotalDataSize);
//     printf("User Data Count: %d\n", header.UserDataCount);
//     printf("Parking Space Data Count: %d\n", header.ParkingSpaceDataCount);
//     printf("User Data Size: %d\n", header.UserDataSize);
//     printf("Parking Space Size: %d\n", header.ParkingSpaceSize);

//     // 사용자 데이터 읽기
//     printf("\nUser Data\n");
//     printf("--------------------------------------------------------------\n");
//     printf("|   Name   |      Car Type      | Car Number | Parking Space |\n");
//     printf("--------------------------------------------------------------\n");

//     UserData userData;

//     fseek(fp, offset.UserDataOffset, SEEK_SET);

//     for (int i = 0; i < header.UserDataCount; i++) {
//         fread(&userData, sizeof(UserData), 1, fp);
//         printf("|%-13s|%-20s|%-12s|%12d|\n", userData.Name, userData.CarType, userData.CarNumber, userData.ParkingSpace);
//     }

//     printf("--------------------------------------------------------------\n");
// }

// int main() {
//     FILE *fp = fopen("UserData.bin", "rb");
//     if (fp == NULL) {
//         perror("파일 열기 실패");
//         return -1;
//     }

//     readUserData(fp);
//     fclose(fp);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 헤더를 위한 구조체 정의 (위에서 정의한 것과 동일)
typedef struct
{
    int TotalDataSize;
    int UserDataCount;
    int SelectParkingSpaceDataCount;
    int UserDataSize;
    int SelectParkingSpaceSize;
} Header;

// 데이터 오프셋을 위한 구조체 정의 (위에서 정의한 것과 동일)
typedef struct
{
    int UserDataOffset;
    int SelectParkingSpaceDataOffset;
} Offset;

// 사용자 데이터 구조체 정의 (위에서 정의한 것과 동일)
typedef struct
{
    char Name[13];
    char CarType[20];
    char CarNumber[12];
} UserData;

// 선택한 주차 공간 데이터 구조체 정의 (위에서 정의한 것과 동일)
typedef struct
{
    int SelectParkingSpace;
} SelectParkingSpace;

// 바이너리 파일에서 사용자 데이터 읽기 함수
void readUserData()
{
    FILE *fp = fopen("./UserData.bin", "rb");
    if (fp == NULL)
    {
        perror("Failed to open file");
        return;
    }

    UserData userData;

    // 파일에서 사용자 데이터 읽기
    if (fread(&userData, sizeof(UserData), 1, fp) != 1)
    {
        perror("Failed to read user data");
        fclose(fp);
        return;
    }

    // 읽은 데이터 출력
    printf("Name: %s\n", userData.Name);
    printf("Car Type: %s\n", userData.CarType);
    printf("Car Number: %s\n", userData.CarNumber);

    fclose(fp);
}

// 메인 함수
int main()
{
    readUserData();
    return 0;
}
