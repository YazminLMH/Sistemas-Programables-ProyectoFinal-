#include <Servo.h>

#include <Ultrasonic.h>
Servo myservo; 


#define TRIGGER A0     // Pin para enviar el pulso de disparo
#define ECHO A1       // Pin para recibir el pulso de eco


int obstamax =14; // distancia maxima 
int obstamin= 2; // distancia minima 
int i=0;
long  gradoDetectado=0;

int sensorizq2 = 5;
int Stateizq2 = 0;
int sensorder2 = 4;//
int Stateder2 = 0;
int sensorizq1 = 7;
int Stateizq1 = 0;
int sensorder1 = 6;
int Stateder1 = 0;
int sensorizq = 8;//sensor derecha debajo del carro
int Stateizq = 0;
int sensorder = 9;//sensor izquierada debajo del carro
int Stateder = 0;
int ledPin0 =  7; //8
int ledPin1 =  8; //9
int ledPin2 =  5; 
int ledPin3 =  6; 
int powerinfrarojo = 12;//infrarrojo debajo del carro
//int powerinfrarojo1 = 10;
int powerinfrarojo2 = 11;
int kk = 0; 
int dato1=0;
int dato2=0;
int dato3=0;

int estado =0; 
int tt = 0; 
int duracion;
int distancia;
int CMD;
int CMC;
int CMI;
int CM;
//////////////// Variables para el radar


Ultrasonic ultrasonic(TRIGGER, ECHO);
////////////////////
int angizq= 150;
int angdere= 30;
int angcent=90;
int distanciaMaxima; 

int velocidad= 140;// Velocidad del carrito (velocidad a la que va el carrito)

void setup()
{
Serial.begin(9600);
myservo.attach(2); // pin del arduino al que va conectado el servomotor
pinMode(ECHO, INPUT);
pinMode(TRIGGER, OUTPUT);

pinMode(ledPin0, OUTPUT);
pinMode(ledPin1, OUTPUT);
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);

myservo.write(90);//velocidad del servo
delay(2000);
motor_stop();
}

void loop() 
{

        leer_obstaculo();// lee distancia y almacena obstaculo
        //motor_derecha();
       
}

int DISTACM()//funcion que regresa un valor entero de la distancia para su comparacion
{
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW); 
  duracion = pulseIn(ECHO, HIGH); //Medición del ancho de pulso recibido en el pin Echo
  distancia =  (float (duracion))/53;
  delay(5);
  
  return distancia;
  
  
}
////////////////////////////////////////////////
void motor_stop()
{
  analogWrite(ledPin0,0);//motor parado
  analogWrite(ledPin1,0);
  analogWrite(ledPin2,0);
  analogWrite(ledPin3,0);
}
/////////////////////////////////////////////////
void motor_adelante()
{
   analogWrite(ledPin0,0);//atras
   analogWrite(ledPin1,velocidad);
   analogWrite(ledPin2,0);
   analogWrite(ledPin3,velocidad);
}
////////////////////////////////////////////////
void motor_atras()
{
   analogWrite(ledPin0,velocidad);//adelante
   analogWrite(ledPin1,0);
   analogWrite(ledPin2,velocidad);
   analogWrite(ledPin3,0);
}
//////////////////////////////////
void motor_izquierda()//gerena un pequeño tiempo girando debe parar
{
        analogWrite(ledPin0,0);
        analogWrite(ledPin1,velocidad);
        analogWrite(ledPin2,velocidad);
        analogWrite(ledPin3,0);
   
  
}
//////////////////////////////////
void motor_derecha()//gerena un pequeño tiempo girando debe parar
{
 
        analogWrite(ledPin0,velocidad);
        analogWrite(ledPin1,0);
        analogWrite(ledPin2,0);
        analogWrite(ledPin3,velocidad);
    
}
//////////////////////////////////
void leer_obstaculo()
{
 //// leera 2 POSICIONES DERECHA E IZQUIERDA
 /////poscicion uno
 // motor_stop();//parar los motores
 CM = DISTACM();//lee la funcion de leer los centimetros
 
if(CM <= obstamax && CM >=obstamin )//verifica si hay obstaculos menos a 14 cm
        {
        /////poscicion dos
         motor_stop();
          delay(700);
          long tiempoVuelta;
        //Serial.println("Inicia lectura del radar");         
         /*;//servomotor derecha        
         CMD = DISTACM();//lee la funcion de leer los centimetrosmyservo.write(angizq);//servomotor derecha
         delay(1000);
         CMI = DISTACM();//lee la funcion de leer los centimetros*/  

         gradoDetectado=Radar();       
            
           
          if(gradoDetectado<=90)//Si el grado es igual o menor a 90 el obstaculo está en la derecha
          {
            //Serial.println(gradoDetectado);  
          tiempoVuelta=gradoDetectado * 900/90;
          //Serial.println(tiempoVuelta);
          // myservo.write(angcent);///Colocar servomotor en 90 grados
         // Serial.println("90 grados");
           delay(1000);
           motor_derecha(); 
           Serial.print(2);//Se envía la señal de que se girará a la derecha (2)
           Serial.print("|");
           Serial.println(distanciaMaxima);
           delay(tiempoVuelta);
           
          }
          else if (gradoDetectado>90)//Si el grado es mayor a 90 el osbtáculo está en la izquierda. 
          {
           // Serial.println(gradoDetectado);  
           // Serial.println("Más de 90 grados");
            tiempoVuelta=(180-gradoDetectado) * 900 / 90;
          // myservo.write(angcent);//Colocar servomotor en 90 grados
           delay(1000);
           motor_izquierda();    
           Serial.print(-2);//Se envía la señal de que se gira a la izquierda
           Serial.print("|");
           Serial.println(distanciaMaxima);
          // Serial.println(tiempoVuelta);               
           delay(tiempoVuelta);

          }
        }
//////////////////////////////////////

  else 
   {
      // myservo.write(angcent);//servomotor en 180 grados izquierda
       motor_adelante();

   }
}
 

int Radar()
{
 float cmMsec;
 // int distanciaMaxima;
 int a=0;
int dir=5;
  distanciaMaxima=0;
  int grado=0;
  String anguloDireccion="";
  
  do {
        long microsec = ultrasonic.timing();

      //calcula la distancia
      cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
      //mandar al puerto serial
      Serial.print(int(cmMsec));
      Serial.print(",");
      Serial.println(a);

      if(distanciaMaxima<cmMsec){ //Si la distancia es menor a la detectada 
        distanciaMaxima=cmMsec;
        grado=a;
      } 
      
      //mover servo
    a+=dir; // Incrementar/decrementar de 5 en 5 grados

   // if(a==0)dir=5; //Se suman 5
    if(a==180)dir=-5;//Se restan 5

  
    myservo.write(a);
 
    /*delay(300);
    servo.write(0);*/
    delay(100);
  } while (a >=0);
  anguloDireccion=distanciaMaxima;
  delay(500);
  myservo.write(90);
  return grado;
}

