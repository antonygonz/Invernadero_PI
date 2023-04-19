#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <EEPROM.h>

//Definicion de funciones
void Mensaje();


//Variables de Funcion Mensasje
char nombre=1;
char valor=1;
String comun="";
int cont=0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial3.begin(115200);
  pinMode(13,OUTPUT);
  delay(500);
  Serial.println("Hola mundo ");
}

void loop() 
{
  Mensaje();

}

void Mensaje()
{
nombre = (char)Serial.read();

  if (nombre!=valor)
  {
    if (nombre==-1)
    {
      /* code */
    }
    else{
    Serial.print(nombre);
    comun = comun + nombre;
    cont++;
    }
    
  }
  if (cont==5)
  {
    Serial.print("\n");
    Serial.println(comun);
    cont=0;
    comun="";
  }
  
  valor=nombre;
  //delay(500);

  return;
}