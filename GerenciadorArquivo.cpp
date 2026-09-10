#include "GerenciadorArquivo.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdint>

using namespace std;

vector<Aluno> GerenciadorArquivo::lerCSV(const string& caminho) {
    vector<Aluno> alunos;
    ifstream in(caminho);
    if (!in.is_open()) return alunos;

    string linha;
    getline(in, linha);

    while (getline(in, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        string item, tempNome, tempCurso, tempCidade, tempUf;
        Aluno a;

        getline(ss, item, ','); a.matricula = stoi(item);
        
        getline(ss, tempNome, ',');
        strncpy(a.nome, tempNome.c_str(), sizeof(a.nome) - 1);
        a.nome[sizeof(a.nome)-1] = '\0';
        
        getline(ss, item, ','); a.idade = stoi(item);
        
        getline(ss, tempCurso, ',');
        strncpy(a.curso, tempCurso.c_str(), sizeof(a.curso) - 1);
        a.curso[sizeof(a.curso)-1] = '\0';
        
        getline(ss, tempCidade, ',');
        strncpy(a.cidade, tempCidade.c_str(), sizeof(a.cidade) - 1);
        a.cidade[sizeof(a.cidade)-1] = '\0';
        
        getline(ss, tempUf, ',');
        strncpy(a.uf, tempUf.c_str(), sizeof(a.uf) - 1);
        a.uf[sizeof(a.uf)-1] = '\0';
        
        getline(ss, item); a.cra = stof(item);

        alunos.push_back(a);
    }
    return alunos;
}

bool GerenciadorArquivo::salvarFixo(const string& arq, const vector<Aluno>& al) {
    ofstream out(arq, ios::binary);
    if (!out.is_open()) return false;

    char buffer[120];
    for (const auto& a : al) {
        a.packFixo(buffer);
        out.write(reinterpret_cast<char*>(buffer), 120); // Regra 3 do PDF aplicadas
    }
    return true;
}

vector<Aluno> GerenciadorArquivo::lerFixo(const string& arq) {
    vector<Aluno> alunos;
    ifstream in(arq, ios::binary);
    if (!in.is_open()) return alunos;

    char buffer[120];
    while (in.read(reinterpret_cast<char*>(buffer), 120)) {
        Aluno a;
        a.unpackFixo(buffer);
        alunos.push_back(a);
    }
    return alunos;
}

bool GerenciadorArquivo::lerPorRRN(const string& arq, int rrn, Aluno& out) {
    ifstream in(arq, ios::binary);
    if (!in.is_open()) return false;

    in.seekg(rrn * 120, ios::beg);

    char buffer[120];
    if (in.read(reinterpret_cast<char*>(buffer), 120)) {
        out.unpackFixo(buffer);
        return true;
    }
    return false;
}

long long GerenciadorArquivo::obterTamanhoArquivo(const string& arq) {
    ifstream in(arq, ios::binary | ios::ate);
    if (!in.is_open()) return 0;
    return in.tellg();
}

bool GerenciadorArquivo::salvarDelimitado(const string& arq, const vector<Aluno>& al) {
    ofstream out(arq, ios::binary);
    if (!out.is_open()) return false;

    for (const auto& a : al) {
        string reg = a.packDelimitado();
        out.write(reinterpret_cast<const char*>(reg.c_str()), reg.size());
    }
    return true;
}

vector<Aluno> GerenciadorArquivo::lerDelimitado(const string& arq) {
    vector<Aluno> alunos;
    ifstream in(arq, ios::binary);
    if (!in.is_open()) return alunos;

    string registro;
    while (getline(in, registro, '|')) {
        if (registro.empty()) continue;
        Aluno a;
        a.unpackDelimitado(registro); 
        alunos.push_back(a);
    }
    return alunos;
}

bool GerenciadorArquivo::salvarIndicador(const string& arq, const vector<Aluno>& al) {
    ofstream out(arq, ios::binary);
    if (!out.is_open()) return false;

    char buffer[512];
    for (const auto& a : al) {
        int tamTotal = a.packIndicador(buffer);
        out.write(reinterpret_cast<char*>(buffer), tamTotal);
    }
    return true;
}

vector<Aluno> GerenciadorArquivo::lerIndicador(const string& arq) {
    vector<Aluno> alunos;
    ifstream in(arq, ios::binary);
    if (!in.is_open()) return alunos;

    while (in.peek() != EOF) {
        uint16_t tamCorpo = 0;
        
        if (!in.read(reinterpret_cast<char*>(&tamCorpo), sizeof(uint16_t))) break;

        vector<char> buffer(tamCorpo + 2);
        memcpy(buffer.data(), &tamCorpo, 2); 
        
        in.read(reinterpret_cast<char*>(buffer.data() + 2), tamCorpo);

        Aluno a;
        a.unpackIndicador(buffer.data());
        alunos.push_back(a);
    }
    return alunos;
}