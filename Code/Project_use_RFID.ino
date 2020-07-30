#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 5
#define PIR 11 // 인체감지센서 11번핀
#define LED A0 // 사용할 LED A0 핀
#define SWITCH 10

const int a = 9;
const int b = 8;
const int c = 2;
const int d = 3;
const int e = 7;
const int f = 6;
const int g = 4; //LED 7 세그먼트 핀 지정

int flag = 0; //차 안으로 들어온 사람 수
int enter = 0; //들어올 때 0
int IN = 0; //RFID 인식하는 경우 0

// 라이브러리 생성
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

byte nuidPICC[4]; //이전 ID와 비교하기위한 변수

void setup() {
  Serial.begin(9600);
  SPI.begin(); // SPI 시작
  rfid.PCD_Init(); // // RFID 시작
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; //초기 키 ID 초기화
  }
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.println(F("Using the following key:"));

  // 인체감지센서의 핀을 INPUT으로 설정합니다.
  pinMode(PIR, INPUT);
  // LED의 핀을 OUTPUT으로 설정합니다.
  pinMode(LED, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP); //스위치

}

void printDec(byte *buffer, byte bufferSize) //10진수로 변환하는 함수
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void loop() {

  display_7_segment(flag); //현재 탑승한 사람 수 출력
  delay(1000);  //1초동안 대기

  if (IN == 0) {
    // 카드가 인식되었다면 다음으로 넘어가고 아니면 더이상
    // 실행 안하고 리턴
    if ( ! rfid.PICC_IsNewCardPresent())
      return;

    // ID가 읽혀졌다면 다음으로 넘어가고 아니면 더이상
    // 실행 안하고 리턴
    if ( ! rfid.PICC_ReadCardSerial())
      return;

    //카드의 타입을 읽어온다.
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    // MIFARE 방식인지 확인하고 아니면 리턴
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      return;
    }

     //바로 전에 인식한 RF 카드와 다를 경우
    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
        rfid.uid.uidByte[1] != nuidPICC[1] ||
        rfid.uid.uidByte[2] != nuidPICC[2] ||
        rfid.uid.uidByte[3] != nuidPICC[3] ) {
      Serial.println("Hello!");

      // ID를 저장
      for (byte i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }

      Serial.print(F("The NUID tag in dec is: ")); //10진수로 출력
      printDec(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();

      if (enter == 1)  //나가는 경우
      {
        if (rfid.uid.uidByte[0] == 100) 
        {
          Serial.println("Goodbye Hyeonseo!!");
          flag = flag - 1;
        }

        else if (rfid.uid.uidByte[0] == 44) 
        {
          Serial.println("Goodbye Yeonju!!");
          flag = flag - 1;
        }

        else if (rfid.uid.uidByte[0] == 161) 
        {
          Serial.println("Goodbye Andrew!!");
          flag = flag - 1;
        }

        if (flag == 0) 
        {
          IN = 1; 
        }
      }

      delay(2000);

      if (flag < 3 && enter == 0) //들어오는 경우
      {
        if (rfid.uid.uidByte[0] == 100) 
        {
          Serial.println("Welcome Hyeonseo!!");
          flag = flag + 1;
        }

        else if (rfid.uid.uidByte[0] == 44) 
        {
          Serial.println("Welcome Yeonju!!");
          flag = flag + 1;
        }

        else if (rfid.uid.uidByte[0] == 161) 
        {
          Serial.println("Welcome Andrew!!");
          flag = flag + 1;
        }
        if (flag == 3) 
        {
          enter = 1;
        }
      }
      delay(2000);
    }

    if (rfid.uid.uidByte[0] == nuidPICC[0] ||
        rfid.uid.uidByte[1] == nuidPICC[1] ||
        rfid.uid.uidByte[2] == nuidPICC[2] ||
        rfid.uid.uidByte[3] == nuidPICC[3] ) //바로 전에 인식한 것과 동일할 경우
        {
      Serial.println(F("Card read previously."));
        }

      // PICC 종료
      rfid.PICC_HaltA();

      // Stop encryption on PCD
      rfid.PCD_StopCrypto1();

    
  }

  if (IN == 1) {
    // 적외선 인체감지 센서에서 값을 읽는다
    int sensor = digitalRead(PIR);
    // 센서값을 시리얼 모니터에 출력
    Serial.println(sensor);

    // 센서에 감지될 경우 LED를 키고 시리얼 모니터에 Detect 출력한다.
    // 센서값이 HIGH일 경우 9번 LED를 킨다.
    if (sensor == HIGH)
    {
      // 내장된 LED가 연결된 핀의 로직레벨을 HIGH (5V)로 설정하여, LED가 켜지도록 합니다.
      digitalWrite(LED, HIGH);
      Serial.println("Detect");
      delay(2000);
    }

    // 센서에 감지되지 않을 경우 시리얼 모니터에 Not Detect를 출력한다.
    if (sensor == LOW)
    {
      Serial.println("Not Detect");
      delay(2000);
    }

    //인체 감지 센서에 감지되어 LED가 켜졌을 때 스위치를 눌러서 LED를 끈다.
    if (digitalRead(SWITCH) == LOW)
    {
      digitalWrite(LED, LOW);
      delay(3000);
    }
  }
}

void display_7_segment(int num) //LED 7 세그먼트 숫자
{
  switch (num) {
    case 0:
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 1:
      digitalWrite(a, HIGH);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 2:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, HIGH);
      break;
    case 3:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, HIGH);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    default:
      break;
  }
}
