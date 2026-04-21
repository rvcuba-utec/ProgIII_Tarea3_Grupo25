#include "Entity.h"
#include "CommandCenter.h"
#include <iostream>
#include <list>
#include <string>
#include <stdexcept>

// ---- funciones libres ----

void mostrarEstado(Entity& e, const std::list<std::string>& args){
    if(args.size() != 0){
        throw std::invalid_argument("status no recibe argumentos");
    }
    std::cout<<"STATUS: ";
    e.mostrar();
    std::cout<<"\n";
}

void resetear(Entity& e, const std::list<std::string>& args){
    if(args.size() != 0){
        throw std::invalid_argument("reset no recibe argumentos");
    }
    std::cout<<"RESET antes: ";
    e.mostrar();
    std::cout<<"\n";
    e.resetear();
    std::cout<<"RESET despues: ";
    e.mostrar();
    std::cout<<"\n";
}

void saludar(Entity& e, const std::list<std::string>& args){
    std::cout<<"Hola "<<e.getNombre();
    if(args.size() > 0){
        std::cout<<", mensaje: "<<args.front();
    }
    std::cout<<"\n";
}


// ---- functores ----

// functor que cura pero tiene limite de usos
class CuraLimitada{
private:
    Entity& entidad;
    int usosRestantes;
public:
    CuraLimitada(Entity& entidad_, int usos) : entidad(entidad_){
        usosRestantes = usos;
    }
    void operator()(const std::list<std::string>& args){
        if(args.size() != 1){
            throw std::invalid_argument("curaLim: falta la cantidad");
        }
        if(usosRestantes <= 0){
            std::cout<<"curaLim ya no tiene usos\n";
            return;
        }
        int cant = std::stoi(args.front());
        if(cant < 0){
            throw std::invalid_argument("curaLim: cantidad negativa");
        }
        std::cout<<"CURA_LIM antes: "<<entidad.aTexto();
        entidad.curar(cant);
        usosRestantes = usosRestantes - 1;
        std::cout<<" -> despues: "<<entidad.aTexto();
        std::cout<<" (usos que quedan: "<<usosRestantes<<")\n";
    }
};

// functor que cuenta cuantas veces se ejecuta y da oro
class Recompensa{
private:
    Entity& entidad;
    int veces;
    int oroPorUso;
public:
    Recompensa(Entity& entidad_, int oro) : entidad(entidad_){
        veces = 0;
        oroPorUso = oro;
    }
    void operator()(const std::list<std::string>& args){
        if(args.size() != 0){
            throw std::invalid_argument("recompensa no recibe argumentos");
        }
        veces = veces + 1;
        std::cout<<"RECOMPENSA antes: "<<entidad.aTexto();
        entidad.agregarOro(oroPorUso);
        std::cout<<" -> "<<entidad.aTexto();
        std::cout<<" (veces usado: "<<veces<<")\n";
    }
};

// functor pa subir de nivel
class SubirNivel{
private:
    Entity& entidad;
    int ultimoNivel;
public:
    SubirNivel(Entity& entidad_) : entidad(entidad_){
        ultimoNivel = entidad_.getNivel();
    }
    void operator()(const std::list<std::string>& args){
        if(args.size() != 0){
            throw std::invalid_argument("nivel no recibe argumentos");
        }
        std::cout<<"NIVEL antes: "<<entidad.aTexto();
        entidad.subirNivel();
        ultimoNivel = entidad.getNivel();
        std::cout<<" -> "<<entidad.aTexto()<<"\n";
    }
};


// ---- main ----

