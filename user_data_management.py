import pickle # 원하는 데이터를 자료형의 변경없이 파일로 저장하거나 그대로 로드할 수 있습니다.

USERDATABIN_FILE_PATH = "./UserData.bin"
PARKINGDATABIN_FILE_PATH ="./ParkingSpaceData.bin"


class UserDataManagement:
    def __init__(self, user_data_file = USERDATABIN_FILE_PATH, parking_space_file =PARKINGDATABIN_FILE_PATH):
        self.user_data_file = user_data_file
        self.parking_space_file = parking_space_file

        # 사용자 데이터와 주차 공간 데이터를 따로 저장
        self.user_data = {}
        self.parking_space_data = {}

    def sync_data_(self):
       with open(self.user_data_file, "rb") as file:
          try:
            self.user_data = pickle.load(file)
          except EOFError as e:
             print(e, ",user_data.bin 파일 이상 또는 비어있음")
             self.user_data = {}
             
       with open(self.parking_space_file, "rb") as file:
          try:
            self.parking_space_file = pickle.load(file)
          except EOFError as e:
            print(e, ",ParkingSpaceData.bin 파일 이상 또는 비어있음")
            self.parking_space_data = {}

    def register_data(self, CarNumber, data):
        # 사용자 데이터 등록
        self.user_data[CarNumber] = data
        print(self.user_data)
        with open(self.user_data_file, "wb") as file:
            pickle.dump(self.user_data, file)
        return "데이터가 등록되었습니다."
      
    def search_data(self, CarNumber):
      if CarNumber in self.user_data:
        return self.user_data[CarNumber]
      else:
        return "데이터를 찾을 수 없습니다."


    def match_and_register_data(self, CarNumber, ParkingSpace):
        if CarNumber in self.user_data:
            self.user_data[CarNumber]["ParkingSpace"] = ParkingSpace
            with open(self.user_data_file, "wb") as file:
              pickle.dump(self.user_data, file)
            return "데이터가 등록되었습니다."
        else:
            return "데이터를 다시 입력해주세요."


    # def register_parking_space(self, CarNumber, data):
    #     self.parking_space_data[CarNumber] = data
    #     with open(self.parking_space_file, "wb") as file:
    #         pickle.dump(self.parking_space_data, file)
    #     return "주차 공간이 등록되었습니다."