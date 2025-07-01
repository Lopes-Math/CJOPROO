#ifndef CADASTROUSUARIO_H
#define CADASTROUSUARIO_H

#include <QDialog>

namespace Ui {
class CadastroUsuario;
}

// Tela de cadastro de usuário
class CadastroUsuario : public QDialog
{
    Q_OBJECT

public:
    CadastroUsuario(QWidget *parent = nullptr);
    ~CadastroUsuario();

private slots:
    void onRegisterClicked(); // Acionado quando o botão de cadastro é clicado

private:
    Ui::CadastroUsuario *ui;
    QString usuariosPath = "usuarios.json"; // Arquivo onde os usuários ficam salvos
    bool salvarUsuario(const QString &usuario, const QString &senha); // Salva um novo usuário
    bool usuarioExiste(const QString &usuario); // Verifica se o usuário já existe
};

#endif // CADASTROUSUARIO_H
