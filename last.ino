//메뉴를 넣을 때에는 시리얼 모니터에 1~4의 값을 넣어주면 됨
// 1 = 김치 2,3 아리까리함 4 그냥 테스트 용도
//

#include <Servo.h>
#include <SoftwareSerial.h>

#define rxd 15
#define txd 14

SoftwareSerial esp(rxd, txd);

#define sensor1 A1
#define sensor2 A2
#define sensor3 A3
#define sensor4 A4
#define sensor5 A5
#define sensor6 A6

//컨베이어, 포토 용
#define con_motor1 7
#define con_motor2 6

#define kimchi A0
#define budae 29
#define sundubu 30

//워터펌프 용
#define wat_motor1 5
#define wat_motor2 4

#define con_photo 10
#define bow_photo 11

#define con_speed 40

//재료통 홀더용
#define hol_motor1 3
#define hol_motor2 2

#define encoderPinA 21
#define encoderPinB 20

int encoderPos = 0;
const float ratio = 360./27./52.;//엔코더 회전 비율을 설정해주는 상수

int motor_state = 0;


float Kp = 20;//p제어 상수

unsigned long past = 0;
unsigned long now = 0;

void doEncoderA(){  encoderPos += (digitalRead(encoderPinA)==digitalRead(encoderPinB))?-1:1;}
void doEncoderB(){  encoderPos += (digitalRead(encoderPinA)==digitalRead(encoderPinB))?1:-1;}

void doMotor(bool dir, int vel){
  
  digitalWrite(hol_motor1, dir);
  analogWrite(hol_motor2, dir?(-vel):vel);
}

float motorDeg = 0;//현재 각도를 0으로 설정
float past_motorDeg = 0;//각도를 0으로 설정
#define pin1 42
#define pin2 43
#define pin3 44
#define pin4 45
#define pin5 46
#define pin6 47

#define button 22
#define interruptpin 37

Servo servo1, servo2, servo3, servo4, servo5, servo6;


//그릇배출기용
#define bow_motor1 8
#define bow_motor2 9

//LED용
#define LED1 48
#define LED2 49
#define LED3 50
#define LED4 51
#define LED5 52
#define LED6 53
/////////////////////////////////////쓰는 거 ㄴㄴ/////////////////////////////////////////////////////
class Button_Class{
  private :
    char state;
    int Speed = 0;
    int cha = 0;
    
  public :
    void button_play(char get_state, int get_speed, float get_cha){
      Speed = get_speed;
      state = get_state;
      cha = get_cha;
      if(state == "hol", Speed < cha) {
         ////////////////-방향//////////////////////
         digitalWrite(hol_motor1, HIGH);
         digitalWrite(hol_motor2, LOW);
      }
      else {
        ////////////////+방향//////////////////////
          digitalWrite(hol_motor1, LOW);
          digitalWrite(hol_motor2, HIGH);
      }
    }
};
/////////////////////////////////////쓰는 거 ㄴㄴ/////////////////////////////////////////////////////


//////////////////////////////////////포토 인터럽터 사용하는 클래스///////////////////////////////////////
class Photo_Class{
  private :
    int target = 0;
  public :
    bool photo_state(int get_target){
      target = get_target;
      switch(target){
        case con_photo :
          if(digitalRead(con_photo) == HIGH){
            return true;
          }
          else{
            return false;
          }
          break;
        case bow_photo :
          if(digitalRead(bow_photo) == HIGH){
            return true;
          }
          else{
            return false;
          }
          break;
      }
    }
};///////////////인터럽터 값이 읽히면 HIGH이니까 true를 리턴함//////////////////////////////////////////////
//////////////////////////////////////포토 인터럽터 사용하는 클래스///////////////////////////////////////

////////////////////////////////컨베이어를 사용하는 클래스//////////////////////////////////////////////
class Con_Class{
  
