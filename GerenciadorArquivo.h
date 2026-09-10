#ifndef GERENCIADORARQUIVO_H
#define GERENCIADORARQUIVO_H

#include "Aluno.h"
#include <vector>
#include <string>

class GerenciadorArquivo {
public:
    static std::vector<Aluno> lerCSV(const std::string& caminho);
    
    static bool salvarFixo(const std::string& arq, const std::vector<Aluno>& al);
    static std::vector<Aluno> lerFixo(const std::string& arq);
    static bool lerPorRRN(const std::string& arq, int rrn, Aluno& out);
    
    static bool salvarDelimitado(const std::string& arq, const std::vector<Aluno>& al);
    static std::vector<Aluno> lerDelimitado(const std::string& arq);
    
    static bool salvarIndicador(const std::string& arq, const std::vector<Aluno>& al);
    static std::vector<Aluno> lerIndicador(const std::string& arq);
    
    static long long obterTamanhoArquivo(const std::string& arq);
};

#endif