//******************************************************************************************
//Programa: ChaModbus
//Descripción: Escaner protocolo Modbus TCP/IP modelo cliente para pruebas de comunicación
//Descripción: Offset de una dirección por alias de comunicación
//Descripción: Operación en esta versión solo para valores de registros de 16bits sin signo
//Autor: J. Quintero P.
//Ubicacion: Guatire - Venezuela
//Ejecución: Sistemas operativos Linux 32/64 bits 
//Conexion Modbus: Solo para servidores Modbus TCP con arquitectura de 32 bits
//Versión: 2.3.05
//Fecha: Abril, 2023
//Hora: 13:55H
//******************************************************************************************
#include "modbus.h" //Archivo de cabecera para apertura y cierre de puertos así como creación de FC(XY) y eFC Modbus(XY)
#include "piepagina.h" //Archivo de cabecera para mostrar pie de pagina al final del algoritmo
#include "membrete.h" //Archivo de cabecera para mostrar membrete del algoritmo
#include <string> //Se incluye librería de cpp para manipular datos en formato cadena de caracter
#include <iomanip> //Manipulador de flujos parametrizados
#include <iostream> //Se incluye librería de cpp para manejar entrada y salida de datos
//******************************************************************************************
#define Leer_bobinas 1
#define Leer_bits 2
#define Leer_registros_HR 3
#define Leer_registros_IR 4
#define Escribir_bobina 5
#define Escribir_registro_HR 6
#define Escribir_bobinas 7
#define Escribir_registros_HR 8 
//******************************************************************************************
using std::cout; //Uso de las funciones del iostream para imprimir datos en consola
using std::cin; //Uso de las funciones del iostream para recibir datos desde consola
using std::endl; //Terminador de linea con retorno de carril - consola
using std::setw; //Se imprime los valores de salida en un ancho especifico 
//*** Declaracion e inicializacion de variables globales ***
uint16_t FC03_Dir=0,FC03_Offset=0;
uint16_t FC04_Dir=0,FC04_Offset=0;
uint16_t FC05_Dir=0;
uint16_t FC06_Dir=0;
uint16_t FC0F_Dir=0,FC0F_Offset=0;
uint16_t FC10_Dir=0,FC10_Offset=0;
uint16_t FC01_Offset=0,FC01_Dir=0;
uint16_t FC02_Offset=0,FC02_Dir=0;
uint8_t *msg_fc01;
int UnitID=1,v1=0,v2=0,v3=0,v4=0,v5=0,v6=0,v7=0,seleccion_menu=0,eb=0,er=0,fc01=0x01;
string continuar,dirIP;
bool badUid=false;
//******************************************************************************************
void ContinuarMinusculas(string &seguir){
	for (int aux=0;aux<seguir.length();aux++){
		seguir[aux]=tolower(seguir[aux]);
	}
}
void MenuPrincipal(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú principal                    ***********!"<<endl; 
	cout<<"!***********      Funciones Modbus TCP/IP disponibles         ***********!"<<endl;
	cout<<"!***********         1.  FC-01 Leer Bobinas  (0X)             ***********!"<<endl;
	cout<<"!***********         2.  FC-02 Leer Bits     (1X)             ***********!"<<endl;
	cout<<"!***********         3.  FC-04 Leer Registros HR (4X)         ***********!"<<endl;
	cout<<"!***********         4.  FC-03 Leer Registros IR (3X)         ***********!"<<endl;
	cout<<"!***********         5.  FC-05 Escribir Bobina (0X)           ***********!"<<endl;
	cout<<"!***********         6.  FC-06 Escribir Registro HR (4X)      ***********!"<<endl;
	cout<<"!***********         7.  FC-0F Escribir Bobinas (0X)          ***********!"<<endl;
	cout<<"!***********         8.  FC-10 Escribir Registros HR (4X)     ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
	
	}
