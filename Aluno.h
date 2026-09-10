#ifndef ALUNO_H
#define ALUNO_H

#include <string>

class Aluno {
public:
    int matricula;
    char nome[40];
    int idade;
    char curso[35];
    char cidade[30];
    char uf[3];
    float cra;

    Aluno();
    Aluno(int matricula, const char* nome, int idade, const char* curso, const char* cidade, const char* uf, float cra);

    int packFixo(char* buffer) const;
    std::string packDelimitado() const;
    int packIndicador(char* buffer) const;

    void unpackFixo(const char* buffer);
    void unpackDelimitado(const std::string& dados);
    void unpackIndicador(const char* buffer);

    int getBytesUteis() const;
    void imprimir() const;
};

#endif