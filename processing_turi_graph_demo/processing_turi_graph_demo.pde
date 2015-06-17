//シリアルライブラリを取り入れる
import processing.serial.*;
//myPort（任意名）というインスタンスを用意
Serial myPort;

//対応した変数をそれぞれ用意
int x = 0;
int y = 0;
int z = 0;

void setup(){
  //画面サイズ
  size(256,256);
  //シリアルポートの設定
  //printArray(Serial.list());
  String portName = Serial.list()[11]; //下のポート
  myPort=new Serial(this, portName,9600);
}

void draw(){
  //背景色を白に設定
  background(255);
  //XY座標を(x,100)に設定し、
  //幅50、高さ50の円を描画
  ellipse(x,y,50,50);
  println(x + ", " + y + ", " + z);
}

void serialEvent(Serial p){
  //Arduinoから送られてきたデータが3個の場合
  if(myPort.available() > 2) {
  // x, y, zの順番でデータを読み込む
  x = myPort.read();
  y = myPort.read();
  z = myPort.read();
  // 読み込み後、合図データを送信
  myPort.write(65);
  }
}


//マウスが押されたら通信開始
void mousePressed() {
  myPort.clear();
  myPort.write(65);
}


void dispose(){
  myPort.clear();
  myPort.stop();
  println("Safety stopped!");
}