  private :
    Photo_Class photo;
  public :
    void con_play(){//인터럽터가 LOW일 동안 모터를 진행
      delay(100);
      while(photo.photo_state(con_photo)){
        digitalWrite(con_motor1, HIGH);
        analogWrite(con_motor2, -con_speed);
      }
    }
    void con_escape(){//인터럽터가 HIGH일 동안 모터를 진행
      while(!photo.photo_state(con_photo)){
        digitalWrite(con_motor1, HIGH);
        analogWrite(con_motor2, -con_speed);
      }
    }
    void con_stop(){
      digitalWrite(con_motor1, LOW);
      digitalWrite(con_motor2, LOW);
    }
};
///////////////////////////만약 컨베이어 모터의 속도를 조절하고 싶다면 위 쪽에서 con_speed값을 조정하면 됨/////////////////////////
///////////////////////////////컨베이어를 사용하는 클래스/////////////////////////////////////

/////////////////////////그릇 배출기를 사용하는 클래스/////////////////////////////////////////
class Bow_Class{
  private:
    Photo_Class photo;
  public:
    void bow_play(){
      while(photo.photo_state(bow_photo)){
        digitalWrite(bow_motor1, HIGH);
        analogWrite(bow_motor2, -con_speed);
      }
    }
    void bow_escape(){
      while(!photo.photo_state(bow_photo)){
        digitalWrite(bow_motor1, HIGH);
        analogWrite(bow_motor2, -con_speed);
      }
    }
    void bow_stop(){
      digitalWrite(bow_motor1, LOW);
      digitalWrite(bow_motor2, LOW);
    }
};
//////////////////컨베이어와 설명은 같음/////////////////////////////////////
/////////////////////////그릇 배출기를 사용하는 클래스/////////////////////////////////////////

//////////////////////////////LED를 사용하는 클래스////////////////////////////
class LED_Class{
  private :
    int state = 0;
  public :
    void LED_on(int get_state){
      state = get_state;
      switch(state){
        case 1:
          state = LED1;
          break;
        case 2:
          state = LED2;
          break;
        case 3:
          state = LED3;
          break;
        case 4:
          state = LED4;
          break;
        case 5:
          state = LED5;
          break;
        default:
          state = LED6;
      }
      digitalWrite(state, LOW);
    }
    void LED_off(int get_state){
      state = get_state;
      switch(state){
        case 1:
          state = LED1;
          break;
        case 2:
          state = LED2;
          break;
        case 3:
          state = LED3;
          break;
        case 4:
          state = LED4;
          break;
        case 5:
          state = LED5;
          break;
        default:
          state = LED6;
      }
      digitalWrite(state, HIGH);
    }
    void LED_all_off(){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
    }
};
//////////////////////////함수의 매개변수로 어디 LED를 끌지 켤지를 결정///////////////////////////////
/////////////////////////////////LED를 사용하는 클래스//////////////////////////

//////////////////서보 모터를 사용하는 클래스/////////////////////////////////////////
class Servo_Class{
  private :
    int state = 0;
    int angle = 180;
  public :
    void servo_play(int get_state){
      state = get_state;
      sensor_play(state);
      switch(state){
        case 1:
          for(int i = 0; i <= angle; i ++){
            servo1.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo1.write(i*2);
            delay(10);
          }
          
          break;
        case 2:
          for(int i = 0; i <= angle; i ++){
            servo2.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo2.write(i*2);
            delay(10);
          }
          
          break;
        case 3:
          for(int i = 0; i <= angle; i ++){
            servo3.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo3.write(i*2);
            delay(10);
          }
          
          break;
        case 4:
          for(int i = 0; i <= angle; i ++){
            servo4.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo4.write(i*2);
            delay(10);
          }
          
          break;
        case 5:
          for(int i = 0; i <= angle; i ++){
            servo5.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo5.write(i*2);
            delay(10);
          }
          
          break;
        case 6:
          for(int i = 0; i <= angle; i ++){
            servo6.write(i*2);
            delay(10);
          }
          for(int i = angle; i >= 0; i --){
            servo6.write(i*2);
            delay(10);
          }
          break;
      }
    }
};
///////////////////////만약 모터의 회전각을 조절하고 싶으면 이 클래스에 angle이라는 변수의 값을 조정해주면 됨/////////////////////////////
//////////////////서보 모터를 사용하는 클래스/////////////////////////////////////////

