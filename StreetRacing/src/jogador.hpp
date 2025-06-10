#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include "raylib.h"

class Jogador {
public:
    Jogador();
    ~Jogador();

    void CarregarTextura(const char* caminho);
    void Atualizar();
    void Desenhar();
    void DefinirPosicao(Vector2 pos);
    Vector2 ObterPosicao() const;
    Rectangle GetRetangulo() const;

private:
    Texture2D textura;
    Vector2 posicao;
    float velocidade;
};

#endif