int main(){
    Entity jugador("Aria", 100, 50);
    jugador.agregarOro(10);

    CommandCenter centro;

    std::cout<<"=== ESTADO INICIAL ===\n";
    jugador.mostrar();
    std::cout<<"\n";

    // registro de funciones libres (envueltas en lambdas pa pasarles el jugador)
    std::cout<<"\n=== REGISTRO: funciones libres ===\n";

    centro.registerCommand("status", [&jugador](const std::list<std::string>& args){
        mostrarEstado(jugador, args);
    });
    centro.registerCommand("reset", [&jugador](const std::list<std::string>& args){
        resetear(jugador, args);
    });
    centro.registerCommand("saludar", [&jugador](const std::list<std::string>& args){
        saludar(jugador, args);
    });

    std::list<std::string> vacio;

    centro.execute("status", vacio);

    std::list<std::string> args1;
    args1.push_back("bienvenida!");
    centro.execute("saludar", args1);

    centro.execute("reset", vacio);
    jugador.agregarOro(10);


    // lambdas con captura
    std::cout<<"\n=== REGISTRO: lambdas con captura ===\n";

    centro.registerCommand("mover", [&jugador](const std::list<std::string>& args){
        if(args.size() != 2){
            throw std::invalid_argument("mover necesita 2 argumentos: dx dy");
        }
        auto it = args.begin();
        float dx = std::stof(*it);
        ++it;
        float dy = std::stof(*it);
        std::cout<<"MOVER antes: "<<jugador.aTexto();
        jugador.mover(dx, dy);
        std::cout<<" -> "<<jugador.aTexto()<<"\n";
    });

    centro.registerCommand("daniar", [&jugador](const std::list<std::string>& args){
        if(args.size() != 1){
            throw std::invalid_argument("daniar necesita 1 argumento");
        }
        int cant = std::stoi(args.front());
        if(cant < 0){
            throw std::invalid_argument("daniar: no puede ser negativo");
        }
        std::cout<<"DANIAR antes: "<<jugador.aTexto();
        jugador.daniar(cant);
        std::cout<<" -> "<<jugador.aTexto()<<"\n";
    });

    centro.registerCommand("renombrar", [&jugador](const std::list<std::string>& args){
        if(args.size() != 1){
            throw std::invalid_argument("renombrar necesita 1 argumento");
        }
        std::cout<<"RENOMBRAR antes: "<<jugador.aTexto();
        jugador.cambiarNombre(args.front());
        std::cout<<" -> "<<jugador.aTexto()<<"\n";
    });

    std::list<std::string> argsMover;
    argsMover.push_back("10");
    argsMover.push_back("20");
    centro.execute("mover", argsMover);

    std::list<std::string> argsDaniar;
    argsDaniar.push_back("15");
    centro.execute("daniar", argsDaniar);

    std::list<std::string> argsRen;
    argsRen.push_back("Aria_la_Valiente");
    centro.execute("renombrar", argsRen);


    // functores
    std::cout<<"\n=== REGISTRO: functores ===\n";

    centro.registerCommand("curaLim", CuraLimitada(jugador, 2));
    centro.registerCommand("recompensa", Recompensa(jugador, 5));
    centro.registerCommand("nivel", SubirNivel(jugador));

    std::list<std::string> argsCura;
    argsCura.push_back("8");
    centro.execute("curaLim", argsCura);

    centro.execute("recompensa", vacio);
    centro.execute("nivel", vacio);


    // pruebas de errores
    std::cout<<"\n=== PRUEBAS DE ERRORES ===\n";

    std::list<std::string> mal1;
    mal1.push_back("solo_uno");
    centro.execute("mover", mal1);

    std::list<std::string> mal2;
    mal2.push_back("abc");
    centro.execute("daniar", mal2);

    std::list<std::string> mal3;
    mal3.push_back("extra");
    centro.execute("status", mal3);

    centro.execute("noExiste", vacio);


    // eliminar comandos
    std::cout<<"\n=== ELIMINAR COMANDOS ===\n";
    centro.listarComandos();

    bool r1 = centro.removeCommand("renombrar");
    bool r2 = centro.removeCommand("loQueSea");
    std::cout<<"eliminar renombrar: ";
    if(r1) std::cout<<"ok\n"; else std::cout<<"no existe\n";
    std::cout<<"eliminar loQueSea: ";
    if(r2) std::cout<<"ok\n"; else std::cout<<"no existe\n";

    centro.listarComandos();

    std::list<std::string> argsRen2;
    argsRen2.push_back("NombreNuevo");
    centro.execute("renombrar", argsRen2);


    // macros
    std::cout<<"\n=== REGISTRO DE MACROS ===\n";

    // macro 1: curar y ver
    std::list<std::pair<std::string, std::list<std::string>>> macro1;
    std::list<std::string> a1;
    a1.push_back("9");
    macro1.push_back(std::make_pair("curaLim", a1));
    macro1.push_back(std::make_pair("status", std::list<std::string>()));
    centro.registerMacro("curar_y_ver", macro1);

    // macro 2: combo de pelea
    std::list<std::pair<std::string, std::list<std::string>>> macro2;
    std::list<std::string> m2a;
    m2a.push_back("5");
    m2a.push_back("-3");
    macro2.push_back(std::make_pair("mover", m2a));
    std::list<std::string> m2b;
    m2b.push_back("10");
    macro2.push_back(std::make_pair("daniar", m2b));
    macro2.push_back(std::make_pair("recompensa", std::list<std::string>()));
    macro2.push_back(std::make_pair("status", std::list<std::string>()));
    centro.registerMacro("combo_pelea", macro2);

    // macro 3: ciclo completo
    std::list<std::pair<std::string, std::list<std::string>>> macro3;
    macro3.push_back(std::make_pair("reset", std::list<std::string>()));
    macro3.push_back(std::make_pair("nivel", std::list<std::string>()));
    macro3.push_back(std::make_pair("recompensa", std::list<std::string>()));
    macro3.push_back(std::make_pair("status", std::list<std::string>()));
    centro.registerMacro("ciclo_completo", macro3);

    // macro roto pa probar el error
    std::list<std::pair<std::string, std::list<std::string>>> macroRoto;
    macroRoto.push_back(std::make_pair("status", std::list<std::string>()));
    macroRoto.push_back(std::make_pair("comandoFalso", std::list<std::string>()));
    centro.registerMacro("macro_roto", macroRoto);

    std::cout<<"\n=== EJECUTAR MACROS ===\n";
    centro.executeMacro("curar_y_ver");
    centro.executeMacro("combo_pelea");
    centro.executeMacro("ciclo_completo");

    std::cout<<"\n=== MACROS CON ERROR ===\n";
    centro.executeMacro("macro_roto");
    centro.executeMacro("no_existe");


    std::cout<<"\n=== ESTADO FINAL ===\n";
    jugador.mostrar();
    std::cout<<"\n";

    centro.mostrarHistorial();

    return 0;
}
