#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 헤더를 위한 구조체 정의.
typedef struct
{
  int TotalDataSize;               // 전체 데이터 사이즈
  int UserDataCount;               // 사용자 데이터 수
  int SelectParkingSpaceDataCount; // 선택한 주차 공간 데이터 수
  int UserDataSize;                // 사용자 데이터 사이즈
  int ParkingSpaceSize;            // 선택한 주차 공간 사이즈
} Header;

// 데이터 오프셋을 위한 구조체 정의.
typedef struct
{
  int UserDataOffset;         // 사용자 데이터 오프셋
  int ParkingSpaceDataOffset; // 선택한 주차 공간 데이터 오프셋
} Offset;

// 사용자 데이터 구조체 정의.
typedef struct
{
  char Name[13];      // 이름
  char CarType[20];   // 차량 유형
  char CarNumber[12]; // 차량 번호
  int ParkingSpace;   // 선택한 주차 공간
} UserData;

// 선택한 주차 공간 데이터 구조체 정의.
typedef struct
{
  int SelectParkingSpace; // 선택한 주차 공간 번호
} SelectParkingSpace;

// 사용자 데이터 쓰기 함수.
void writeUserData(const char *name, const char *carType, const char *carNumber, int parkingSpace, FILE *fp)
{
  // 헤더 및 오프셋.
  Header header;
  Offset offset;

  // 사용자 데이터 사이즈 계산.
  header.UserDataSize = sizeof(UserData);
  header.TotalDataSize = sizeof(Header) + header.UserDataSize; // 전체 데이터 사이즈 업데이트.
  header.UserDataCount = 1;                                    // 하나의 사용자 데이터만 있다고 가정.

  // 오프셋 계산.
  offset.UserDataOffset = sizeof(Header);

  // 파일에 헤더 데이터 구조체 쓰기.
  fwrite(&header, sizeof(Header), 1, fp);
  fwrite(&offset, sizeof(Offset), 1, fp);

  // 제공된 데이터로 사용자 데이터 구조체 채우기.
  UserData userData;
  strcpy(userData.Name, name);
  strcpy(userData.CarType, carType);
  strcpy(userData.CarNumber, carNumber);
  userData.ParkingSpace = parkingSpace;

  // 파일에 사용자 데이터 구조체 쓰기.
  fwrite(&userData, sizeof(UserData), 1, fp);

  printf("UserData.bin에 데이터가 쓰여졌습니다\n");
}

// 메인 함수.
int main(int argc, char *argv[])
{
  // 올바른 수의 인자가 전달되었는지 확인.
  if (argc < 5)
  {
    printf("사용자 데이터: %s 이름 차종 차량번호 주차공간\n", argv[0]);
    return -1;
  }

  FILE *fp = fopen("UserData.bin", "wb");
  if (fp == NULL)
  {
    perror("파일 열기 실패");
    return -1;
  }

  int parkingSpace = atoi(argv[4]);

  // 사용자 데이터 쓰기 함수 호출.
  writeUserData(argv[1], argv[2], argv[3], parkingSpace, fp);

  fclose(fp);

  return 0;
}
