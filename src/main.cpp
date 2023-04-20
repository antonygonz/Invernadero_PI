#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <EEPROM.h>

//Definicion de funciones
String Mensaje();//lee comando
String shell(String);//interpreta el comando para iniciar otras funciones
void start();//enciende la maquina
void stop();//detiene la maquina
void modgan();//cambia las ganancias 

//Variables de Funcion Mensasje
char datoLeido=1;//dato leido al momento
char ultimoDato=1;//dato anterior
String comando="";//Comando actual
String comandoAnterior="";//Comando Anterior
String shellcomando="";
int cont=0;
//Variables de shell
char carriagereturn=13;//carriage return for windows 
String startup="start";
String stopun="stop";
String nohacenada="1";



void setup()
{
  //se agrega el caracter CR a las variables de shell
  startup.concat(carriagereturn);
  stopun.concat(carriagereturn);
  nohacenada.concat(carriagereturn);
  //aqui termina

  Serial.begin(9600);//se establece la velocidad de comunicacion usb
  //Serial3.begin(115200);//se establece la velocidad de comunicacion wifi con el esp8266
  pinMode(13,OUTPUT);
  delay(500);
  Serial.println("Hola mundo ");
}

void loop() 
{
  if (Serial.available())//revisa que esten entrando datos para obtener el mensaje
  {
    shellcomando = Mensaje();
  }
  shellcomando=shell(shellcomando);
  
}

String Mensaje()
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
    //Sigiente apartado para pruebas
    //Serial.println("Comando actual es:"+comando);
    //Serial.println("Comando anterior es:"+comandoAnterior);
    //Serial.println(comando);//manda el string completo que se almaceno
    //cont=0; //contador para obtener strings de misma longitud
    comandoAnterior=comando;//copia el comando actual al anterior
    comando="";//resetea la variable del string
    Serial.println("comando: "+comandoAnterior); 
    return comandoAnterior;
  }
  ultimoDato=datoLeido;//iguala los datos para la comparacion del siguiente ciclo
  return"";
}
String shell(String debugcomand)
{
  
  if (debugcomand.equals(nohacenada))
  {
    delay(500);
    Serial.println("No hace nada");
    debugcomand="";
  }
  else if (debugcomand.equals(startup))
  {
    delay(500);
    Serial.println("Comenzando");
    debugcomand="";
  }else if (debugcomand.equals(stopun))
  {
    delay(500);
    Serial.println("finalizando");
    debugcomand="";
  }
  return debugcomand; 
  
  /*else
  {//apartado de pruebas para visualizar strings
    delay(500);
    Serial.println("debugcomand");
    for (size_t i = 0; i < debugcomand.length(); i++)
    {
      
      Serial.print((String)i+" letra= ");
      Serial.print((int)debugcomand[i]);
      Serial.print("\n");
    }
    delay(500);
    Serial.println("start");
    for (size_t i = 0; i < startup.length(); i++)
    {
      
      Serial.print((String)i+" letra= ");
      Serial.print((int)startup[i]);
      Serial.print("\n");
    }
  }
  */
   
  
}