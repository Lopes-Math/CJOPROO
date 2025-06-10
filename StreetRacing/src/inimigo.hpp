#ifndef INIMIGO_HPP
#define INIMIGO_HPP

#include "raylib.h"

class Inimigo {
public:
    Vector2 posicao;
    Texture2D textura;
    float velocidade;
    int faixa;
    int largura;
    int altura;

    Inimigo(Texture2D tex, int faixa, float yInicial, float velocidade, int largura, int altura);
    void Atualizar();
    void Desenhar();
    bool ForaDaTela() const;
    bool VisivelNaTela() const;
    Rectangle GetRetangulo() const;
};

#endif
