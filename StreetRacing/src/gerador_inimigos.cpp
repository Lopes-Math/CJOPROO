#include "gerador_inimigos.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

GeradorInimigos::GeradorInimigos() {
    tempoSpawn = 1.5f;
    tempoAtual = 0;
    velocidade = 4.0f;
    srand(time(NULL));
}

GeradorInimigos::~GeradorInimigos() {
    for (auto& par : texturasPorTipo) {
        for (auto& tex : par.second) {
            UnloadTexture(tex);
        }
    }
}

void GeradorInimigos::CarregarTexturas() {
    texturasPorTipo[1] = {
        LoadTexture("src/images/carro1.png"),
        LoadTexture("src/images/carro1_b.png")
    };
    tamanhosPorTipo[1] = {110, 170};

    texturasPorTipo[2] = {
        LoadTexture("src/images/carro2.png"),
        LoadTexture("src/images/carro2_b.png")
    };
    tamanhosPorTipo[2] = {120, 200};

    texturasPorTipo[3] = {
        LoadTexture("src/images/carro3.png"),
        LoadTexture("src/images/carro3_b.png")
    };
    tamanhosPorTipo[3] = {200, 450};

    texturasPorTipo[4] = {
        LoadTexture("src/images/carro4.png")
    };
    tamanhosPorTipo[4] = {110, 165};

    texturasPorTipo[5] = {
        LoadTexture("src/images/carro5.png"),
    };
    tamanhosPorTipo[5] = {200, 320};

    texturasPorTipo[6] = {
        LoadTexture("src/images/carro6.png"),
        LoadTexture("src/images/carro6_b.png"),
        LoadTexture("src/images/carro6_c.png")
    };
    tamanhosPorTipo[6] = {110, 163};

    texturasPorTipo[7] = {
        LoadTexture("src/images/carro7.png")
    };
    tamanhosPorTipo[7] = {100, 157};
}

void GeradorInimigos::Atualizar() {
    tempoAtual += GetFrameTime();

    if (tempoAtual >= tempoSpawn) {
        tempoAtual = 0;

        std::vector<int> faixas = {0, 1, 2};
        int qtd = GetRandomValue(1, 2);
        std::random_shuffle(faixas.begin(), faixas.end());

        for (int i = 0; i < qtd; i++) {
            int faixa = faixas[i];

            bool faixaOcupada = false;
            for (const auto& inimigo : inimigos) {
                if (inimigo.faixa == faixa && inimigo.posicao.y < 300) {
                    faixaOcupada = true;
                    break;
                }
            }

            if (faixaOcupada) continue;

            int tipo = GetRandomValue(1, 7);
            std::vector<Texture2D>& lista = texturasPorTipo[tipo];
            Texture2D tex = lista[GetRandomValue(0, (int)lista.size() - 1)];
            Vector2 tamanho = tamanhosPorTipo[tipo];

            inimigos.emplace_back(tex, faixa, -tamanho.y, velocidade, (int)tamanho.x, (int)tamanho.y);
        }
    }

    for (auto& i : inimigos)
        i.Atualizar();

    inimigos.erase(
        std::remove_if(inimigos.begin(), inimigos.end(), [](Inimigo& i) {
            return i.ForaDaTela();
        }),
        inimigos.end()
    );
}

void GeradorInimigos::Desenhar() {
    for (auto& i : inimigos)
        i.Desenhar();
}

void GeradorInimigos::Resetar() {
    inimigos.clear();
    tempoAtual = 0;
}

const std::vector<Inimigo>& GeradorInimigos::GetInimigos() const {
    return inimigos;
}