////////////////////////////////////워터펌프를 사용하는 클래스/////////////////////////////////////////////
class Wat_Class{
  private :
    int delay_time = 4000;

  public :
    void wat_play(){
      digitalWrite(wat_motor1, HIGH);
      digitalWrite(wat_motor2, LOW);
      delay(delay_time);
      /*wat_stop();
      delay(500);
      digitalWrite(wat_motor1, LOW);
      digitalWrite(wat_motor2, HIGH);
      delay(500);*/
    }
    void wat_stop(){
      digitalWrite(wat_motor1, LOW);
      digitalWrite(wat_motor2, LOW);
    }
};
////////////////////////////물이 나오는 시간을 조절하고 싶으면 이 클래스에 delay_time이라는 변수의 값을 조정하면 됨/////////////////////
////////////////////////////////////워터펌프를 사용하는 클래스/////////////////////////////////////////////

//////////////////////////////세븐 세그먼트를 사용하는 클래스임//////////////////////////////////////
class Seg_Class{
  private :
    int state = 0;
  public:
    void seg_play(int get_state){
      state = get_state;
      switch(state){
        case 1:
          digitalWrite(31, LOW);
          digitalWrite(30, LOW);
          digitalWrite(29, LOW);
          digitalWrite(28, HIGH);
          break;
        case 2:
          digitalWrite(31, LOW);
          digitalWrite(30, LOW);
          digitalWrite(29, HIGH);
          digitalWrite(28, LOW);
          break;
        case 3:
          digitalWrite(31, LOW);
          digitalWrite(30, LOW);
          digitalWrite(29, HIGH);
          digitalWrite(28, HIGH);
          break;
        case 4:
          digitalWrite(31, LOW);
          digitalWrite(30, HIGH);
          digitalWrite(29, LOW);
          digitalWrite(28, LOW);
          break;
        case 5:
          digitalWrite(31, LOW);
          digitalWrite(30, HIGH);
          digitalWrite(29, LOW);
          digitalWrite(28, HIGH);
          break;
        case 6:
          digitalWrite(31, LOW);
          digitalWrite(30, HIGH);
          digitalWrite(29, HIGH);
          digitalWrite(28, LOW);
          break;
        case 7:
          digitalWrite(31, LOW);
          digitalWrite(30, HIGH);
          digitalWrite(29, HIGH);
          digitalWrite(28, HIGH);
          break;
        case 8:
          digitalWrite(31, HIGH);
          digitalWrite(30, LOW);
          digitalWrite(29, LOW);
          digitalWrite(28, LOW);
          break;
        default :
          digitalWrite(31, HIGH);
          digitalWrite(30, LOW);
          digitalWrite(29, LOW);
          digitalWrite(28, HIGH);
      }
    }
    
