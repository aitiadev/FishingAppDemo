//シリアルライブラリを取り入れる
import processing.serial.*;
Serial myPort;

int[] nums = new int[2];
String readStr;

int NUM = 2; //センサーの数
 
int[] sensors = new int[NUM]; //センサーの値を格納する配列
int cnt; //カウンター
 
// グラフの線の色を格納
color[] col = new color[6];

//対応した変数をそれぞれ用意
int x;
int y;
int z;



void setup(){
  //画面サイズ
  size(800, 400);
  //シリアルポートの設定
  printArray(Serial.list());
  String portName = Serial.list()[11]; //下のポート
  myPort=new Serial(this, portName,57600);
  
  for(int i=0;i<2;i++){
    nums[i]=0; 
  }
  
  initGraph();
}



void draw(){
  
  fill(col[0]);
  ellipse(cnt, x*10+100, 4, 4);
  
  fill(col[1]);
  ellipse(cnt, -y + 500, 4, 4);
  
  if(z == 0){
  fill(col[2]);
  ellipse(750, 350, 40, 40);
  }else{
  fill(0);
  ellipse(750, 350, 40, 40);
  }
  
  if (cnt > width) {
    initGraph();
  }
 // println(x + ", " + y + ", " + z);
  cnt++;
}



//グラフの初期化
void initGraph() {
  background(47);
  noStroke();
  cnt = 0;
  // グラフ描画の線の色を定義
  col[0] = color(255, 127, 31);
  col[1] = color(31, 255, 127);
  col[2] = color(0, 255, 255);
  col[3] = color(31, 127, 255);
  col[4] = color(127, 255, 31);
  col[5] = color(127);
}


void serialEvent(Serial p){

    String inString = p.readStringUntil('\n');
    
    if(inString != null) {
    println(inString);
    inString = trim(inString);
    nums = int(trim(split(inString, ',')));
    x = nums[0];
    y = nums[1];
    z = nums[2];
  }
}


void dispose(){
  myPort.clear();
  myPort.stop();
  println("Safety stopped!");
}

