#include "raylib.h"
#include "jogador.hpp"
#include "gerador_inimigos.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

bool jogoAtivo = false;
bool mostrarRecordes = false;
bool gameOver = false;
bool roncoConstanteTocando = false;
bool inserindoNome = true;

Jogador jogador;
GeradorInimigos gerador;

Sound somBuzina;
Sound somBatida;
Sound somRoncoAumentando;
Sound somRoncoConstante;
Sound somTrânsito;
Sound somLigaCarro;
Music musicaFundo;

Font fontePixel;
Texture2D fundoMenu;
Texture2D texturaRodovia;

char nomeInput[20] = "";
std::string nomeJogador = "";

int score = 0;
float tempoVivo = 0.0f;

struct Registro {
    std::string nome;
    int score;
};
std::vector<Registro> recordes;

void DesenharEntradaNome(Font fontePixel);
void DesenharMenu(Texture2D fundo, Font fontePixel);
void DesenharBotaoArredondado(Rectangle retangulo, const char* texto, Font fontePixel);
void DesenharTelaGameOver(Font fontePixel);
void AtualizarScore();
void SalvarRecorde();

// Função Principal
int main() {
    InitWindow(800, 720, "Street Racing - Corrida de Rua");
    InitAudioDevice();
    SetTargetFPS(60);

    fontePixel = LoadFont("src/fonts/PressStart2P.ttf");
    fundoMenu = LoadTexture("src/images/fundo_menu_principal.png");
    texturaRodovia = LoadTexture("src/images/fundo_rodovia.png");

    jogador.CarregarTextura("src/images/celtinha.png");
    gerador.CarregarTexturas();

    somBuzina = LoadSound("src/sounds/buzina.wav");
    somBatida = LoadSound("src/sounds/batida.wav");
    somRoncoAumentando = LoadSound("src/sounds/roncoaumentando.wav");
    somRoncoConstante = LoadSound("src/sounds/roncoconstante.wav");
    somTrânsito = LoadSound("src/sounds/barulhotrafego.wav");
    somLigaCarro = LoadSound("src/sounds/ligandocarro.wav");
    musicaFundo = LoadMusicStream("src/sounds/musica.wav");

    PlaySound(somLigaCarro);
    PlayMusicStream(musicaFundo);

    float deslocamentoFundo = 0.0f;
    float velocidadeRolagem = 10.0f;

    while (!WindowShouldClose()) {
        UpdateMusicStream(musicaFundo);
        deslocamentoFundo += velocidadeRolagem;
        if (deslocamentoFundo >= texturaRodovia.height)
            deslocamentoFundo = 0;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!jogoAtivo && !mostrarRecordes && !gameOver) {
            DrawTexture(fundoMenu, 0, 0, WHITE);
            DesenharEntradaNome(fontePixel);
            if (!inserindoNome) {
                DesenharMenu(fundoMenu, fontePixel);
            }
        }
        else if (jogoAtivo && !gameOver) {
            // Se o jogo estiver em andamento
            AtualizarScore();
            DrawTexture(texturaRodovia, 0, (int)deslocamentoFundo, WHITE);
            DrawTexture(texturaRodovia, 0, (int)deslocamentoFundo - texturaRodovia.height, WHITE);

            // Faixas da rodovia
            int larguraFaixaBranca = 8;
            DrawRectangle(6, 0, larguraFaixaBranca, GetScreenHeight(), WHITE);
            DrawRectangle(GetScreenWidth() - larguraFaixaBranca - 6, 0, larguraFaixaBranca, GetScreenHeight(), WHITE);

            float pistaUtil = GetScreenWidth() - 2 * (larguraFaixaBranca + 6);
            float larguraFaixa = pistaUtil / 3.0f;
            float pos1 = larguraFaixaBranca + 6 + larguraFaixa;
            float pos2 = larguraFaixaBranca + 6 + 2 * larguraFaixa;
            int alturaFaixa = 40, larguraAmarela = 10, espacamento = 80;
            Color amarelo = {255, 208, 0, 255};

            for (int y = -alturaFaixa; y < GetScreenHeight() + espacamento; y += espacamento) {
                int yAnimado = y + (int)deslocamentoFundo % espacamento;
                DrawRectangle(pos1 - larguraAmarela / 2, yAnimado, larguraAmarela, alturaFaixa, amarelo);
                DrawRectangle(pos2 - larguraAmarela / 2, yAnimado, larguraAmarela, alturaFaixa, amarelo);
            }

            jogador.Atualizar();
            gerador.Atualizar();
            gerador.Desenhar();
            jogador.Desenhar();

            DrawText(TextFormat("SCORE: %04d", score), 30, 20, 20, RED);

            // Sons do carro do jogador
            if (IsKeyPressed(KEY_SPACE)) PlaySound(somBuzina);
            if (!IsSoundPlaying(somTrânsito)) PlaySound(somTrânsito);

            if (!IsSoundPlaying(somRoncoAumentando) && !roncoConstanteTocando) {
                PlaySound(somRoncoAumentando);
            } else if (!roncoConstanteTocando && !IsSoundPlaying(somRoncoAumentando)) {
                PlaySound(somRoncoConstante);
                roncoConstanteTocando = true;
            }

            // Colisões
            Rectangle jogadorRec = jogador.GetRetangulo();
            for (const auto& inimigo : gerador.GetInimigos()) {
                if (inimigo.VisivelNaTela() && CheckCollisionRecs(jogadorRec, inimigo.GetRetangulo())) {
                    gameOver = true;
                    jogoAtivo = false;

                    StopMusicStream(musicaFundo);
                    StopSound(somTrânsito);
                    StopSound(somRoncoConstante);
                    StopSound(somRoncoAumentando);
                    StopSound(somLigaCarro);
                    PlaySound(somBatida);

                    SalvarRecorde();
                    break;
                }
            }
        }
        else if (gameOver) {
            // Tela de game over
            ClearBackground(BLACK);

            const char* texto = "GAME OVER";
            int tamanho = 90;
            int larguraTexto = MeasureText(texto, tamanho);
            int x = (GetScreenWidth() - larguraTexto) / 2;
            DrawText(texto, x, 240, tamanho, RED);

            DrawText(TextFormat("SCORE: %d", score), GetScreenWidth()/2 - 100, 360, 30, RED);

            Rectangle botaoMenu = {GetScreenWidth() / 2.0f - 80, 440, 160, 50};
            DesenharBotaoArredondado(botaoMenu, "MENU", fontePixel);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, botaoMenu)) {
                    gameOver = false;
                    mostrarRecordes = false;
                    jogoAtivo = false;
                    score = 0;
                    tempoVivo = 0;
                    gerador.Resetar();
                    PlaySound(somLigaCarro);
                    PlayMusicStream(musicaFundo);
                }
            }
        }
        else if (mostrarRecordes) {
            // Tela de Recordes
            ClearBackground(BLACK);
            
            const char* titulo = "TOP 10 RECORDES";
            int tamanhoFonte = 28;
            int larguraTexto = MeasureText(titulo, tamanhoFonte);
            int xTitulo = (GetScreenWidth() - larguraTexto) / 2;
            DrawText(titulo, xTitulo, 60, tamanhoFonte, RED);


            for (size_t i = 0; i < recordes.size(); ++i) {
                std::string linha = std::to_string(i + 1) + ". " + recordes[i].nome + " - " + std::to_string(recordes[i].score);
                DrawText(linha.c_str(), 220, 120 + i * 40, 22, RED);
            }

            Rectangle botaoVoltar = {GetScreenWidth() / 2.0f - 80, 600, 160, 50};
            DesenharBotaoArredondado(botaoVoltar, "MENU", fontePixel);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                    mostrarRecordes = false;
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(fundoMenu);
    UnloadTexture(texturaRodovia);
    UnloadFont(fontePixel);
    UnloadSound(somBuzina);
    UnloadSound(somBatida);
    UnloadSound(somRoncoAumentando);
    UnloadSound(somRoncoConstante);
    UnloadSound(somTrânsito);
    UnloadSound(somLigaCarro);
    UnloadMusicStream(musicaFundo);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void AtualizarScore() {
    tempoVivo += GetFrameTime();
    score = (int)(tempoVivo * 5);
}

// Carrega o recorde para o arquivo txt
void SalvarRecorde() {
    std::ofstream file("recordes.txt", std::ios::app);
    if (file.is_open()) {
        file << nomeJogador << "|" << score << std::endl;
        file.close();
    }
}

// Lê o txt para exibir na tela de recordes
void CarregarRecordes() {
    recordes.clear();
    std::ifstream file("recordes.txt");

    if (file.is_open()) {
        std::string linha;
        while (std::getline(file, linha)) {
            size_t separador = linha.rfind('|');
            if (separador != std::string::npos) {
                std::string nome = linha.substr(0, separador);
                int s = std::stoi(linha.substr(separador + 1));
                recordes.push_back({nome, s});
            }
        }
        file.close();
    }

    std::sort(recordes.begin(), recordes.end(), [](const Registro& a, const Registro& b) {
        return a.score > b.score;
    });

    if (recordes.size() > 10) recordes.resize(10);
}

// Tela de input do nome do jogador no começo do jogo
void DesenharEntradaNome(Font fontePixel) {
 
    Rectangle origem = { 0, 0, (float)fundoMenu.width, (float)fundoMenu.height };
    Rectangle destino = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origemCentro = { 0, 0 };
    DrawTexturePro(fundoMenu, origem, destino, origemCentro, 0.0f, WHITE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Título 
    const char* titulo = "DIGITE SEU NOME:";
    int tamanhoFonte = 20;
    int larguraTexto = MeasureText(titulo, tamanhoFonte);
    DrawText(titulo, (screenWidth - larguraTexto) / 2, screenHeight - 180, tamanhoFonte, RED);

    Rectangle caixaTexto = {
        screenWidth / 2.0f - 200,
        (float)(screenHeight - 140),
        400,
        50
    };
    DrawRectangleRec(caixaTexto, WHITE);
    DrawRectangleLinesEx(caixaTexto, 2, RED);

    DrawText(nomeJogador.c_str(), caixaTexto.x + 10, caixaTexto.y + 12, 24, BLACK);

    // Botão OK 
    Rectangle botaoOk = {
        screenWidth / 2.0f - 80,
        (float)(screenHeight - 70),
        160,
        50
    };
    DesenharBotaoArredondado(botaoOk, "OK", fontePixel);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, botaoOk) && !nomeJogador.empty()) {
            inserindoNome = false;
        }
    }

    // Input do nome
    int tecla = GetCharPressed();
    while (tecla > 0) {
        if ((tecla >= 32) && (tecla <= 125) && nomeJogador.length() < 12)
            nomeJogador += (char)tecla;
        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !nomeJogador.empty())
        nomeJogador.pop_back();
}

