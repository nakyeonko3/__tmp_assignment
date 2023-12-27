# 과제 : Parking Space Management System Assignement

### 프로그램 실행 방법

```bash
python -m venv .venv
source .venv/bin/activate
pip install flask
pip install flask-cors
python server.py
```


### 아래 명령어로 테스트 하면 됩니다


```bash
curl -d '{"EventID":"1", "UserName":"nakyeonko", "CarType":"value2", "CarNumber":"312314"}' -H "Content-Type: application/json" -X POST http://localhost:5000//UserInfo

```

```bash
curl -d '{"EventID":"2", "ParkingSpace":25, "CarNumber":"312314"}' -H "Content-Type: application/json" -X POST http://localhost:5000//UserInfo
```

```bash
python Ruser.py
```


