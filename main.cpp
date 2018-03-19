//
//  main.cpp
//  ProyectoAutomata
//
//  Created by Mac on 2/17/18.
//  Copyright © 2018 Mac. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>


using namespace std;

struct Arista {
    string simbolo;
    int estado;
    
public:
    Arista(int e, string s){
        simbolo = s;
        estado = e;
    }
};

struct Estado {
    string name;
    vector<Arista *> transicionesArr;
    vector<Arista *> transicionesNFAArr;
    int cant;
    int cantNFA;
    bool aceptacion;
    
public:
    Estado(string n){
        name = n;
        cant = 0;
        aceptacion = false;
    }
    
    void addToTransArray(Arista * n){
        transicionesArr.push_back(n);
        cant++;
    }
    
    void addToTransNFAArray(Arista * n){
        transicionesNFAArr.push_back(n);
        cantNFA++;
    }
    
    Arista * getTransicion(int n){
        return transicionesArr.at(n);
    }
    
};

struct Automata {
    vector<Estado*> estadosArr;
    vector<Estado*> estadosJuntadosArr;
    Estado * inicial;
    vector<Estado*> aceptacionArr;
    vector<string> simbolosArr;
    int cantA;
    int cantE;
    
public:
    Automata(){cantA = 0; cantE=0;}
    
    vector<Estado*> estadosNFAtoDFA;
    
    
    void simbolosPossibles(int cant, vector<string> simbs){
        for(int a = 0; a<cant; a++){
            simbolosArr.push_back(simbs[a]);
        }
    }
    
    void setEstadoIni(Estado * eIni){
        inicial = eIni;
    }
    
    void setEstadosFin(vector<Estado*> esf){
        int size = esf.size();
        for(int a = 0; a<size; a++){
            aceptacionArr.push_back(esf[a]);
        }
        
        for(int b = 0; b<size; b++){
            esf.at(b)->aceptacion = true;
        }
    }
    
    Estado * getEstado(int pos){
        return estadosArr.at(pos);
    }
    
    
    void printArray(){
        cout<<"ESTADOS ARRAY"<<endl;
        for(int a = 0; a<estadosArr.size(); a++){
            if(estadosArr.at(a)){
                cout<<"Inicial: "<<estadosArr.at(a)->name<<endl;
            } else if(estadosArr.at(a)){
                cout<<"Aceptacion: "<<estadosArr.at(a)->name<<endl;
            } else{
                cout<<"Normal: "<<estadosArr.at(a)->name<<endl;
            }
        }
    }
    void printDFA(){
        int canti = estadosArr.size();
        for(int x = 0;x<canti;x++){
            cout<<getEstado(x)->name + " tiene transiciones hacia: ";
            for(int z = 0;z<getEstado(x)->cant;z++)
            {
                cout<<getEstado(x)->getTransicion(z)->estado;
                cout<<" con ";
                cout<<getEstado(x)->getTransicion(z)->simbolo;
                if(z != (getEstado(x)->cant - 1))
                    cout<<" | ";
            }
            
            cout<<""<<endl;
        }
    }
    
    bool checkIfCadenaValidaDFA(char cadena[]){
        int x = strlen(cadena); //o strlen!!
        int y = simbolosArr.size();
        Estado * temp = inicial;
        
        for(int a = 0; a<x; a++){
            for(int b = 0; b<y; b++){
                stringstream ss;
                string sig;
                char current = cadena[a];
                ss<<current;
                ss>>sig;
                
                if(temp->getTransicion(b)->simbolo == sig){
                    temp = getEstado(temp->getTransicion(b)->estado);
                }
            }
        }
        
        int size = aceptacionArr.size();
        for(int a = 0; a<size; a++){
            if(aceptacionArr.at(a)->name == temp->name)
                return true;
        }
        return false;
    }
    
    
    bool checkIfCadenaValidaDFA2(string cadena){
        Estado * temp = inicial;
        if(doDFA(temp,inicial->transicionesArr,cadena,0) == 0){
            return true;
        } else{
            return false;
        }
        
    }
    
