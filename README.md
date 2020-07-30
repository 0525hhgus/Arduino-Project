# Arduino-Project
어린이 통학버스 사고 예방을 위한 안전 장치 2018-12-01 (BackUp)
# 1. 제작 배경
- 통학버스 사고 빈번하게 발생
  - 기존의 방식 개선 필요
  - 차량 내 아이를 외부에 알리는 것 중요


# 2. 사용 모듈 및 센서
- HBK-ADK-2560
- Sensor Shield
- 인체 감지 센서
- RFID 모듈
- 7 세그먼트 표시장치
- LED
- 스위치

# 3. 기능
## 3-1. 예방
- RFID 모듈 : 사용자의 고유한 NFC칩 배부, 버스 승하차 시 칩을 태그
- 버스 승하차 시 어린이 수 표시

## 3-2. 외부 알림
- LED : 차량 내부의 움직임이 감지되면 외부 LED에 불이 들어온다. 
- 스위치 : 차량 외부 LED를 끌 수 있다.
- 인체 감지 센서 : 차량 내부의 움직임을 감지

# 4. 코드
- [Code](Code/Project_use_RFID.ino)
- 코드 일부

- https://www.youtube.com/watch?v=QvB_2JwLFfI
- https://www.open-lab.co.kr/events/?q=YToyOntzOjEyOiJrZXl3b3JkX3R5cGUiO3M6MzoiYWxsIjtzOjQ6InBhZ2UiO2k6Mjt9&bmode=view&idx=1746252&t=board



