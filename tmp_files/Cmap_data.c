#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 데이터 헤더를 위한 구조체 정의
typedef struct
{
    int TotalDataSize;
    int OutlineDataCount;
    int MiddleLineDataCount;
    int ParkingSpaceDataCount;
    int PathDataCount;
    int PathDataSize;
    int OutlineDataSize;
    int MiddleLineDataSize;
    int ParkingSpaceDataSize;
} Header;

// 데이터 오프셋을 위한 구조체 정의
typedef struct
{
    int OutlineDataOffset;
    int MiddleLineDataOffset;
    int ParkingSpaceDataOffset;
    int PathDataOffset;
} Offset;

// 외곽선 데이터를 위한 구조체 정의
typedef struct
{
    int StartVertex[2];
    int EndVertex[2];
} Outline, MiddleLine;

// 주차 공간 데이터를 위한 구조체 정의
typedef struct
{
    char ParkingSpaceID[3];
    int BottomLeftVertex[2];
    int BottomRightVertex[2];
    int TopRightVertex[2];
    int TopLeftVertex[2];
    bool IsParkingAvailable;
    bool IsHandicappedParkingSpace;
} ParkingSpace;

// 경로 데이터를 위한 구조체 정의
typedef struct
{
    char NodeID[3];
    int StartVertex[2];
    int NodeVertex[2];
    int LinkedNodes[4];
    int LinkedIndex[4];
} PathData;