    int doDFA(Estado * temp,vector<Arista*> trans, string cadena, int cp){
        int cont = 0;
        if(cp == cadena.size()){
            if(temp->aceptacion)
                return 0;
            else
                return 1;
        }
        for(cont = 0;cont<trans.size();cont++){
            int x = -1;
            Arista * tempA = trans.at(cont);
            string n = tempA->simbolo;
            if(n.at(0) == cadena.at(cp)){
                x = doDFA(getEstado(tempA->estado),getEstado(tempA->estado)->transicionesArr,cadena,cp+1);
                if(x == 3){
                    continue;
                } else if(x==1){
                    continue;
                } else if(x ==0){
                    return x;
                }
            }
        }
        return 2;
    }
    
    int NFAtoDFA(int cantS, int cantE){
        
       vector<string> names;
        int cont[cantS];
        int numEstado[cantS];
        string name[cantS];
        
        bool flag = false;
        
        for(int x = 0; x<cantE;x++){
            flag = false;
            for(int y = 0;y<cantS;y++){
                cont[y] = 0;
                name[y] ="";
            }
            
            for(int y = 0;y<cantS;y++){
                for(int z = 0;z<getEstado(x)->cant;z++){
                    if(getEstado(x)->getTransicion(z)->simbolo == simbolosArr.at(y)){
                        name[y] = name[y] + to_string(getEstado(x)->getTransicion(z)->estado);
                        numEstado[y] = x;
                        cont[y]++;
                    }
                }
            }
            
            //Verifica que tenga mas de 2 transiciones
            for(int y = 0;y<cantS;y++){
                if(cont[y] > 1)
                    flag = true;
            }
            
            //Verifica que el estado ya no existe, si existe lo quita
            for(int y = 0;y<names.size();y++){
                if(names.at(y)==name[y]){
                    name[y]="";
                }
            }
            
            if(flag){
                Estado * estado_A = new Estado(getEstado(x)->name);
                estadosNFAtoDFA.push_back(estado_A);
                for(int y = 0;y<cantS;y++){
                    Arista * tempA = new Arista(numEstado[y],simbolosArr.at(y));
                    estado_A->addToTransArray(tempA);
                    
                    if(names.size()==0){
                        Estado * estado_B = new Estado(name[y]);
                        names.push_back(name[y]);
                        estadosNFAtoDFA.push_back(estado_B);
                    }
                    else if(names.size()>0){
                        for(int j = 0;j<names.size();j++){
                            if(name[j]!=names.at(j)){
                                Estado * estado_B = new Estado(name[y]);
                                names.push_back(name[y]);
                                estadosNFAtoDFA.push_back(estado_B);
                            }
                        }
                        
                    }
                }
            }
        }
        
        //Solo imprime para verificar
        for(int x = 0;x<estadosNFAtoDFA.size();x++){
            cout<<"Estados Cambiados: "<<estadosNFAtoDFA.at(x)->name<<endl;
        }
        return 0;
    }
    
    
};

