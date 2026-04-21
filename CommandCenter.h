#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H
#include <functional>
#include <list>
#include <map>
#include <string>
#include <utility>

using Command = std::function<void(const std::list<std::string>&)>;

using Paso = std::pair<std::string, std::list<std::string>>;

class CommandCenter{
private:
    std::map<std::string, Command> comandos;
    std::map<std::string, std::list<Paso>> macros;
    std::list<std::string> historial;
public:
    CommandCenter();
    void registerCommand(std::string nombre, Command cmd);
    void execute(std::string nombre, const std::list<std::string>& args);
    bool removeCommand(std::string nombre);
    bool existe(std::string nombre);
    void registerMacro(const std::string& name, const std::list<std::pair<std::string, std::list<std::string>>>& steps);
    void executeMacro(const std::string& name);
    void mostrarHistorial();
    void listarComandos();
};

#endif