// Tela do menu principal do jogo
void DesenharMenu(Texture2D fundo, Font fontePixel) {
    Rectangle origem = {0, 0, (float)fundo.width, (float)fundo.height};
    Rectangle destino = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 origemCentro = {0, 0};

    DrawTexturePro(fundo, origem, destino, origemCentro, 0.0f, WHITE);

    // nome do jogador no topo 
    if (!inserindoNome) {
        DrawText(TextFormat("JOGADOR: %s", nomeJogador.c_str()), 20, 20, 20, MAROON);
    }

    // Botões do menu
    float larguraBotao = 160;
    float alturaBotao = 50;
    float espaco = 40;
    float centroX = GetScreenWidth() / 2.0f;
    float y = GetScreenHeight() - alturaBotao - 20;

    Rectangle botaoJogar = {centroX - larguraBotao - espaco / 2, y, larguraBotao, alturaBotao};
    Rectangle botaoRecordes = {centroX + espaco / 2, y, larguraBotao, alturaBotao};

    DesenharBotaoArredondado(botaoJogar, "JOGAR", fontePixel);
    DesenharBotaoArredondado(botaoRecordes, "RECORDES", fontePixel);

    // Se clicar em jogar...
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, botaoJogar) && !inserindoNome) {
            jogoAtivo = true;
            gameOver = false;
            tempoVivo = 0;
            score = 0;
            gerador.Resetar();
            roncoConstanteTocando = false;

            StopSound(somLigaCarro);
            PlaySound(somRoncoAumentando);
            PlaySound(somTrânsito);
        }

        if (CheckCollisionPointRec(mouse, botaoRecordes)) {
            CarregarRecordes();
            mostrarRecordes = true;
        }
    }
}

