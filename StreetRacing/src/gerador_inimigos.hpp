#ifndef GERADOR_INIMIGOS_HPP
#define GERADOR_INIMIGOS_HPP

#include "inimigo.hpp"
#include <vector>
#include <map>

class GeradorInimigos {
public:
    GeradorInimigos();
    ~GeradorInimigos();

    void CarregarTexturas();
    void Atualizar();
    void Desenhar();
    void Resetar();

    const std::vector<Inimigo>& GetInimigos() const; 

private:
    std::map<int, std::vector<Texture2D>> texturasPorTipo;
    std::map<int, Vector2> tamanhosPorTipo;

    std::vector<Inimigo> inimigos;
    float tempoAtual;
    float tempoSpawn;
    float velocidade;
};

#endif