    void seg1_play(int get_state){
      state = get_state;
      switch(state){
        case 1:
          digitalWrite(41, LOW);
          digitalWrite(40, LOW);
          digitalWrite(39, LOW);
          digitalWrite(38, HIGH);
          break;
        case 2:
          digitalWrite(41, LOW);
          digitalWrite(40, LOW);
          digitalWrite(39, HIGH);
          digitalWrite(38, LOW);
          break;
        case 3:
          digitalWrite(41, LOW);
          digitalWrite(40, LOW);
          digitalWrite(39, HIGH);
          digitalWrite(38, HIGH);
          break;
        case 4:
          digitalWrite(41, LOW);
          digitalWrite(40, HIGH);
          digitalWrite(39, LOW);
          digitalWrite(38, LOW);
          break;
        case 5:
          digitalWrite(41, LOW);
          digitalWrite(40, HIGH);
          digitalWrite(39, LOW);
          digitalWrite(38, HIGH);
          break;
        case 6:
          digitalWrite(41, LOW);
          digitalWrite(40, HIGH);
          digitalWrite(39, HIGH);
          digitalWrite(38, LOW);
          break;
        case 7:
          digitalWrite(41, LOW);
          digitalWrite(40, HIGH);
          digitalWrite(39, HIGH);
          digitalWrite(38, HIGH);
          break;
        case 8:
          digitalWrite(41, HIGH);
          digitalWrite(40, LOW);
          digitalWrite(39, LOW);
          digitalWrite(38, LOW);
          break;
        default :
          digitalWrite(41, HIGH);
          digitalWrite(40, LOW);
          digitalWrite(39, LOW);
          digitalWrite(38, HIGH);
      }
    }
    void seg1_stop(){
      digitalWrite(38, HIGH);
      digitalWrite(39, HIGH);
      digitalWrite(40, HIGH);
      digitalWrite(41, HIGH);
    }
    void seg1_zero(){
      digitalWrite(38, LOW);
      digitalWrite(39, LOW);
      digitalWrite(40, LOW);
      digitalWrite(41, LOW);
    }
    void seg_stop(){
      digitalWrite(28, HIGH);
      digitalWrite(29, HIGH);
      digitalWrite(30, HIGH);
      digitalWrite(31, HIGH);
    }
};
///////////////////////////////////////매개변수로 무슨 숫자를 표시할지 넣으면 됨///////////////////////////////
//////////////////////////////seg가 망가진 세그먼트 seg1이 정상인 세그먼트
//////////////////////////////////////세븐 세그먼트를 사용하는 함수임///////////////////////////////////////////////

/////////////////////////////재료통 홀더를 사용하는 클래스/////////////////////////////////////////
class Hol_Class{
  Seg_Class seg;
  private :
    
    int state = 0;
  public :
    void get_play(int get_state){
      state = get_state;
      switch(state){
        case 1:
          past = millis();
          hol_play(0);
          break;
        case 2:
          past = millis();
          hol_play(60);
          break;
        case 3:
          past = millis();
          hol_play(120);
          break;
        case 4:
          past = millis();
          hol_play(0);
          break;
        case 5:
          past = millis();
          hol_play(-120);
          break;
        default :
          past = millis();
          hol_play(-60);
      }
    }
    void hol_stop(){
      while(1){
        digitalWrite(hol_motor1, LOW);
        digitalWrite(hol_motor2, LOW);
        delay(3000);
        break;
      }
    }
    