// Desenha o Game Over
void DesenharTelaGameOver(Font fontePixel) {
    ClearBackground(BLACK);
    
    const char* texto = "GAME OVER";
    int tamanho = 90;
    int larguraTexto = MeasureText(texto, tamanho);
    int x = (GetScreenWidth() - larguraTexto) / 2;

DrawText(texto, x, 240, tamanho, RED);

    Rectangle botaoMenu = {GetScreenWidth() / 2.0f - 80, 400, 160, 50};
    DesenharBotaoArredondado(botaoMenu, "MENU", fontePixel);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, botaoMenu)) {
            gameOver = false;
            mostrarRecordes = false;
            jogoAtivo = false;

            PlaySound(somLigaCarro);
            PlayMusicStream(musicaFundo);
        }
    }
}

// Layout dos botões dos menus
void DesenharBotaoArredondado(Rectangle ret, const char* texto, Font fontePixel) {
    Color corFundo = {250, 188, 45, 200};
    Color corBordaExterna = {195, 29, 43, 255};
    Color corBordaInterna = {221, 65, 45, 255};

    DrawRectangleRounded(ret, 0.5f, 10, corFundo);
    DrawRectangleRoundedLines(ret, 0.5f, 10, 4, corBordaExterna);

    Rectangle interno = {ret.x + 2, ret.y + 2, ret.width - 4, ret.height - 4};
    DrawRectangleRoundedLines(interno, 0.5f, 10, 3, corBordaInterna);

    float tamanho = 24;
    float espacamento = 2;
    Vector2 posTexto = {
        ret.x + (ret.width - MeasureTextEx(fontePixel, texto, tamanho, espacamento).x) / 2,
        ret.y + (ret.height - tamanho) / 2
    };

    DrawTextEx(fontePixel, texto, posTexto, tamanho, espacamento, corBordaExterna);
}