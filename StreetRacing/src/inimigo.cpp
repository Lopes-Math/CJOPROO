#include "inimigo.hpp"

Inimigo::Inimigo(Texture2D tex, int faixaIndex, float yInicial, float vel, int larguraAlvo, int alturaAlvo)
    : textura(tex), faixa(faixaIndex), velocidade(vel), largura(larguraAlvo), altura(alturaAlvo) {
    
    int larguraFaixaBranca = 8;
    float pistaUtil = 800 - 2 * (larguraFaixaBranca + 6);
    float larguraFaixa = pistaUtil / 3.0f;

    posicao.x = larguraFaixaBranca + 6 + larguraFaixa * faixa + (larguraFaixa - largura) / 2;
    posicao.y = yInicial;
}

void Inimigo::Atualizar() {
    posicao.y += velocidade;
}

void Inimigo::Desenhar() {
    Rectangle source = {0, 0, (float)textura.width, (float)textura.height};
    Rectangle dest = {posicao.x, posicao.y, (float)largura, (float)altura};
    Vector2 origin = {0, 0};

    DrawTexturePro(textura, source, dest, origin, 0.0f, WHITE);
}

bool Inimigo::ForaDaTela() const {
    return posicao.y > GetScreenHeight();
}

bool Inimigo::VisivelNaTela() const {
    return posicao.y + altura >= 0 && posicao.y <= GetScreenHeight();
}

Rectangle Inimigo::GetRetangulo() const {
    return {
        posicao.x + 5,
        posicao.y + 5,
        (float)largura - 10,
        (float)altura - 10
    };
}