// JSON 파일을 읽고 처리하는 함수
int readAndProcessJSON(const char *jsonFileName)
{
    Header header; // Header 구조체
    Offset offset; // Offset 구조체
    FILE *file;    // 파일 포인터

    // JSON 객체 및 배열 선언
    json_object *jsonData, *outlineArray, *middleLineArray, *parkingSpaceArray, *pathArray;
    json_object *outlineObj, *middleLineObj, *parkingSpaceObj, *pathDataObj;
    json_object *nodeID, *startVertex, *endVertex, *vertex, *isParkingAvailable, *isHandicappedParkingSpace, *nodeVertex, *linkedNodes, *linkedIndex, *parkingSpaceId;
    int i;

    // JSON 파일을 읽어서 json_object를 생성
    jsonData = json_object_from_file(jsonFileName);
    if (!jsonData)
    {
        printf("Failed to read JSON file.\n");
        return -1; // JSON 파일 읽기 실패
    }

    // JSON에서 데이터 배열을 가져와서 각각의 데이터 수를 저장
    if (json_object_object_get_ex(jsonData, "outline", &outlineArray) &&
        json_object_object_get_ex(jsonData, "middleLine", &middleLineArray) &&
        json_object_object_get_ex(jsonData, "parkingSpace", &parkingSpaceArray) &&
        json_object_object_get_ex(jsonData, "PathData", &pathArray))
    {
        // 윤곽선 데이터 배열 길이 저장
        header.OutlineDataCount = json_object_array_length(outlineArray);

        // 중앙선 데이터 배열 길이 저장
        header.MiddleLineDataCount = json_object_array_length(middleLineArray);

        // 주차 공간 데이터 배열 길이 저장
        header.ParkingSpaceDataCount = json_object_array_length(parkingSpaceArray);

        // 경로 데이터 배열 길이 저장
        header.PathDataCount = json_object_array_length(pathArray);
    }
    else
    {
        // 데이터 배열을 JSON에서 가져오지 못했을 때 오류 메시지 출력
        printf("Failed to get data arrays from JSON.\n");

        // JSON 객체 메모리 해제
        json_object_put(jsonData);

        // JSON 파일 읽기 실패
        return -1;
    }

    // 바이너리 파일을 생성하고 헤더와 오프셋 정보를 기록
    file = fopen("MapData.bin", "wb");
    if (!file)
    {
        perror("Unable to create file");
        json_object_put(jsonData);
        return -1;
    }

    // Outline 데이터의 시작 위치
    offset.OutlineDataOffset = sizeof(Header) + sizeof(Offset);

    // MiddleLine 데이터의 시작 위치
    offset.MiddleLineDataOffset = offset.OutlineDataOffset + header.OutlineDataCount * sizeof(Outline);
    
     // ParkingSpace 데이터의 시작 위치
    offset.ParkingSpaceDataOffset = offset.MiddleLineDataOffset + header.MiddleLineDataCount * sizeof(MiddleLine);
    
    // PathData 데이터의 시작 위치
    offset.PathDataOffset = offset.ParkingSpaceDataOffset + header.ParkingSpaceDataCount * sizeof(ParkingSpace);

    // Outline 데이터의 총 크기
    header.OutlineDataSize = header.OutlineDataCount * sizeof(Outline);

    // MiddleLine 데이터의 총 크기
    header.MiddleLineDataSize = header.MiddleLineDataCount * sizeof(MiddleLine);

    // ParkingSpace 데이터의 총 크기
    header.ParkingSpaceDataSize = header.ParkingSpaceDataCount * sizeof(ParkingSpace);

    // PathData 데이터의 총 크기
    header.PathDataSize = header.PathDataCount * sizeof(PathData);

    // 총 데이터 크기 계산: 모든 데이터 크기와 헤더 크기를 합하여 전체 데이터 크기를 계산합니다.
    header.TotalDataSize = sizeof(Header) + sizeof(Offset) + header.OutlineDataSize +
    header.MiddleLineDataSize + header.ParkingSpaceDataSize + header.PathDataSize;

    fwrite(&header, sizeof(Header), 1, file);
    fwrite(&offset, sizeof(Offset), 1, file);

    // 외곽선 데이터를 읽어서 이진 파일에 기록
    for (i = 0; i < header.OutlineDataCount; i++)
    {
        // 'outlineArray' 배열에서 'i' 인덱스에 있는 JSON 객체를 가져옵니다.
        outlineObj = json_object_array_get_idx(outlineArray, i);

        // 'Outline' 타입의 'outline' 변수를 선언합니다.
        Outline outline;

        // "StartVertex" 필드에서 시작 데이터를 추출하고 구조체에 저장합니다.
        if (json_object_object_get_ex(outlineObj, "StartVertex", &startVertex))
        {
            outline.StartVertex[0] = json_object_get_int(json_object_array_get_idx(startVertex, 0));
            outline.StartVertex[1] = json_object_get_int(json_object_array_get_idx(startVertex, 1));
        }

        // "EndVertex" 필드에서 종료 데이터를 추출하고 구조체에 저장합니다.
        if (json_object_object_get_ex(outlineObj, "EndVertex", &endVertex))
        {
            outline.EndVertex[0] = json_object_get_int(json_object_array_get_idx(endVertex, 0));
            outline.EndVertex[1] = json_object_get_int(json_object_array_get_idx(endVertex, 1));
        }
        fwrite(&outline, sizeof(Outline), 1, file);
    }

    // 중간 선 데이터를 읽어서 이진 파일에 기록
    for (i = 0; i < header.MiddleLineDataCount; i++)
    {
        // 'middleLineArray'에서 i번째 JSON 객체를 가져옵니다.
        middleLineObj = json_object_array_get_idx(middleLineArray, i);

        // 'MiddleLine' 타입의 변수 'middleLine'을 선언합니다.
        MiddleLine middleLine;

        // 'middleLineObj'에서 "StartVertex" 키를 가진 객체를 찾아 'startVertex'에 저장합니다.
        if (json_object_object_get_ex(middleLineObj, "StartVertex", &startVertex))
        {
            // 'startVertex' 배열의 첫 번째 요소를 정수로 변환하여 'middleLine.StartVertex[0]'에 저장합니다.
            middleLine.StartVertex[0] = json_object_get_int(json_object_array_get_idx(startVertex, 0));
            // 'startVertex' 배열의 두 번째 요소를 정수로 변환하여 'middleLine.StartVertex[1]'에 저장합니다.
            middleLine.StartVertex[1] = json_object_get_int(json_object_array_get_idx(startVertex, 1));
        }

        // 'middleLineObj'에서 "EndVertex" 키를 가진 객체를 찾아 'endVertex'에 저장합니다.
        if (json_object_object_get_ex(middleLineObj, "EndVertex", &endVertex))
        {
            // 'endVertex' 배열의 첫 번째 요소를 정수로 변환하여 'middleLine.EndVertex[0]'에 저장합니다.
            middleLine.EndVertex[0] = json_object_get_int(json_object_array_get_idx(endVertex, 0));
            // 'endVertex' 배열의 두 번째 요소를 정수로 변환하여 'middleLine.EndVertex[1]'에 저장합니다.
            middleLine.EndVertex[1] = json_object_get_int(json_object_array_get_idx(endVertex, 1));
        }
        fwrite(&middleLine, sizeof(MiddleLine), 1, file);
    }

    // 주차 공간 데이터를 읽어서 이진 파일에 기록
    for (i = 0; i < header.ParkingSpaceDataCount; i++)
    {
        // JSON 배열에서 주어진 인덱스(i)에 해당하는 주차 공간 객체를 가져옵니다.
        parkingSpaceObj = json_object_array_get_idx(parkingSpaceArray, i);

        // ParkingSpace 구조체를 생성합니다.
        ParkingSpace parkingSpace;

        // "ParkingSpaceID" 필드를 추출하고 ParkingSpaceID 멤버에 저장합니다.
        if (json_object_object_get_ex(parkingSpaceObj, "ParkingSpaceID", &nodeID))
        {
            // JSON 문자열을 가져와서 ParkingSpace 구조체의 ParkingSpaceID 멤버에 최대 3글자까지 복사합니다.
            strncpy(parkingSpace.ParkingSpaceID, json_object_get_string(nodeID), 3);
        }

        // "BottomLeftVertex" 필드를 추출하고 BottomLeftVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(parkingSpaceObj, "BottomLeftVertex", &vertex))
        {
            // 'vertex' 배열의 첫 번째 요소를 정수로 변환하여 ' parkingSpace.BottomLeftVertex[0]' 저장합니다.
            parkingSpace.BottomLeftVertex[0] = json_object_get_int(json_object_array_get_idx(vertex, 0));
            // 'vertex' 배열의 두 번째 요소를 정수로 변환하여 ' parkingSpace.BottomLeftVertex[1]' 저장합니다.
            parkingSpace.BottomLeftVertex[1] = json_object_get_int(json_object_array_get_idx(vertex, 1));
        }

        // "BottomRightVertex" 필드를 추출하고 BottomRightVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(parkingSpaceObj, "BottomRightVertex", &vertex))
        {
            // 'vertex' 배열의 첫 번째 요소를 정수로 변환하여 'parkingSpace.BottomRightVertex[0]' 저장합니다.
            parkingSpace.BottomRightVertex[0] = json_object_get_int(json_object_array_get_idx(vertex, 0));
            // 'vertex' 배열의 두 번째 요소를 정수로 변환하여 'parkingSpace.BottomRightVertex[1]' 저장합니다.
            parkingSpace.BottomRightVertex[1] = json_object_get_int(json_object_array_get_idx(vertex, 1));
        }

        // "TopRightVertex" 필드를 추출하고 TopRightVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(parkingSpaceObj, "TopRightVertex", &vertex))
        {
            // 'vertex' 배열의 첫 번째 요소를 정수로 변환하여 'parkingSpace.TopRightVertex[0]' 저장합니다.
            parkingSpace.TopRightVertex[0] = json_object_get_int(json_object_array_get_idx(vertex, 0));
             // 'vertex' 배열의 두 번째 요소를 정수로 변환하여 'parkingSpace.TopRightVertex[1]' 저장합니다.
            parkingSpace.TopRightVertex[1] = json_object_get_int(json_object_array_get_idx(vertex, 1));
        }

        // "TopLeftVertex" 필드를 추출하고 TopLeftVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(parkingSpaceObj, "TopLeftVertex", &vertex))
        {
            // 'vertex' 배열의 첫 번째 요소를 정수로 변환하여 'parkingSpace.TopLeftVertex[0]' 저장합니다.
            parkingSpace.TopLeftVertex[0] = json_object_get_int(json_object_array_get_idx(vertex, 0));
            // 'vertex' 배열의 두 번째 요소를 정수로 변환하여 'parkingSpace.TopLeftVertex[1]' 저장합니다.
            parkingSpace.TopLeftVertex[1] = json_object_get_int(json_object_array_get_idx(vertex, 1));
        }

        // 주차 공간 ID에 따른 처리
        int parkingSpaceID = atoi(parkingSpace.ParkingSpaceID);
        // 21번과 22번 주차 공간에 대한 조건
        if (parkingSpaceID == 21 || parkingSpaceID == 22)
        {
             // "IsHandicappedParkingSpace" 필드를 추출하고 해당 값을 ParkingSpace 구조체의 IsHandicappedParkingSpace 멤버에 저장합니다.            
            if (json_object_object_get_ex(parkingSpaceObj, "IsHandicappedParkingSpace", &isHandicappedParkingSpace))
            {
                parkingSpace.IsHandicappedParkingSpace = json_object_get_boolean(isHandicappedParkingSpace);
            }
            // IsParkingAvailable 멤버를 true로 설정합니다.
            parkingSpace.IsParkingAvailable = true;
        }
        else
        {
            // 나머지 주차 공간 처리
            // "IsParkingAvailable" 필드를 추출하고 해당 값을 ParkingSpace 구조체의 IsParkingAvailable 멤버에 저장
            if (json_object_object_get_ex(parkingSpaceObj, "IsParkingAvailable", &isParkingAvailable))
            {
                // JSON 변수 `isHandicappedParkingSpace`에서 부울 값을 추출하여 `parkingSpace.IsHandicappedParkingSpace` 변수에 할당합니다.
                parkingSpace.IsParkingAvailable = json_object_get_boolean(isParkingAvailable);
            }
            // IsHandicappedParkingSpace 멤버를 false로 설정합니다.
            parkingSpace.IsHandicappedParkingSpace = false;
        }
        fwrite(&parkingSpace, sizeof(ParkingSpace), 1, file);
    }

    // 경로 데이터를 읽어서 이진 파일에 기록
    for (int i = 0; i < header.PathDataCount; i++)
    {
        // JSON 배열에서 주어진 인덱스(i)에 해당하는 경로 데이터 객체를 가져옵니다.
        pathDataObj = json_object_array_get_idx(pathArray, i);
        // PathData 구조체를 생성합니다.
        PathData pathData;

        // "NodeID" 필드를 추출하고 NodeID 멤버에 저장 (최대 4글자까지 복사)
        if (json_object_object_get_ex(pathDataObj, "NodeID", &nodeID))
        {
            // JSON 변수 `nodeID`에서 최대 4개의 문자를 복사하여 `pathData.NodeID` 문자열에 저장합니다.
            strncpy(pathData.NodeID, json_object_get_string(nodeID), 4);
        }

        // "StartVertex" 필드를 추출하고 StartVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(pathDataObj, "StartVertex", &startVertex))
        {
            // 'startVertex' 배열의 첫 번째 요소를 정수로 변환하여 'pathData.StartVertex[0]' 저장합니다.
            pathData.StartVertex[0] = json_object_get_int(json_object_array_get_idx(startVertex, 0));
            // 'startVertex' 배열의 두 번째 요소를 정수로 변환하여 'pathData.StartVertex[1]' 저장합니다.
            pathData.StartVertex[1] = json_object_get_int(json_object_array_get_idx(startVertex, 1));
        }

        // "NodeVertex" 필드를 추출하고 NodeVertex 멤버에 저장합니다.
        if (json_object_object_get_ex(pathDataObj, "NodeVertex", &nodeVertex))
        {
            // 'nodeVertex' 배열의 첫 번째 요소를 정수로 변환하여 'pathData.NodeVertex[0]' 저장합니다.
            pathData.NodeVertex[0] = json_object_get_int(json_object_array_get_idx(nodeVertex, 0));
            // 'nodeVertex' 배열의 두 번째 요소를 정수로 변환하여 'pathData.NodeVertex[1]' 저장합니다.
            pathData.NodeVertex[1] = json_object_get_int(json_object_array_get_idx(nodeVertex, 1));
        }

        // "LinkedNodes" 필드를 추출하고 이에 대한 처리를 추가합니다.
        if (json_object_object_get_ex(pathDataObj, "LinkedNodes", &linkedNodes))
        {
            for (int j = 0; j < 4; j++)
            {
                // JSON 배열 `linkedNodes`에서 `j` 인덱스에 해당하는 요소를 가져와서 해당 요소가 존재하는지 검사합니다.
                if (json_object_array_get_idx(linkedNodes, j))
                {
                  // JSON 배열의 j번째 원소를 정수로 변환하여 PathData.LinkedNodes 배열에 저장
                  pathData.LinkedNodes[j] = json_object_get_int(json_object_array_get_idx(linkedNodes, j));
                }
                else
                {
                  // 값이 존재하지 않는 경우, -1을 PathData.LinkedNodes 배열에 저장
                  pathData.LinkedNodes[j] = -1;
                }
            }
        }
        // pathDataObj에서 "LinkedIndex" 키의 값을 가져와서 linkedIndex 저장합니다.
        if (json_object_object_get_ex(pathDataObj, "LinkedIndex", &linkedIndex))
        {
            for (int j = 0; j < 4; j++)
            {
              // JSON 배열 `linkedIndex`에서 `j` 인덱스에 해당하는 요소를 가져와서 해당 요소가 존재하는지 검사합니다.
                if (json_object_array_get_idx(linkedIndex, j))
                {
                  // JSON 배열의 j번째 원소를 정수로 변환하여 PathData.LinkedIndex 배열에 저장합니다.
                  pathData.LinkedIndex[j] = json_object_get_int(json_object_array_get_idx(linkedIndex, j));
                }
                else
                { 
                  // 값이 존재하지 않는 경우, -1을 PathData.LinkedIndex 배열에 저장합니다.
                  pathData.LinkedIndex[j] = -1;
                }
            }
        }
        fwrite(&pathData, sizeof(PathData), 1, file);
    }

    // 파일 닫고 메모리 해제
    fclose(file);
    json_object_put(jsonData);

    // return 0;
}

int main()
{
    // JSON 파일 이름을 상수 문자열로 정의합니다.
    const char *jsonFileName = "MapData.json";

    // readAndProcessJSON 함수를 호출하여 JSON 파일을 읽고 처리합니다.
    return readAndProcessJSON(jsonFileName);
}