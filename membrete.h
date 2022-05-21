//**************************************************************************************************************************************
//Programa 6: Archivo de cabecera membrete tiempo de ejecucion
//Autor: J. Quintero P.
//Ejecución: PC compatible con SO Linux/Windows arquitectura de 64/32 bits
//Versión: 0
//Fecha: Noviembre, 2021
//Hora: 21:17H
//**************************************************************************************************************************************
#include <iostream>
#ifndef MEMBRETE_H
#define MEMBRETE_H
using namespace std; 
class membrete{
	public:
		void mostrar(){
			cout<<"!------------------------------------------------------------------------!"<<endl;
			cout<<"!***********          Scaner cliente Modbus TCP/IP            ***********!"<<endl;
			cout<<"!***********                  Julian210                       ***********!"<<endl;
			cout<<"!------------------------------------------------------------------------!"<<endl;
		}
};
#endif
