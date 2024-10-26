#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <locale>
#include <codecvt>

using namespace std;
void database_in (string basededatos);
void database_out (int letras,int palabras ,string texto);
string noticiero(string texto);
int main(){
  locale::global(locale(locale(), new codecvt_utf8<wchar_t>));

string ubicacion="database/";
cout<<"escriba el año de la base de datos analizar desde el 2010-2023"<<endl;
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
    static bool linea_año=false;
    bool identificador=false;
    database.open(basededatos, ios::in); 
    if (database.fail()){ 
        cout<<"Error al abrir el archivo";
        cout<<endl;
        return ;
    }
    while (!database.eof()){ 
      getline(database,texto); 
        if(linea_año==false){
             database_out(0,0,texto);
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
              while (tx >> palabra) {
                    for(char c : palabra){
                        if(isalnum(c)){
                            letras++;
                        }
                    }
                numpal=numpal+1;
            }
        }
        
        if(pass==2){
        database_out(letras,numpal,url);
        pass=0;
        letras=0;
        numpal=0;
        }
    }

}

void database_out (int letras,int palabras,string texto){ //funcion para agregar datos al txt
    ofstream database; 
    static bool inicio=true;
    static string año;
   database.open("clean.txt", ios::app); 
    if (database.fail()){ 
        cout<<"no se pudo abrir el archivo se va crear uno nuevo";
         database.open("clean.txt",ios::app);
    }
    if(inicio==true){
            database<<"año,"<<"noticiero,"<<"URL,"<<"letras,palabras"<<endl;
            año=texto;
            inicio=false;
        }
    else if(inicio==false){
            database<<año<<","<<noticiero(texto)<<","<<texto<<","<<letras<<","<<palabras<<endl;
    }

        
}

string noticiero( string  texto){
   string result;
    for (int i = 0; i < texto.size(); i++)
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