int main() {
    
    Automata * adminAuto = new Automata();
    
    int query;
    do{

        cout<<"1- DFA"<<endl;
        cout<<"2- NFA"<<endl;
        cout<<"3- NFA-e"<<endl;
        cout<<"Que desea ingresar: ";
        cin>>query;
        
        switch (query) {
            case 1: {
                int cantEstados;
                int cantSimbolos;
                vector<string> simb;
                string **tabla; //solo para imprimir para enseñar al usuario de su automata
                
                cout<<"Cantidad de estados: ";
                cin>>cantEstados;
                cout<<"Cuantos simbolos agregaria: ";
                cin>>cantSimbolos;
                string s;
                cout<<"Cuales serian los simbolos: ";
                for(int a = 0; a<cantSimbolos; a++){
                    cin>>s;
                    simb.push_back(s);
                }
                adminAuto->simbolosPossibles(cantSimbolos,simb); //add al alfabeto
                
                
                tabla = new string*[cantEstados+1];
                for(int a = 0; a<cantEstados+1; a++){
                    tabla[a] = new string[cantSimbolos+1];
                }
                
                Estado * nuevo;
                
                //PARA CONSTRUIR LA TABLA EN EL EXTERIOR
                tabla[0][0] = "-";
                
                for(int a = 1; a<=cantEstados; a++){
                    tabla[a][0] = to_string(a-1);
                    nuevo = new Estado(to_string(a-1));
                    adminAuto->estadosArr.push_back(nuevo); //add to vector del automata
                }
                for(int b = 1; b<=cantSimbolos; b++){
                    tabla[0][b] = simb[b-1];
                }
                
                //PARA AGREGAR VALORES DENTRO DE LA TABLA
                for(int a = 1; a<=cantEstados; a++){
                    for(int b = 1; b<=cantSimbolos; b++){
                        tabla[a][b] = "-";
                    }
                }
                
                //PRINT TABLA PARA ENSEÑAR A USUARIO
                for(int a = 0; a<cantEstados+1; a++){
                    for(int b = 0; b<cantSimbolos+1; b++){
                        if(b!=cantSimbolos){
                            cout<<tabla[a][b]<<' ';
                        } else{
                            
                            cout<<tabla[a][b]<<endl;
                        }
                        
                    }
                }
                
                //SETTING ESTADOS FINALES E INICIAL
                int estadoIni;
                int ef;
                vector<Estado *>estadoFin;
                
                cout<<"Estado inicial: ";
                cin>>estadoIni;
                
                Estado * ini = adminAuto->getEstado(estadoIni);
                adminAuto->setEstadoIni(ini);
                cout<<"Cuantos estados finales serian? ";
                cin>>ef;
                cout<<"Estado(s) final(es): ";
                int temp;
                for(int a = 0; a<ef; a++){
                    cin>>temp;
                    Estado * fin = adminAuto->getEstado(temp);
                    estadoFin.push_back(fin);
                }
                
                adminAuto->setEstadosFin(estadoFin);
                
                    //CREANDO EL DFA
                    vector<int> estadoSig;
                    int e;
                    int cantidadEstados;
                    for(int a = 1; a<=cantEstados; a++){
                        for(int b = 1; b<=cantSimbolos; b++){
                            cout<<"Para el estado "<<to_string(a-1)<<", el simbolo "<<"'"<<simb[b-1]<<"'"<<" ira hacia el estado: ";
                            cin>>e;
                           
                            Arista * nuevaArista = new Arista(e, simb[b-1]); //aqui para abajo se va creando en dfa
                            adminAuto->getEstado(a-1)->addToTransArray(nuevaArista);
                            tabla[a][b] = to_string(e);
                        }
                    }

                //PRINT TABLA PARA ENSEÑAR A USUARIO
                for(int a = 0; a<cantEstados+1; a++){
                    for(int b = 0; b<cantSimbolos+1; b++){
                        if(b!=cantSimbolos){
                            cout<<tabla[a][b]<<' ';
                        } else{
                            
                            cout<<tabla[a][b]<<endl;
                        }
                        
                    }
                }
                
                
                //LEER CADENAS DEL ARCHIVO!!!
                string filename;
                
                string cadena;
                char * cad;
                cout<<"Ingrese nombre del archivo: ";
                cin>>filename;
                ifstream inFile(filename);
                
                while(inFile>>cadena){
                    cad = new char[cadena.length()];
                    
                    for(int a = 0; a<cadena.length(); a++){
                        cad[a] = cadena.at(a);
                    }
                    bool cc = adminAuto->checkIfCadenaValidaDFA(cad);
                    if(cc){
                        cout<<"Cadena "<<"'"<<cadena<<"'"<<" es aceptada"<<endl;
                    } else{
                        cout<<"Cadena "<<"'"<<cadena<<"'"<<" no es aceptada"<<endl;
                    }
                    
                }
                inFile.close();
                
                for(int a = 0; a<adminAuto->estadosArr.size(); a++){
                    adminAuto->estadosArr.at(a)->transicionesArr.clear();
                }
                break;
            }
            case 2: {
                int cantEstados2;
                int cantSimbolos2;
                vector<string> simb2;
                string **tabla2; //solo para imprimir para enseñar al usuario de su automata
                
                cout<<"Cantidad de estados: ";
                cin>>cantEstados2;
                cout<<"Cuantos simbolos agregaria: ";
                cin>>cantSimbolos2;
                string s2;
                cout<<"Cuales serian los simbolos: ";
                for(int a = 0; a<cantSimbolos2; a++){
                    cin>>s2;
                    simb2.push_back(s2);
                }
                adminAuto->simbolosPossibles(cantSimbolos2,simb2); //add al alfabeto
                
                
                tabla2 = new string*[cantEstados2+1];
                for(int a = 0; a<cantEstados2+1; a++){
                    tabla2[a] = new string[cantSimbolos2+1];
                }
                
                Estado * nuevo2;
                
                //PARA CONSTRUIR LA TABLA EN EL EXTERIOR
                tabla2[0][0] = "-";
                
                for(int a = 1; a<=cantEstados2; a++){
                    tabla2[a][0] = to_string(a-1);
                    nuevo2 = new Estado(to_string(a-1));
                    adminAuto->estadosArr.push_back(nuevo2); //add to vector del automata
                }
                for(int b = 1; b<=cantSimbolos2; b++){
                    tabla2[0][b] = simb2[b-1];
                }
                
                //PARA AGREGAR VALORES DENTRO DE LA TABLA
                for(int a = 1; a<=cantEstados2; a++){
                    for(int b = 1; b<=cantSimbolos2; b++){
                        tabla2[a][b] = "-";
                    }
                }
                
                //SETTING ESTADOS FINALES E INICIAL
                int estadoIni2;
                int ef2;
                vector<Estado *>estadoFin2;
                
                cout<<"Estado inicial: ";
                cin>>estadoIni2;
                
                Estado * ini2 = adminAuto->getEstado(estadoIni2);
                adminAuto->setEstadoIni(ini2);
                cout<<"Cuantos estados finales serian? ";
                cin>>ef2;
                cout<<"Estado(s) final(es): ";
                int temp2;
                for(int a = 0; a<ef2; a++){
                    cin>>temp2;
                    Estado * fin = adminAuto->getEstado(temp2);
                    estadoFin2.push_back(fin);
                }
                
                adminAuto->setEstadosFin(estadoFin2);
                
                vector<Arista *> crearEstados;
                int cantT;
                string cc;
                vector<string> cualesIria;
                int cantEstadosJuntados;

                for(int a = 0;a<cantEstados2;a++){
                    for(int b = 0;b<cantSimbolos2;b++){
                        cout<<"A cuantos estados ira el simbolo "<<"'"<<simb2[b]<<"'"<<" con el estado "<<to_string(a)<<": ";
                        cin>>cantT;
                        cout<<"Cuales serian: ";
                        for(int a = 0; a<cantT; a++){
                            cin>>cc;
                            cualesIria.push_back(cc);
                        }
                        for(int x = 0; x<cualesIria.size(); x++){
                            Arista * nuevaArista = new Arista(stoi(cualesIria[x]),adminAuto->simbolosArr.at(b));
                            adminAuto->getEstado(a)->addToTransArray(nuevaArista);
                        }
                        
                        if(cantT == 1){
                            Arista * nuevaArista = new Arista(stoi(cualesIria[0]),adminAuto->simbolosArr.at(b));
                            adminAuto->getEstado(a)->addToTransNFAArray(nuevaArista);
                        } else{
                            string nuevoEstadoName;
                            
                            
                            bool existe = false;
                            for(int x = 0; x<adminAuto->estadosArr.size(); x++){
                                if(adminAuto->estadosArr.at(x)->name == nuevoEstadoName){
                                    existe = true;
                                    break;
                                }
                            }
                            if(!existe){
                                Estado * nfaNew = new Estado(nuevoEstadoName);
                                adminAuto->estadosJuntadosArr.push_back(nfaNew);
                                cantEstadosJuntados++;
                            }
                        }
                        cualesIria.clear();
                
                    }
                }
                
                vector<Estado *> estadosConMas;
                
                for(int a = 0; a<estadosConMas.size(); a++){
                    for(int b = 0; b<cantT; b++){
                        //adminAuto->getEstado(a)->transicionesArr
                        cout<<estadosConMas.at(a)->name<<" con los estados: "<<crearEstados.at(b)<<endl;
                    }
                    
                }
                
                //LEER CADENAS DEL ARCHIVO!!!
                string filename;
                
                string cadena;
                char * cad;
                cout<<"Ingrese nombre del archivo: ";
                cin>>filename;
                ifstream inFile(filename);
                
                while(inFile>>cadena){

                    bool cc = adminAuto->checkIfCadenaValidaDFA2(cadena);
                    if(cc){
                        cout<<"Cadena "<<"'"<<cadena<<"'"<<" es aceptada"<<endl;
                    } else{
                        cout<<"Cadena "<<"'"<<cadena<<"'"<<" no es aceptada"<<endl;
                    }
                    
                }
                inFile.close();
                
                for(int a = 0; a<adminAuto->estadosArr.size(); a++){
                    adminAuto->estadosArr.at(a)->transicionesArr.clear();
                }
                
                
                break;
            }
            case 3:{
                int cantEstados;
                int cantSimbolos;
                vector<string> simb;
                string **tabla; //solo para imprimir para enseñar al usuario de su automata
                
                cout<<"Cantidad de estados: ";
                cin>>cantEstados;
                cout<<"Cuantos simbolos agregaria: ";
                cin>>cantSimbolos;
                string s;
                cout<<"Cuales serian los simbolos: ";
                for(int a = 0; a<cantSimbolos; a++){
                    cin>>s;
                    simb.push_back(s);
                }
                simb.push_back("e"); //add el epsilon al bajo bajo
                cantSimbolos++;
                adminAuto->simbolosPossibles(cantSimbolos,simb); //add al alfabeto
                
                
                tabla = new string*[cantEstados+1];
                for(int a = 0; a<cantEstados+1; a++){
                    tabla[a] = new string[cantSimbolos+1];
                }
                
                Estado * nuevo;
                
                //PARA CONSTRUIR LA TABLA EN EL EXTERIOR
                tabla[0][0] = "-";
                
                for(int a = 1; a<=cantEstados; a++){
                    tabla[a][0] = to_string(a-1);
                    nuevo = new Estado(to_string(a-1));
                    adminAuto->estadosArr.push_back(nuevo); //add to vector del automata
                }
                for(int b = 1; b<=cantSimbolos; b++){
                    tabla[0][b] = simb[b-1];
                }
                
                //PARA AGREGAR VALORES DENTRO DE LA TABLA
                for(int a = 1; a<=cantEstados; a++){
                    for(int b = 1; b<=cantSimbolos; b++){
                        tabla[a][b] = "-";
                    }
                }
                
                
                //SETTING ESTADOS FINALES E INICIAL
                int estadoIni;
                int ef;
                vector<Estado *>estadoFin;
                
                cout<<"Estado inicial: ";
                cin>>estadoIni;
                
                Estado * ini = adminAuto->getEstado(estadoIni);
                adminAuto->setEstadoIni(ini);
                cout<<"Cuantos estados finales serian? ";
                cin>>ef;
                cout<<"Estado(s) final(es): ";
                int temp;
                for(int a = 0; a<ef; a++){
                    cin>>temp;
                    Estado * fin = adminAuto->getEstado(temp);
                    estadoFin.push_back(fin);
                }
                
                adminAuto->setEstadosFin(estadoFin);
                
                
                
                
                break;
            
            }
            default:
                break;
        }
        
    } while (query != 4);
    
    adminAuto->estadosArr.clear();
    adminAuto->aceptacionArr.clear();
    adminAuto->simbolosArr.clear();
    delete adminAuto;
  
    return 0;
}
