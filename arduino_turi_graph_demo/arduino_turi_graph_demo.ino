#include <Wire.h>
#include <FlexiTimer2.h>
#define sbi(PORT,BIT) PORT|=_BV(BIT) // PORTの指定BITに1をセット
#define cbi(PORT,BIT) PORT&=~_BV(BIT) // PORTの指定BITをクリア

// 送信用データ格納変数
int x; // 引き値
int y; // 水深
int z; // 当たり


int led=2;
byte data[4];
byte data2send[4];
byte type;
int sensval;
int sw_state;
char senddata[4];

// ロータリーエンコーダーの配線に合わせる
int rotA,rotB;

//２点の各閾値．雑音余裕をもっておきます．
int shA1=950;
int shA2=900;
int shB1=950;
int shB2=900;

//２センサのセンサ値，センサstate
int vA,vB;
int sA,sB;

int prestate;

// ロータリーエンコーダーの状態を記憶する
// 割り込み中に変化する変数はvolatileはをつけて宣言する
char m_nOldRot = 0;

// 今回はLCDに回転回数を表示させる
long m_nValue  = 0;
long centi=0;




void setup(){
  
  pinMode(led,OUTPUT);
  for(int i=3;i<=15;i++){
      pinMode(i,INPUT_PULLUP);
  }
  
  pinMode(12,INPUT_PULLUP);
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  sw_state=digitalRead(12);
  
  Wire.begin(0x01);
  Wire.onRequest(requestEvent);
  
  Serial.begin(57600);

  sense();
  
  prestate=sA;
  
  FlexiTimer2::set(10, flash);
  FlexiTimer2::start();

}


void loop(){
  
  sense();
  sw_state=digitalRead(12);
  
  if(sA!=prestate){
  rotRotEnc();
  }
  
  prestate=sA;
   sensval=analogRead(0);
   
   
   
  // 送信用に値を変更
  x = m_nValue;
  y = sensval;
  z = sw_state;
  
  //processingからデータが送られてきたら
  //シリアルでvalを送信
if(Serial.available() > 0){
    Serial.write(x);
    Serial.write(y);
    Serial.write(z); 
  //processingからの合図データを読み込む
    //Serial.read();
  }
}





// 以下、深度関連

void sense(){
  vA=analogRead(1);
    if(vA<shA1){
    sA=1;
  }
  else if(vA>shA2){ sA=0; }
  
    vB=analogRead(2);
    if(vB<shB1){
    sB=1;
  }
  else if(vB>shB2){ sB=0; }
}


// 外部割り込みから呼び出される変数
void rotRotEnc(void)
{
  if(!sA){
    if(sB){
      m_nOldRot = 'R';
    } else {
      m_nOldRot = 'L';
    }
  } else {
    if(sB){
      if(m_nOldRot == 'L'){ 
        m_nValue--;
      }
    } else {
      if(m_nOldRot == 'R'){
        m_nValue++;
      }
    }
    m_nOldRot = 0;
  }
}


void flash(){
 Serial.print(m_nValue);
 Serial.print(",");
 Serial.print(sensval);
 Serial.print(",");
 Serial.print(sw_state);
 Serial.println(",");
}


void requestEvent()
{
  digitalWrite(led,HIGH);

  char buf[10];
  itoa2(m_nValue*-1,buf,10,4);
  itoa2(sensval,&buf[4],10,4);
  itoa2(sw_state,&buf[8],10,1);
  Wire.write(buf);

  digitalWrite(led,LOW);
}


void itoa2(int n, char s[], int size, int field)
{
    int i, len;

    itoa(n, s, size);
    len = strlen(s);
    if (len > field || field > size)
        exit(0);
    s[field + 1] = '\0';
    for (i = len; i >= 0; i--)
        s[field - len + i] = s[i];
    for (i = 0; i < field - len; i++)
        s[i] = ' ';
}

