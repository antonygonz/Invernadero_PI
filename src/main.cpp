#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <EEPROM.h>

//Definicion de funciones
void Mensaje();


//Variables de Funcion Mensasje
char datoLeido=1;//dato leido al momento
char ultimoDato=1;//dato anterior
String comando="";//Comando actual
String comandoAnterior="";//Comando Anterior
int cont=0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);//se establece la velocidad de comunicacion usb
  //Serial3.begin(115200);//se establece la velocidad de comunicacion wifi con el esp8266
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
datoLeido = (char)Serial.read();

  if (datoLeido!=ultimoDato)//verifica que la ultima lectura no sea el mismo dato
  {//aunque realmente verifica q
    if (datoLeido!=-1)//verifica que el dato no sea levantamiento de tecla puede ocasionar error a futuro
    {
      Serial.print(datoLeido);//muestra el ultimo dato en entrar
      if (datoLeido!=10)//evita agragar un enter al string
      {
        comando = comando + datoLeido;//agrega el ultimo dato al string
      }
      //cont++;//suma datos al contador actualmente reemplazado por el enter
    }
  }
  if (datoLeido==10)//verifica que el dato leido sea enter para terminar la comunicacion 
  {
    //Serial.print("\n");//agrega un salto de linea para diferenciar de la linea anterior
    
    Serial.println(comando);//manda el string completo que se almaceno
    //cont=0; //contador para obtener strings de misma longitud
    comandoAnterior=comando;//copia el comando actual al anterior
    comando="";//resetea la variable del string comando
  }
  ultimoDato=datoLeido;//iguala los datos para la comparacion del siguiente ciclo
  //creo que quitar la ultima linea generara mas errores asi que la voy a dejar

  //delay(500);

  return;
}