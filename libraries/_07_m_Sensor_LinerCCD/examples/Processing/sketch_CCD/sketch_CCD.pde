import processing.serial.*;
//float  [] PixelArray = new float[128];
int  [] PixelArray = new int[128];
int i = 0;
Serial myPort;

PFont f;
 
 void setup() {
   //size(1000, 200);
   size(1000, 250);
   f = createFont("Arial",12,true);
   fill(0);
   println(Serial.list());
   myPort = new Serial(this,Serial.list()[0],38400);
   myPort.bufferUntil('\n');
 }
 
void draw() {
  for(i = 0; i < 128; i ++){
    stroke(PixelArray[i]); 
    for(int a=0;a<8;a++){
      line(i*8+a, 0, i*8+a, 200);
    }
    
    //show pixel num and value


      //text(PixelArray[i],i*8,220);
      line(i*8, 200, i*8, 250);
      String num=str(i);
      int size=num.length();
      for(int j=0; j<size; j++) {
        text(num.charAt(j),i*8,220+(j*10));
      }
      

  }
 }


void serialEvent(Serial myPort) { 
   String inString = myPort.readStringUntil('\n');
   if (inString != null) {
     inString = trim(inString);
     int[] colors = int(split(inString, ","));
     if (colors.length >=128) {
       for(i = 0; i < 128; i ++){
         PixelArray[i] = colors[i];
         print(PixelArray[i]);
         print(", ");
       }
       println();
     }
   }
}