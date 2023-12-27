from flask import Flask, request, jsonify
from flask_cors import CORS
from user_data_management import UserDataManagement
import subprocess

app = Flask(__name__)
CORS(app)

user_data_management = UserDataManagement()
user_data_management.sync_data_()

@app.route("/UserInfo", methods=["POST"])
def handle_user():

    data = request.json
    EventID = data.get("EventID")
    CarNumber = data.get("CarNumber")
    
    # 클라이언트로부터 받은 EventID 출력
    print(f"Received EventID from client: {EventID}") 
    
    if EventID == "1":
        result = user_data_management.search_data(CarNumber)
        if result == "데이터를 찾을 수 없습니다.":
            return user_data_management.register_data(CarNumber, data)
        else:
            return jsonify(result)  # 검색된 데이터를 클라이언트에게 전송

    elif EventID == "2":
      CarNumber = data.get("CarNumber")
      ParkingSpace = data.get("ParkingSpace")
      return user_data_management.match_and_register_data(CarNumber, ParkingSpace)

@app.route("/get-json-data", methods=["GET"])
def get_json_data():

    EventID = request.args.get("EventID")
    
    # 클라이언트로부터 받은 EventID 출력
    print(f"Received EventID from client: {EventID}")

    if EventID == "3":
      result = subprocess.run(["./Rmap_data"], capture_output = True, text = True)
      if result.returncode == 0:
          print("returncode: ", result.returncode)
          json_data = result.stdout # stdout: Standard Output 줄임말, "./Rmap_data" 실행하고 저장하고 반환합니다.
          print("Command Output: ", result.stdout)
          return jsonify(json_data)
      else:
            return jsonify({"message": "데이터가 없습니다."}), 404 # 클라이언트가 요청한 데이터를 서버에서 찾을 수 없는 오류입니다.
    else:
        return jsonify({"message": "잘못된 EventID입니다."}), 404 # 클라이언트가 요청한 데이터를 서버에서 찾을 수 없는 오류입니다.

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=True)