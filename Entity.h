#ifndef ENTITY_H
#define ENTITY_H
#include <string>

class Entity{
private:
    std::string nombre;
    float posX;
    float posY;
    int vida;
    int vidaMax;
    int energia;
    int nivel;
    int oro;
public:
    Entity();
    Entity(std::string nombre_, int vida_, int energia_);

    void mover(float dx, float dy);
    void curar(int cant);
    void daniar(int cant);
    void gastarEnergia(int cant);
    void subirNivel();
    void agregarOro(int cant);
    void cambiarNombre(std::string nuevo);
    void resetear();

    std::string getNombre();
    float getX();
    float getY();
    int getVida();
    int getVidaMax();
    int getEnergia();
    int getNivel();
    int getOro();

    void mostrar();
    std::string aTexto();
};

#endif