void MenuFC01(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-01                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de consulta            ***********!"<<endl;
	cout<<"!***********      2. Cantidad total de bobinas a leer         ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC02(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-02                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de consulta            ***********!"<<endl;
	cout<<"!***********      2. Cantidad total de bits a leer            ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC03(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-03                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de consulta            ***********!"<<endl;
	cout<<"!***********      2. Cantidad total de registros IR a leer    ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC04(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-04                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de consulta            ***********!"<<endl;
	cout<<"!***********      2. Cantidad total de registros HR a leer    ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC05(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-05                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección de bobina a escribir           ***********!"<<endl;
	cout<<"!***********      2. Valor a setear en bobina                 ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC06(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-06                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección del registro a escribir        ***********!"<<endl;
	cout<<"!***********      2. Valor a setear en el registro            ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC0F(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-0F                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de las bobinas         ***********!"<<endl;
	cout<<"!***********      2. Cantidad de bobinas a escribir           ***********!"<<endl;
	cout<<"!***********      3. Valores a escribir en cada bobina        ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}
void MenuFC10(){
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<"!***********                Menú FC-10                        ***********!"<<endl;
	cout<<"!***********   Cuando se le solicite en pantalla introduzca:  ***********!"<<endl;
	cout<<"!***********      1. Dirección inicial de los registros HR    ***********!"<<endl;
	cout<<"!***********      2. Cantidad de registros HR a escribir      ***********!"<<endl;
	cout<<"!***********      3. Valores a escribir en cada registro      ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
}		
//******************************************************************************************
int main(int argc, char **argv)
{
	membrete JQ216_Membrete; //Creacion del objeto membrete
	JQ216_Membrete.mostrar(); //Se llama a la propiedad mostrar del objeto
	cout<<"!***********           Configuracion principal                ***********!"<<endl;
	cout<<"!***********       Conexion a servidor Modbus TC/IP           ***********!"<<endl;
	cout<<"!------------------------------------------------------------------------!"<<endl;
	cout<<endl;
	cout<<"0. Digite direccion IP del servidor destino: ";
	cin>>dirIP;
	cout<<"1. Digite ID del servidor destino: ";
	do{
		cin>>UnitID;
		if (UnitID <= 0 or UnitID > 255){
			badUid=true;
			cout<<endl;
			cout<<"El ID debe estar en un rango entre 1 y 255"<<endl;
			cout<<"1. Digite ID del servidor destino: ";
		}
		else{
			badUid=false;
        }
    }while (badUid==true);
	cout<<endl;
	system("clear");
    do{
		eb=0; //Se coloca esta variable entera en 0 para normalizar ultimo bucle do-while
		system("clear"); //Esta instruccion se ejecuta si se repite el bluce do-while
		MenuPrincipal(); //Llamada a funcion Menu Principal
		cout<<endl;
		cout<<"Por favor digite la funcion Modbus-TCP/IP a ejecutar: ";
		cin>>seleccion_menu; 
		modbus mb = modbus(dirIP, 502); //Creacion de objeto Modbus TCP
		mb.modbus_set_slave_id(UnitID); //Asignacion ID 
		mb.modbus_connect(); //Conexion con el servidor
		system("clear");
		switch(seleccion_menu){
			case Leer_bobinas:{ //FC-01 - Leer bobinas (0X)
				//FC-01 - Leer Bobinas (0X)
				bool FC01_Val[v2];
				MenuFC01(); //Llamada a funcion Menu FC01
				cout<<endl;
				cout<<"1. Introduzca direccion inicial de consulta 0X: ";
				cin>>FC01_Dir;
				cout<<"2. Indique la cantidad de bobinas a leer: ";
				cin>>FC01_Offset;
				mb.modbus_read_coils(FC01_Dir, FC01_Offset, &FC01_Val[v2]);
				if (mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else{
					for(int l=0;l<4;l++){
						cout<<endl;
						cout<<"----------------------------------------"<<endl;
						cout<<"       Resultados de la funcion 0X"<<endl;
						cout<<"Dir"<<setw(9)<<"Estado"<<setw(9)<<endl;
						for(int j=0;j<FC01_Offset;j++){
							cout<<setw(2)<<(j+FC01_Dir)<<setw(7)<<FC01_Val[j]<<endl;
						}
						cout<<"----------------------------------------"<<endl;
						v2=0;
						sleep(2); //Retardo para ejecución de bucle for por 2 segundos
					}
				}	
			}break;
			case Leer_bits:{ //FC-02 - Leer bits (1X)
				//FC-02 - Leer Bits (1X)
				bool FC02_Val[v3];
				MenuFC02(); //Llamada funcion Menu FC02
				cout<<endl;
				cout<<"1. Introduzca direccion inicial de consulta 1X: ";
				cin>>FC02_Dir;
				cout<<"2. Indique la cantidad de bobinas a leer: ";
				cin>>FC02_Offset;
				mb.modbus_read_input_bits(FC02_Dir, FC02_Offset, &FC02_Val[v3]);
				if(mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;			
				}
				else {
					for(int a=0;a<4;a++){
						cout<<endl;
						cout<<"----------------------------------------"<<endl;
						cout<<"       Resultados de la funcion 1X"<<endl;
						cout<<"Dir"<<setw(9)<<"Estado"<<setw(9)<<endl;
						for(int i=0;i<FC02_Offset;i++){
							cout<<setw(2)<<(i+FC02_Dir)<<setw(7)<<FC02_Val[i]<<endl;
						}
						cout<<"----------------------------------------"<<endl;
						v3=0;
					sleep(2); //Retardo para ejecución de bucle for por 2 segundos
					}
				}	
			}break;
			case Leer_registros_HR:{ //FC-04 - Leer registros holding (4X)
				//FC-04 - Leer registros holding (4X)
				uint16_t FC04_Val[v4];
				MenuFC04(); //Llamada funcion Menu FC04
				cout<<endl;
				cout<<"1. Introduzca direccion inicial de consulta 4X: ";
				cin>>FC04_Dir;
				cout<<"2. Indique la cantidad de registros HR a leer: ";
				cin>>FC04_Offset;
				mb.modbus_read_holding_registers(FC04_Dir, FC04_Offset, &FC04_Val[v4]);
				if (mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else{ 
					for(int r=0;r<4;r++){
					//mb.modbus_read_holding_registers(FC04_Dir, FC04_Offset, &FC04_Val[v4]);
					cout<<endl;
					cout<<"----------------------------------------"<<endl;
					cout<<"       Resultados de la funcion 4X"<<endl;
					cout<<"Dir"<<setw(9)<<"Valor"<<setw(9)<<endl;
						for(int a=0;a<FC04_Offset;a++){
							cout<<setw(2)<<(a+FC04_Dir)<<setw(9)<<FC04_Val[a]<<endl;
						}	
					cout<<"----------------------------------------"<<endl;
					v4=0;
					sleep(2); //Retardo para ejecución de bucle for por 2 segundos
					}
				}	
			}break;
			case Leer_registros_IR:{ //FC-03 - Leer registros de entrada (3X)
				//FC-03 - Leer registros de entrada (3X)
				uint16_t FC03_Val[v5];
				MenuFC03(); //Llamada funcion Menu FC03
				cout<<endl;
				cout<<"1. Introduzca direccion inicial de consulta 3X: ";
				cin>>FC03_Dir;
				cout<<"2. Indique la cantidad de registros HR a leer: ";
				cin>>FC03_Offset;
				mb.modbus_read_input_registers(FC03_Dir, FC03_Offset, &FC03_Val[v5]);
				if(mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else {
					for(int h=0;h<4;h++){
						//mb.modbus_read_input_registers(FC03_Dir, FC03_Offset, &FC03_Val[v5]);
						cout<<endl;
						cout<<"----------------------------------------"<<endl;
						cout<<"       Resultados de la funcion 3X"<<endl;
						cout<<"Dir"<<setw(9)<<"Valor"<<setw(9)<<endl;
						for(int b=0;b<FC03_Offset;b++){
							cout<<setw(2)<<(b+FC03_Dir)<<setw(9)<<FC03_Val[b]<<endl;
					}
						cout<<"----------------------------------------"<<endl;
						v5=0;
						sleep(2); //Retardo para ejecución de bucle for por 2 segundos
					}
				}	
			}break;
			case Escribir_bobina:{ //FC-05 - Escribir bobina (0X)
				//FC-05 - Escribir Bobina (0X)                	
				bool FC05_Val;
				MenuFC05(); //Llamada funcion Menu FC05
				cout<<endl;
				cout<<"1. Introduzca direccion 0X: ";
				cin>>FC05_Dir;
				cout<<"2. Indicar accion (1:True / 0:False): ";
				cin>>FC05_Val;
				if(mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else{
					mb.modbus_write_coil(FC05_Dir, FC05_Val);
				}
			}break;
			case Escribir_registro_HR:{ //FC-06 - Escribir registro holding (4X)
				//FC-06 - Escribir Registro (4X)
				uint16_t FC06_Val;
				MenuFC06(); //Llamada funcion Menu FC06
				cout<<endl;
				cout<<"1. Introduzca direccion 4X: ";
				cin>>FC06_Dir;
				cout<<"2. Indicar valor registro: ";
				cin>>FC06_Val;
				if(mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else {
				mb.modbus_write_register(FC06_Dir, FC06_Val);
				}
			}break;
			case Escribir_bobinas:{ //FC-0F - Escribir multiples bobinas (0X)
				//FC-0F - Escoribir Multiples Bobinas (0X)
				bool FC0F_Val[v6]; // Arreglo para escribir datos FC
				bool FC0F_Est; // Valor de las direcciones a escribir en arreglo
				MenuFC0F(); //Llamada funcion Menu FC0F
				cout<<"1. Introduzca la direccion 0X: ";
				cin>>FC0F_Dir;
				cout<<"2. Ingrese la cantidad de bobinas a escribir: ";
				cin>>FC0F_Offset;
				for(int z=0;z<FC0F_Offset;z++){
					cout<<"3. Ingrese el "<<z+1<<" valor de la bobina (1:True/0:False): ";
					cin>>FC0F_Est;
					FC0F_Val[z]=FC0F_Est;
					if(mb.err){
						cout<<endl;
						cout<<"Codigo de error: "<<mb.error_msg<<endl;
					}
					else{
						mb.modbus_write_coils(FC0F_Dir, FC0F_Offset, FC0F_Val);
					}
				}
				/*if(mb.err){
					cout<<endl;
					cout<<"Codigo de error: "<<mb.error_msg<<endl;
				}
				else{	
					mb.modbus_write_coils(FC0F_Dir, FC0F_Offset, FC0F_Val);
				}*/
			}break;
			case Escribir_registros_HR:{ //FC-10 - Escribir multiples registros holding (4X)
				//FC-10 - Escribir Multiples Registros (4X)
				uint16_t FC10_Val[v7];
				uint16_t FC10_Est;
				MenuFC10(); //Llamada funcion Menu FC10
				cout<<"1. Introduzca la direcion 4X: ";
				cin>>FC10_Dir;
				cout<<"2. Ingrese la cantidad de registros a escribir: ";
				cin>>FC10_Offset;
				for(int w=0;w<FC10_Offset;w++){
					cout<<"3. Ingrese el "<<w+1<<" valor del registro HR: ";
					cin>>FC10_Est;
					FC10_Val[w]=FC10_Est;
					if(mb.err){
						cout<<endl;
						cout<<"Codigo de error: "<<mb.error_msg<<endl;	
					}
					else{
						mb.modbus_write_registers(FC10_Dir, FC10_Offset, FC10_Val);	
					}
				}
			}break;
		}
		mb.modbus_close(); //Se cierra la conexion al servidor y se limpia la memoria almacenada en buffer de datos
		cout<<endl;
		do{ //Este do-while se ejecuta en caso el usuario presione otra tecla distinta a s o n minuscula para continuar o salir del programa
			cout<<"Desea efectuar otra consulta Modbus (s/n): ";
			cin>>continuar;
			ContinuarMinusculas(continuar);
			if (continuar=="s"){
				eb=1; // Variable auxiliar para la ejecución de la instrucción continuar en 1
			}
			else if(continuar=="n"){
				eb=2; // Variable auxiliar para la ejecución de la instrucción salir en 2
			}
			else if (continuar!="n" or continuar!="s"){
				cout<<"Debe digitar (n) para salir o (s) para continuar"<<endl;
				cout<<endl;
				eb=3; // Variable auxiliar para la ejecución de la instrucción indeterminado en 3
			}
		}while(eb==3); // si el valor del auxiliar es indeterminado se solicita nuevamente las opciones permitidas
	}while(eb==1);
	//}while(continuar.compare("S")==0 or continuar.compare("s")==0); //Si el usuario presionó "S" o "s", el bucle do-while se vuelve a ejecutar.
    system("clear");
    piepagina J216_pieDpagina; //Creacion del objeto pie de pagina
	J216_pieDpagina.mostrar(); //Se llama a la propiedad mostrar del objeto
    return 0;
}
