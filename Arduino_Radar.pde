
import processing.serial.*;//carga libreria serial
Serial port;//crea una variable (objeto) para puerto

// magnitude [0] and angle in radians [1]
int n=2; //numero de variables que env'ia el arduino separadas por coma
float[] data = new float[n];
boolean radar;

void setup()
{
  size(700, 700);
  println(Serial.list());//ver e que puerto esta el arduino
  port = new Serial(this, "COM12", 9600);//crea e puerto 9600 debe ser igaul en Arduino
  port.bufferUntil('\n');
}

void draw()
{
 
  if(radar){
    if (data[1]==0) {
    background(0);
    }
    stroke(0, 255, 0, 10);
    strokeWeight(20);
    //hacer algo con los datos
    //p.e. data[0],data[1] contiene distancia y angulo
    //***dibuja lineas en nuevo marco de referencia
    pushMatrix();
    translate(width/2, 10);
    rotate(HALF_PI-data[1]*PI/180);
    line(0, 0, 0, 800);
    stroke(255, 0, 0, 50);
    line(0,data[0]*3,0,800);
    popMatrix();
    
    if (data[1]==0) {
      background(0);
    }
  }
  else{
    if(data[0]==-2){
      fill(98,245,31);
       textSize(40);
       text("Espacio libre detectado a "+data[1] +" CM", width-width*0.975, height-height*0.5277);
       text("Girando a la derecha", width-width*0.775, height-height*0.1277);
    }
    else if(data[0]==2){
      fill(98,245,31);
       textSize(40);
       text("Espacio libre detectado a "+data[1] +" CM", width-width*0.975, height-height*0.5277);
       text("Girando a la izquierda", width-width*0.775, height-height*0.1277);
    }
     
  }
  //******
}

//funcion que corre sola cada vez que entra informaci'on
//por el puerto serial
void serialEvent(Serial port) {
  String bufString = port.readString();
  //Lee el serial 
  // -2 es vuelta a la izquierda  
  //2 es vuelta a la derecha
  if (bufString.indexOf(",")==-1){
    data = float(split(bufString, '|'));
    radar=false;
  }
  else{
    data = float(split(bufString, ','));
    radar=true;
  }
  
}
