#include "cadastrousuario.h"
#include "ui_cadastrousuario.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// Construtor da classe CadastroUsuario
CadastroUsuario::CadastroUsuario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CadastroUsuario)
{
    ui->setupUi(this);  // Configura a interface gráfica

    // Conectar o botão de "Registrar" ao slot que trata o clique
    connect(ui->registerButton, &QPushButton::clicked, this, &CadastroUsuario::onRegisterClicked);
}

// Destruidor da classe CadastroUsuario
CadastroUsuario::~CadastroUsuario()
{
    delete ui;  // Libera a memória da interface gráfica
}

// Slot que é chamado quando o botão "Registrar" é clicado
void CadastroUsuario::onRegisterClicked()
{
    // Obtém o nome de usuário e a senha digitados
    QString usuario = ui->usernameLineEdit->text().trimmed();  // Remove espaços em branco antes e depois do texto
    QString senha = ui->passwordLineEdit->text().trimmed();

    // Valida se os campos de usuário e senha não estão vazios
    if (usuario.isEmpty() || senha.isEmpty()) {
        ui->messageLabel->setText("Preencha todos os campos.");  // Exibe uma mensagem de erro
        return;
    }

    // Verifica se o nome de usuário já existe
    if (usuarioExiste(usuario)) {
        ui->messageLabel->setText("Usuário já existe!");  // Se o usuário já existe, exibe um aviso
        return;
    }

    // Salva o novo usuário no arquivo
    if (salvarUsuario(usuario, senha)) {
        ui->messageLabel->setStyleSheet("color: green; font-size: 16px; font-weight: bold;");  // Estiliza a mensagem de sucesso
        ui->messageLabel->setText("Usuário cadastrado com sucesso!");  // Exibe a mensagem de sucesso
        ui->usernameLineEdit->clear();  // Limpa o campo de usuário
        ui->passwordLineEdit->clear();  // Limpa o campo de senha
    } else {
        ui->messageLabel->setText("Erro ao salvar o usuário.");  // Exibe uma mensagem de erro se falhar ao salvar
    }
}

// Função que verifica se o nome de usuário já existe no arquivo JSON
bool CadastroUsuario::usuarioExiste(const QString &usuario)
{
    QFile file(usuariosPath);  // Abre o arquivo JSON que contém os usuários
    if (!file.open(QIODevice::ReadOnly)) return false;  // Se não conseguir abrir o arquivo para leitura, retorna falso

    QByteArray data = file.readAll();  // Lê todos os dados do arquivo
    file.close();  // Fecha o arquivo após a leitura

    QJsonDocument doc = QJsonDocument::fromJson(data);  // Converte os dados lidos em um documento JSON
    QJsonArray usuarios = doc.array();  // Converte o documento em um array de objetos JSON (usuários)

    // Percorre todos os usuários no arquivo JSON
    for (const QJsonValue &val : usuarios) {
        if (val.toObject().value("usuario").toString() == usuario) {  // Verifica se o nome de usuário já existe
            return true;  // Retorna verdadeiro se o usuário já existir
        }
    }
    return false;  // Retorna falso caso o usuário não tenha sido encontrado
}

// Função que salva o novo usuário no arquivo JSON
bool CadastroUsuario::salvarUsuario(const QString &usuario, const QString &senha)
{
    QFile file(usuariosPath);  // Abre o arquivo onde os usuários são armazenados
    QJsonArray usuarios;  // Cria um array para armazenar os dados dos usuários

    // Se o arquivo já existe, lê os dados existentes
    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly)) return false;  // Abre o arquivo para leitura
        QByteArray data = file.readAll();  // Lê os dados do arquivo
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);  // Converte os dados em um documento JSON
        usuarios = doc.array();  // Converte o documento em um array de usuários
    }

    // Cria um novo objeto JSON para o novo usuário
    QJsonObject novoUsuario;
    novoUsuario["usuario"] = usuario;  // Adiciona o nome de usuário
    novoUsuario["senha"] = senha;  // Adiciona a senha do usuário

    usuarios.append(novoUsuario);  // Adiciona o novo usuário ao array de usuários

    // Abre o arquivo novamente, agora para escrita
    QJsonDocument novoDoc(usuarios);  // Cria um documento JSON com o array atualizado
    if (!file.open(QIODevice::WriteOnly)) return false;  // Se não conseguir abrir o arquivo para escrita, retorna falso
    file.write(novoDoc.toJson());  // Escreve o novo conteúdo JSON no arquivo
    file.close();  // Fecha o arquivo após escrever

    return true;  // Retorna verdadeiro se o usuário foi salvo com sucesso
}
