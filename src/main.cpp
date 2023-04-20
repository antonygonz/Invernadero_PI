#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

//Definicion de funciones
String Mensaje();//lee comando
String shell(String);//interpreta el comando para iniciar otras funciones
void start();//enciende la maquina
void stop();//detiene la maquina
void modgan();//cambia las ganancias 
void temp280();

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
bool working=false;//checa si el sitema esta trabajando
String nohacenada="1";
String modganancia="modgan";
String readtemperatura="readtemp";
String coling="cool";
//configuraciones del sensor de temperatura bmp280
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();//funcion del objeto bmp(tipo:Adafruit_BMP280)para leer temperatura
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();//funcion del objeto bmp(tipo:Adafruit_BMP280)para leer presion
bool SensorDig=true;//verdadero para bmp280, falso para ads1115

void setup()
{
  //se agrega el caracter CR a las variables de shell
  startup.concat(carriagereturn);
  stopun.concat(carriagereturn);
  nohacenada.concat(carriagereturn);
  modganancia.concat(carriagereturn);
  readtemperatura.concat(carriagereturn);
  coling.concat(carriagereturn);
  //aqui termina

  Serial.begin(9600);//se establece la velocidad de comunicacion usb
  //Serial3.begin(115200);//se establece la velocidad de comunicacion wifi con el esp8266
  pinMode(13,OUTPUT);
  delay(500);
  Serial.println("Hola mundo ");
  //apartado del sensor bmp280
  if (SensorDig)
  {
    //configuraciones del sensor bmp280
    Serial.println(F("BMP280 Sensor event test"));
    unsigned status;
    status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
    if (!status)//si no encuentra el sensor en el puerto i2c
    {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                        "try a different address!"));
      Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
      /* Default settings from datasheet. */
      bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

      bmp_temp->printSensorDetails();
    }
  }
  //fin del apartado del sensor 


}

void loop() 
{
  if (Serial.available())//revisa que esten entrando datos para obtener el mensaje
  {
    shellcomando = Mensaje();
  }
  shellcomando=shell(shellcomando);//adquiere el comando y resetea el comando para una unica ejecuccion
  
  
  
}

String Mensaje()
{
datoLeido = (char)Serial.read();
  //if (datoLeido!=ultimoDato)//verifica que la ultima lectura no sea el mismo dato//actualmente desactivado
  //{//aunque realmente verifica q
    if (datoLeido!=-1)//verifica que el dato no sea levantamiento de tecla puede ocasionar error a futuro
    {
      Serial.print(datoLeido);//muestra el ultimo dato en entrar
      if (datoLeido!=10)//evita agragar un enter al string
      {
        comando = comando + datoLeido;//agrega el ultimo dato al string
      }
      //cont++;//suma datos al contador actualmente reemplazado por el enter
    }
  //}
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
  //delay(500);
  if (debugcomand.equals(nohacenada))
  {
    Serial.println("No hace nada");
    debugcomand="";
  }
  else if (debugcomand.equals(startup))
  {
    working=true;
    Serial.println("Comenzando");
    debugcomand="";
  }else if (debugcomand.equals(stopun))
  {
    working=false;
    Serial.println("finalizando");
    debugcomand="";
  }else if (debugcomand.equals(modganancia))
  {
    Serial.println("cambio de ganancias");
    debugcomand="";
  }else if (debugcomand.equals(readtemperatura))
  {
    if (SensorDig)//verdadero para sensor bmp280
    {
      temp280();
    }else{}//falso para sensor ads1115 codigo en desarrollo}
    debugcomand="";
  }else if (debugcomand.equals(coling))
  {
    if (!working)
    {
      Serial.println("Enfriando");
      //codigo para enfriar en el futuro
    }else
    {
      Serial.println("Espere a que se apage el sistema");
    }
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
void temp280()
{
sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  //delay(2000);  

}
