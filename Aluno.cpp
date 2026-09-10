#include "Aluno.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

Aluno::Aluno() {};

Aluno::Aluno(int matricula, const char* nome, int idade, const char* curso, const char* cidade, const char* uf, float cra) {
    this->matricula = matricula;
    
    memset(this->nome, 0, sizeof(this->nome));
    strncpy(this->nome, nome, sizeof(this->nome) - 1);
    
    this->idade = idade;
    
    memset(this->curso, 0, sizeof(this->curso));
    strncpy(this->curso, curso, sizeof(this->curso) - 1);
    
    memset(this->cidade, 0, sizeof(this->cidade));
    strncpy(this->cidade, cidade, sizeof(this->cidade) - 1);
    
    memset(this->uf, 0, sizeof(this->uf));
    strncpy(this->uf, uf, sizeof(this->uf) - 1);
    
    this->cra = cra;
}

int Aluno::packFixo(char* buffer) const {
    int offset = 0;

    memcpy(buffer + offset, &this->matricula, sizeof(this->matricula));
    offset += sizeof(this->matricula);

    memcpy(buffer + offset, this->nome, sizeof(this->nome));
    offset += sizeof(this->nome);

    memcpy(buffer + offset, &this->idade, sizeof(this->idade));
    offset += sizeof(this->idade);

    memcpy(buffer + offset, this->curso, sizeof(this->curso));
    offset += sizeof(this->curso);

    memcpy(buffer + offset, this->cidade, sizeof(this->cidade));
    offset += sizeof(this->cidade);

    memcpy(buffer + offset, this->uf, sizeof(this->uf));
    offset += sizeof(this->uf);

    memcpy(buffer + offset, &this->cra, sizeof(this->cra));

    return offset;
}

string Aluno::packDelimitado() const {
    ostringstream ss;
    ss << matricula << "#"
       << nome << "#"
       << idade << "#"
       << curso << "#"
       << cidade << "#"
       << uf << "#"
       << cra << "|";
    return ss.str();
}

int Aluno::packIndicador(char* buffer) const {
    int offset = 0;
    offset += 2; 

    memcpy(buffer + offset, &this->matricula, sizeof(this->matricula));
    offset += sizeof(this->matricula);

    unsigned char lenNome = strlen(this->nome);
    memcpy(buffer + offset, &lenNome, 1);
    offset += 1;
    memcpy(buffer + offset, this->nome, lenNome);
    offset += lenNome;

    memcpy(buffer + offset, &this->idade, sizeof(this->idade));
    offset += sizeof(this->idade);

    unsigned char lenCurso = strlen(this->curso);
    memcpy(buffer + offset, &lenCurso, 1);
    offset += 1;
    memcpy(buffer + offset, this->curso, lenCurso);
    offset += lenCurso;

    unsigned char lenCidade = strlen(this->cidade);
    memcpy(buffer + offset, &lenCidade, 1);
    offset += 1;
    memcpy(buffer + offset, this->cidade, lenCidade);
    offset += lenCidade;

    unsigned char lenUf = strlen(this->uf);
    memcpy(buffer + offset, &lenUf, 1);
    offset += 1;
    memcpy(buffer + offset, this->uf, lenUf);
    offset += lenUf;

    memcpy(buffer + offset, &this->cra, sizeof(this->cra));
    offset += sizeof(this->cra);

    unsigned short tamanhoCorpo = (unsigned short)(offset - 2);
    memcpy(buffer, &tamanhoCorpo, 2);

    return offset;
}

void Aluno::unpackFixo(const char* buffer) {
    int offset = 0;

    memcpy(&this->matricula, buffer + offset, sizeof(this->matricula));
    offset += sizeof(this->matricula);

    memcpy(this->nome, buffer + offset, sizeof(this->nome));
    offset += sizeof(this->nome);

    memcpy(&this->idade, buffer + offset, sizeof(this->idade));
    offset += sizeof(this->idade);

    memcpy(this->curso, buffer + offset, sizeof(this->curso));
    offset += sizeof(this->curso);

    memcpy(this->cidade, buffer + offset, sizeof(this->cidade));
    offset += sizeof(this->cidade);

    memcpy(this->uf, buffer + offset, sizeof(this->uf));
    offset += sizeof(this->uf);

    memcpy(&this->cra, buffer + offset, sizeof(this->cra));
    offset += sizeof(this->cra);
}

void Aluno::unpackDelimitado(const string& dadosStr) {
    string dados = dadosStr;

    if(!dados.empty() && dados.back() == '|'){
        dados.pop_back();
    }

    stringstream ss(dados);
    string token;

    getline(ss, token, '#');
    this->matricula = stoi(token);

    getline(ss, token, '#');
    strncpy(this->nome, token.c_str(), sizeof(this->nome));
    this->nome[sizeof(this->nome)-1] = '\0';

    getline(ss, token, '#');
    this->idade = stoi(token);

    getline(ss, token, '#');
    strncpy(this->curso, token.c_str(), sizeof(this->curso));
    this->curso[sizeof(this->curso) - 1] = '\0';

    getline(ss, token, '#');
    strncpy(this->cidade, token.c_str(), sizeof(this->cidade));
    this->cidade[sizeof(this->cidade) - 1] = '\0';

    getline(ss, token, '#');
    strncpy(this->uf, token.c_str(), sizeof(this->uf));
    this->uf[sizeof(this->uf) - 1] = '\0';

    getline(ss, token, '#');
    this->cra = stof(token);
}

void Aluno::unpackIndicador(const char* buffer) {
    int offset = 0;
    offset += 2; 
    memcpy(&this->matricula, buffer + offset, sizeof(this->matricula));
    offset += sizeof(this->matricula);

    unsigned char lenNome;
    memcpy(&lenNome, buffer + offset, 1);
    offset += 1;
    memcpy(this->nome, buffer + offset, lenNome);
    this->nome[lenNome] = '\0';
    offset += lenNome;

    memcpy(&this->idade, buffer + offset, sizeof(this->idade));
    offset += sizeof(this->idade);

    unsigned char lenCurso;
    memcpy(&lenCurso, buffer + offset, 1);
    offset += 1;
    memcpy(this->curso, buffer + offset, lenCurso);
    this->curso[lenCurso] = '\0';
    offset += lenCurso;

    unsigned char lenCidade;
    memcpy(&lenCidade, buffer + offset, 1);
    offset += 1;
    memcpy(this->cidade, buffer + offset, lenCidade);
    this->cidade[lenCidade] = '\0';
    offset += lenCidade;

    unsigned char lenUf;
    memcpy(&lenUf, buffer + offset, 1);
    offset += 1;
    memcpy(this->uf, buffer + offset, lenUf);
    this->uf[lenUf] = '\0';
    offset += lenUf;

    memcpy(&this->cra, buffer + offset, sizeof(this->cra));
}

int Aluno::getBytesUteis() const {
    int total = 2; 
    total += sizeof(this->matricula);
    total += 1 + strlen(this->nome);
    total += sizeof(this->idade);
    total += 1 + strlen(this->curso);
    total += 1 + strlen(this->cidade);
    total += 1 + strlen(this->uf);
    total += sizeof(this->cra);
    return total;
}

void Aluno::imprimir() const {
    cout << "Matricula: " << this->matricula << "\n"
         << "Nome: " << this->nome << "\n"
         << "Idade: " << this->idade << "\n"
         << "Curso: " << this->curso << "\n"
         << "Cidade: " << this->cidade << "\n"
         << "UF: " << this->uf << "\n"
         << "CRA: " << this->cra << "\n"
         << "------------------------\n";
}