    void starting_point(){//원점을 찾는 함수
      int targetDeg = 0;
      past = millis();
      while(2 <= abs(motorDeg-targetDeg)){
        motorDeg = float(encoderPos)*ratio;
        now = millis();
        /////////////////////////////////////////////////////////////////////////
        if((now - past) > 5000){//5초가 되면 실행 된다는 것
          if(1 > abs(past_motorDeg - motorDeg)){//5초 동안 전의 각도와 지금 각도의 차가 1도 이상 차이가 없다면 실행하라는 것

            while(2 <= abs(motorDeg-targetDeg)){
              
              now = millis();
              if((now - past) > 7000){//7초가 되면 죽는 다는 것
                  hol_stop();
                  while(digitalRead(button) == LOW){
                    seg.seg_play(7);//에러 코드 바꾸고 싶으면  7 대신 다른 1의 자리 숫자
                    if(digitalRead(button) == HIGH){
                      seg.seg_stop();
                      motorDeg = targetDeg;
                      break;
                    }
                  }
                  
              }
              past_motorDeg = motorDeg;
              
            }
          }
        }
          
              
            
        
        past_motorDeg = motorDeg;
        ////////////////////////////////////////////////////////////
        float error = targetDeg - motorDeg;
        int control = Kp*error;
      
        
        doMotor((control>=0)?LOW:HIGH, min(abs(control), 35));//속도 올리고 싶으면 35라는 걸 올리면 됨
        /*digitalWrite(hol_motor1, LOW);
        analogWrite(hol_motor2, 40);*/
        Serial.print("encoderPos : ");
        Serial.print(encoderPos);
        Serial.print("   motorDeg : ");
        Serial.println(float(encoderPos)*ratio);
      }
    }
    void hol_play(int get_state){//위치를 찾아가는 함수
      int targetDeg = get_state;
      while(2 <= abs(motorDeg-targetDeg)){
        motorDeg = float(encoderPos)*ratio;
        now = millis();
        /////////////////////////////////////////////////////////////////////////
        if((now - past) > 5000){//5초가 되면 실행 된다는 것
          if(1 > abs(past_motorDeg - motorDeg)){//5초 동안 전의 각도와 지금 각도의 차가 1도 이상 차이가 없다면 실행하라는 것

            while(2 <= abs(motorDeg-targetDeg)){
              
              now = millis();
              if((now - past) > 7000){//7초가 되면 죽는 다는 것
                  hol_stop();
                  while(digitalRead(button) == LOW){
                    seg.seg_play(7);//에러 코드 바꾸고 싶으면  7 대신 다른 1의 자리 숫자
                    if(digitalRead(button) == HIGH){
                      seg.seg_stop();
                      motorDeg = targetDeg;
                      break;
                    }
                  }
                  
              }
              past_motorDeg = motorDeg;
              
            }
          }
        }
          
              
            
        
        past_motorDeg = motorDeg;
        ////////////////////////////////////////////////////////////
        float error = targetDeg - motorDeg;
        int control = Kp*error;
      
        
        doMotor((control>=0)?LOW:HIGH, min(abs(control), 35));//속도 올리고 싶으면 35라는 걸 올리면 됨
        /*digitalWrite(hol_motor1, LOW);
        analogWrite(hol_motor2, 40);*/
        Serial.print("encoderPos : ");
        Serial.print(encoderPos);
        Serial.print("   motorDeg : ");
        Serial.println(float(encoderPos)*ratio);
      }
    }
      
};
/////////////주석으로 범위가 설정된 곳이 부하 걸리면 에러 코드를 발생하고 얍삽이 사용 가능한 곳//////////////
/////////////////////////지금 각도를 확인하고 싶으면 시리얼 모니터를 확인하면 확인 가능함/////////////
/////////////////////////////재료통 홀더를 사용하는 클래스/////////////////////////////////////////
int sensor_val = 0;
int xxxx = 0;
void sensor_play(int sensor_num){
  int button_state = 0;
  Hol_Class hol;
  Seg_Class seg;
  switch(sensor_num){
    case sensor1 :
      sensor_val = sensor1;
      xxxx = 1;
      break;
    case sensor2 :
      sensor_val = sensor2;
      xxxx = 2;
      break;
    case sensor3 :
      sensor_val = sensor3;
      xxxx = 3;
      break;
    case sensor4 :
      sensor_val = sensor4;
      xxxx = 4;
      break;
    case sensor5 :
      sensor_val = sensor5;
      xxxx = 5;
      break;
    default :
      sensor_val = sensor6;
      xxxx = 6;
  }
  int x = map(sensor_val, 0, 1023, 0, 100);
  if (x > 20) {
    return true;
  }
  else {
    seg.seg_play(xxxx);
    hol.hol_stop();
    while(button_state < 2){//처음에 몇 번을 누를 것인지
      if (digitalRead(button) == HIGH){
        delay(10);
        if(digitalRead(button) == HIGH){
          while(digitalRead(button) == HIGH){
          }
          button_state ++;
        }
      }
    }
    hol.get_play(xxxx+2);
    hol.hol_stop();
    while(x <= 20){
      Serial.println(xxxx);
    }
    while(button_state < 3){//1번 누르면 이 함수를 탈출
      if (digitalRead(button) == HIGH){
        delay(10);
        if(digitalRead(button) == HIGH){
          while(digitalRead(button) == HIGH){
          }
          button_state ++;
        }
      }  
    }
  }
}
void setup() {
  LED_Class led;
  Seg_Class seg;
  
  pinMode(encoderPinA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
 
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);

  pinMode(hol_motor1, OUTPUT);
  pinMode(hol_motor2, OUTPUT);
  
  pinMode(con_motor1, OUTPUT);
  pinMode(con_motor2, OUTPUT);

  pinMode(kimchi, INPUT);
  pinMode(sundubu, INPUT);
  pinMode(budae, INPUT);
  
  pinMode(bow_motor1, OUTPUT);
  pinMode(bow_motor2, OUTPUT);

  pinMode(wat_motor1, OUTPUT);
  pinMode(wat_motor2, OUTPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  led.LED_all_off();

  pinMode(button, INPUT);

  pinMode(con_photo, INPUT);
  pinMode(bow_photo, INPUT);
//////////////처음에 서브 모터 각도를 설정해주는 곳////////////////////////
  servo1.attach(pin1);
  servo1.write(0);

  servo2.attach(pin2);
  servo2.write(0);

  servo3.attach(pin3);
  servo3.write(0);

  servo4.attach(pin4);
  servo4.write(0);

  servo5.attach(pin5);
  servo5.write(0);

  servo6.attach(pin6);
  servo6.write(0);
//////////////처음에 서브 모터 각도를 설정해주는 곳////////////////////////
  pinMode(41, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(38, OUTPUT);

  pinMode(31, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(28, OUTPUT);

  pinMode(button, INPUT);
  pinMode(interruptpin, OUTPUT);
  Serial.begin(9600);

  digitalWrite(38, LOW);
  digitalWrite(39, LOW);
  digitalWrite(40, LOW);
  digitalWrite(41, LOW);

  digitalWrite(28, HIGH);
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);

  esp.begin(115200);

  //attachInterrupt(digitalPinToInterrupt(button), interrupt, CHANGE);
  //만약에 안락사 버튼을 살리고 싶으면 위에 주석 삭제하면 됨

  seg.seg1_zero();
}
int num = 0;
void interrupt(){
  Con_Class con;
  Hol_Class hol;
  Bow_Class bow;
  bow.bow_stop();
  con.con_stop();
  hol.hol_stop();
  Wat_Class wat;
  wat.wat_stop();
  Serial.println("안락사데스..");
  while(1){
    
  }
}

int serial_input = 0;
int get_order(){
  if(digitalRead(kimchi) == HIGH){
    return 49;
  }
  else if(digitalRead(sundubu) == HIGH){
    return 50;
  }
  else if(digitalRead(budae) == HIGH){
    return 51;
  }
}

int orderlist[] = {0,0,0,0,0,0,0};
int ordernum = 0;
void loop() {
  Con_Class con;
  LED_Class led;
  Bow_Class bow;
  Photo_Class photo;
  Hol_Class hol;
  Servo_Class servo;
  Wat_Class wat;
  Seg_Class seg;
  
  
  
  while(1){
    serial_input = 0;
    serial_input = get_order();
    Serial.println(serial_input);  
    con.con_play();
    Serial.println(orderlist[0]);
    Serial.println("on");
    if(!photo.photo_state(con_photo)){
      con.con_stop();
      
    }
    if(serial_input != 0){
      break;
    }
  }
  
  
  //시리얼 모니터로 'q'라는 값을 입력받으면 그릇 배출
  if(serial_input > 48){
    //Serial.println(serial_input);
    if(digitalRead(bow_photo) == LOW){
      bow.bow_escape();
      bow.bow_play();
      if(digitalRead(bow_photo) == LOW){
        bow.bow_stop();
        orderlist[0] = serial_input;
        Serial.println(orderlist[0]);
        serial_input = 0;
      }
      delay(1000);
    }
    else{
      bow.bow_play();
      if(digitalRead(bow_photo) == LOW){
        bow.bow_escape();
        bow.bow_play();
        if(digitalRead(bow_photo) == LOW){
          bow.bow_stop();
          orderlist[0] = serial_input;
          Serial.println(orderlist[0]);
          serial_input = 0;  
        }
        delay(1000);
      }
    }
  }
  while(orderlist[0] + orderlist[1] + orderlist[2]
  + orderlist[3] + orderlist[4] + orderlist[5] + orderlist[6]){
    
    for(int i = 0; i < 7; i++){
      if(orderlist[i] > 48){
    
        led.LED_all_off();
        
        if(!photo.photo_state(con_photo)){
          con.con_escape();
        }
        con.con_play();
        
        int new_list[] = {0,0,0,0,0,0,0};
        for(int i = 0; i < 6; i++){
          new_list[i+1] = orderlist[i];
          orderlist[i] = 0;
        }
        orderlist[6] = 0;
        for(int i = 0; i < 7; i++){
          orderlist[i] = new_list[i];
        }
        con.con_stop();
        
        ////////////////////////재료넣기//////////////////////////////////////
        while(1){
          delay(500);
          //if(i == 2){
            if(orderlist[1] > 48) {
              wat.wat_play();
              wat.wat_stop();  
              Serial.println("wat");
            }
            if(orderlist[2] == 49){
              //1번 메뉴 구라띠 버전
              
              hol.get_play(1);
              hol.hol_stop();
              servo.servo_play(1);
              hol.get_play(2);
              hol.hol_stop();
              servo.servo_play(2);
              hol.get_play(3);
              hol.hol_stop();
              servo.servo_play(3);
              hol.starting_point();
              hol.hol_stop();
              
            }
            else if(orderlist[2] == 50){
              //2번 메뉴
              hol.get_play(1);
              hol.hol_stop();
              servo.servo_play(1);
              hol.get_play(3);
              hol.hol_stop();
              servo.servo_play(3);
              hol.get_play(5);
              hol.hol_stop();
              servo.servo_play(5);
              hol.starting_point();
              hol.hol_stop();
              
            }
            else if(orderlist[2] == 51){
              //3번 메뉴
              hol.get_play(2);
              hol.hol_stop();
              servo.servo_play(2);
              hol.get_play(3);
              hol.hol_stop();
              servo.servo_play(3);
              hol.get_play(6);
              hol.hol_stop();
              servo.servo_play(6);
              hol.starting_point();
              hol.hol_stop();
              
            }
            else if(orderlist[2] == 52){
              //4번 메뉴
              hol.get_play(5);
              hol.hol_stop();
              servo.servo_play(5);
              hol.get_play(6);
              hol.hol_stop();
              servo.servo_play(6);
              hol.starting_point();
              hol.hol_stop();
              
            }
            
          //}
          //else if(i == 6){
            if(orderlist[6] > 48){
              hol.starting_point();
              servo.servo_play(4);
              ordernum ++;
              seg.seg1_play(ordernum);
            }
            break;
          }
        ////////////////////////재료넣기//////////////////////////////////////
        for(int i = 0; i < 7; i++){
          if(orderlist[i] > 0){
            led.LED_on(i);
          }
          else{
            led.LED_off(i);
          }
        }
        
        
        while(1){//5초 대기
          int a = get_order();
          delay(5000);
        
          if(a > 48){
            //Serial.println(serial_input);
            if(digitalRead(bow_photo) == LOW){
              bow.bow_escape();
              bow.bow_play();
              if(digitalRead(bow_photo) == LOW){
                bow.bow_stop();
                orderlist[0] = a;
                Serial.println(orderlist[0]);
                
              }
              delay(1000);
            }
            else{
              bow.bow_play();
              if(digitalRead(bow_photo) == LOW){
                bow.bow_stop();
              }
            }
          }
          break;
        }
        //Serial.println(orderlist[1]);
      }    
    }
  }
  
}
    
