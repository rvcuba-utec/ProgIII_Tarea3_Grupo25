#include "Entity.h"
#include <iostream>

Entity::Entity(){
    nombre = "Heroe";
    posX = 0;
    posY = 0;
    vida = 100;
    vidaMax = 100;
    energia = 50;
    nivel = 1;
    oro = 0;
}

Entity::Entity(std::string nombre_, int vida_, int energia_){
    nombre = nombre_;
    posX = 0;
    posY = 0;
    vida = vida_;
    vidaMax = vida_;
    energia = energia_;
    nivel = 1;
    oro = 0;
}

void Entity::mover(float dx, float dy){
    posX = posX + dx;
    posY = posY + dy;
}

void Entity::curar(int cant){
    if(cant < 0) return;
    vida = vida + cant;
    if(vida > vidaMax) vida = vidaMax;
}

void Entity::daniar(int cant){
    if(cant < 0) return;
    vida = vida - cant;
    if(vida < 0) vida = 0;
}

void Entity::gastarEnergia(int cant){
    energia = energia - cant;
    if(energia < 0) energia = 0;
}

void Entity::subirNivel(){
    nivel = nivel + 1;
    vidaMax = vidaMax + 10;
    vida = vidaMax;
    energia = energia + 10;
}

void Entity::agregarOro(int cant){
    oro = oro + cant;
}

void Entity::cambiarNombre(std::string nuevo){
    nombre = nuevo;
}

void Entity::resetear(){
    posX = 0;
    posY = 0;
    vidaMax = 100;
    vida = 100;
    energia = 50;
    nivel = 1;
    oro = 0;
}

std::string Entity::getNombre(){ return nombre; }
float Entity::getX(){ return posX; }
float Entity::getY(){ return posY; }
int Entity::getVida(){ return vida; }
int Entity::getVidaMax(){ return vidaMax; }
int Entity::getEnergia(){ return energia; }
int Entity::getNivel(){ return nivel; }
int Entity::getOro(){ return oro; }

void Entity::mostrar(){
    std::cout<<"["<<nombre<<" pos=("<<posX<<","<<posY<<") vida="<<vida<<"/"<<vidaMax;
    std::cout<<" energia="<<energia<<" nivel="<<nivel<<" oro="<<oro<<"]";
}

std::string Entity::aTexto(){
    std::string s = "[" + nombre;
    s = s + " vida=" + std::to_string(vida);
    s = s + " pos=(" + std::to_string((int)posX) + "," + std::to_string((int)posY) + ")";
    s = s + " oro=" + std::to_string(oro) + "]";
    return s;
}
