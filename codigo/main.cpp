#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace std;
void database_in (string basededatos);
void database_out (long long int letras,long long int palabras,long long int oraciones,long long int parrafos ,string texto);
string noticiero(string texto);
bool idfechas(const string & text,long long int index);
bool vacios(void);
long long int parrafosc(const string & texto,bool &parrafoant);


int main(){
  locale::global(locale(locale(), new codecvt_utf8<wchar_t>));
cout<<R"( ________  ________  ________      ___    ___ _______   ________ _________  ________     
|\   __  \|\   __  \|\   __  \    |\  \  /  /|\  ___ \ |\   ____\\___   ___\\   __  \    
\ \  \|\  \ \  \|\  \ \  \|\  \   \ \  \/  / | \   __/|\ \  \___\|___ \  \_\ \  \|\  \   
 \ \   ____\ \   _  _\ \  \\\  \   \ \    / / \ \  \_|/_\ \  \       \ \  \ \ \  \\\  \  
  \ \  \___|\ \  \\  \\ \  \\\  \   \/  /  /   \ \  \_|\ \ \  \____   \ \  \ \ \  \\\  \ 
   \ \__\    \ \__\\ _\\ \_______\__/  / /      \ \_______\ \_______\  \ \__\ \ \_______\
    \|__|     \|__|\|__|\|_______|\___/ /        \|_______|\|_______|   \|__|  \|_______|
                                 \|___|/                                                 
                                                                                         
                                                                                         )"<<endl<<endl;
string ubicacion="database/";
cout<<"escriba el anno de la base de datos analizar desde el 2010-2023"<<endl;
int año=0;cin>>año;
if(año>2023 || año<2010){
    cout<<"ingreso una fecha incorrecta"<<endl;
}
else{
    cout<<"ESTAS SEGURO QUE QUIERES ANALIZAR LA BASE DE DATOS "<<año<<" ?"<<endl; 
    cout<<"escriba 1 para SI o 0 para NO"<<endl;
    bool pass=false;cin>>pass;
    if(pass==true){
    database_in(ubicacion+to_string(año)+".txt");
    }
}

    return 0;
}



void database_in (string basededatos){
    ifstream database;
    string texto,url;
    int pass=0;
    long long int letras=0;
    long long int numpal=0;
    long long int oraciones=0;
    long long int parrafos=0;
    static bool linea_año=false;
    bool identificador=false;
    bool parrafoant=false;
    database.open(basededatos, ios::in); 
    if (database.fail()){ 
        cout<<"Error al abrir el archivo";
        cout<<endl;
        return ;
    }
    while (!database.eof()){ 
      getline(database,texto); 
        if(linea_año==false){
             database_out(0,0,0,0,texto);
             linea_año=true;
             continue;
        }
        if(identificador==false && texto!="####"){
            url=texto;
        }
        if(texto=="####"){
            if(identificador==true){
                identificador=false;
                pass++;
            }
            else{
            identificador=true;
            pass++;
            continue;
            }
        }
        //si todo esta correcto cuenta las letra;
        if(identificador==true) {
           stringstream tx(texto);
             string palabra;
            //contar letras y oraciones
             for(long long int i=0;i<texto.size();i++){
                        if(isalnum(texto[i])){
                            letras++;
                            parrafoant=true;
                        }
                        if(texto[i]=='.' && idfechas(texto,i)==false){
                            oraciones++;
                        }
                }
            //contar palabras
            while (tx >> palabra) {
                numpal=numpal+1;
            }
            //contar parrafos
            parrafos=parrafos+parrafosc(texto,parrafoant);
        }
        
        if(pass==2){
        if(parrafoant==true){
            parrafos++;
        }
        database_out(letras,numpal,oraciones,parrafos,url);
        pass=0;
        letras=0;
        numpal=0;
        oraciones=0;
        parrafos=0;
        parrafoant=false;
        }
    }

}

void database_out (long long int letras,long long int palabras,long long int oraciones,long long int parrafos,string texto){ //funcion para agregar datos al txt
    ofstream database; 
    static bool inicio=true;
    static string año;
    static bool vacio=true;
   database.open("clean.txt", ios::app); 
    if (database.fail()){ 
        cout<<"OCURRIO UN ERROR INESPERADO BORRE EL ARCHIVO clean.txt POR SEGURIDAD Y EJECUTE EL PROGRAMA DE NUEVO"<<endl;
    }
    if(vacio==true){
        if(vacios()==true){
            database<<"año,"<<"noticiero,"<<"URL,"<<"letras,palabras,oraciones,parrafos"<<endl;
            vacio=false;
        }
        else{
            vacio=false;
        }
    }
    if(inicio==true){
            año=texto;
            inicio=false;
        }
    else if(inicio==false){
            database<<año<<","<<noticiero(texto)<<","<<texto<<","<<letras<<","<<palabras<<","<<oraciones<<","<<parrafos<<endl;
    }        
}

string noticiero( string  texto){
   string result;
    for (long long int i = 0; i < texto.size(); i++)
    {   if(texto[i]=='/' && texto[i+1]!='/'){
            if(texto[i+1]=='w'){
                i=i+5;
                while(texto[i]!='.'){
                    result=result+texto[i];
                    ++i;
                }
                break;
            }
            else{
                i++;
                while(texto[i]!='.'){
                    result=result+texto[i];
                    ++i;
                }
                break;
            }
        }
        
    }
    return result;
}



bool idfechas(const string & texto,long long int index){
     if (index > 0 && index < texto.size() - 1) {
        return isdigit(texto[index - 1]) && texto[index] == '.' && isdigit(texto[index + 1]);
    }
    return false;
}

bool vacios(void){
        ifstream archivo("clean.txt");
    if(archivo.peek() == std::ifstream::traits_type::eof()){
        return true;
    }
    else{
        return false;
    }
}

long long int parrafosc(const string &texto,bool &parrafoant){
    long long int parrafos=0;
    string linea = texto;
    linea.erase(remove_if(linea.begin(), linea.end(), ::isspace), linea.end());
    if(linea.empty()==true && parrafoant==true){
        parrafos++;
        parrafoant=false;
    }
    return parrafos;
}