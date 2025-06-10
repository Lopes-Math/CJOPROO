#include "jogador.hpp"

Jogador::Jogador() {
    posicao = {350.0f, 560.0f}; 
    velocidade = 5.0f;
    textura = {0};
}

Jogador::~Jogador() {
    UnloadTexture(textura);
}

void Jogador::CarregarTextura(const char* caminho) {
    textura = LoadTexture(caminho);
}

void Jogador::Atualizar() {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) posicao.x += velocidade;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) posicao.x -= velocidade;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) posicao.y -= velocidade;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) posicao.y += velocidade;

    if (posicao.x < 0) posicao.x = 0;
    if (posicao.x + 100 > GetScreenWidth()) posicao.x = GetScreenWidth() - 100;
    if (posicao.y < 0) posicao.y = 0;
    if (posicao.y + 160 > GetScreenHeight()) posicao.y = GetScreenHeight() - 160;
}

void Jogador::Desenhar() {
    Rectangle source = { 0, 0, (float)textura.width, (float)textura.height };
    Rectangle dest = { posicao.x, posicao.y, 100.0f, 160.0f };
    Vector2 origem = { 0, 0 };

    DrawTexturePro(textura, source, dest, origem, 0.0f, WHITE);
}

void Jogador::DefinirPosicao(Vector2 pos) {
    posicao = pos;
}

Vector2 Jogador::ObterPosicao() const {
    return posicao;
}

Rectangle Jogador::GetRetangulo() const {
    return { posicao.x, posicao.y, 100.0f, 160.0f };
}
