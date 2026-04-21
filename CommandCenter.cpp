#include "CommandCenter.h"
#include <iostream>

CommandCenter::CommandCenter(){
}

void CommandCenter::registerCommand(std::string nombre, Command cmd){
    comandos[nombre] = cmd;
    historial.push_back("registrado: " + nombre);
}

void CommandCenter::execute(std::string nombre, const std::list<std::string>& args){
    std::map<std::string, Command>::iterator it = comandos.find(nombre);
    if(it == comandos.end()){
        std::cout<<"ERROR: no existe el comando '"<<nombre<<"'\n";
        historial.push_back("fallo ejecutar: " + nombre);
        return;
    }

    // armamos texto con los argumentos pal historial
    std::string argsTxt = "";
    for(auto a = args.begin(); a != args.end(); ++a){
        argsTxt = argsTxt + " " + *a;
    }

    try{
        it->second(args);
        historial.push_back("ejecutado: " + nombre + argsTxt);
    }catch(std::exception& e){
        std::cout<<"ERROR en "<<nombre<<": "<<e.what()<<"\n";
        historial.push_back("error ejecutar: " + nombre + " -> " + e.what());
    }
}

bool CommandCenter::removeCommand(std::string nombre){
    std::map<std::string, Command>::iterator it = comandos.find(nombre);
    if(it == comandos.end()){
        historial.push_back("no se pudo eliminar (no existe): " + nombre);
        return false;
    }
    comandos.erase(it);
    historial.push_back("eliminado: " + nombre);
    return true;
}

bool CommandCenter::existe(std::string nombre){
    if(comandos.find(nombre) != comandos.end()) return true;
    return false;
}

void CommandCenter::registerMacro(const std::string& name, const std::list<std::pair<std::string, std::list<std::string>>>& steps){
    macros[name] = steps;
    historial.push_back("macro registrado: " + name);
}

void CommandCenter::executeMacro(const std::string& name){
    std::map<std::string, std::list<Paso>>::iterator it = macros.find(name);
    if(it == macros.end()){
        std::cout<<"ERROR: no existe el macro '"<<name<<"'\n";
        historial.push_back("fallo macro: " + name);
        return;
    }

    historial.push_back("inicio macro: " + name);

    std::list<Paso>& pasos = it->second;
    for(std::list<Paso>::iterator p = pasos.begin(); p != pasos.end(); ++p){
        std::string nombreCmd = p->first;
        std::list<std::string> argsCmd = p->second;

        if(existe(nombreCmd) == false){
            std::cout<<"ERROR: el macro '"<<name<<"' usa comando '"<<nombreCmd<<"' que no existe\n";
            historial.push_back("macro abortado: " + name);
            return;
        }
        execute(nombreCmd, argsCmd);
    }

    historial.push_back("fin macro: " + name);
}

void CommandCenter::mostrarHistorial(){
    std::cout<<"\n--- HISTORIAL ("<<historial.size()<<" lineas) ---\n";
    int i = 1;
    for(auto it = historial.begin(); it != historial.end(); ++it){
        std::cout<<i<<") "<<*it<<"\n";
        i = i + 1;
    }
    std::cout<<"---------------------------------\n";
}

void CommandCenter::listarComandos(){
    std::cout<<"Comandos actuales: ";
    for(auto it = comandos.begin(); it != comandos.end(); ++it){
        std::cout<<it->first<<" ";
    }
    std::cout<<"\n";
}
