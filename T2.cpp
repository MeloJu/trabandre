#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

struct arquivo{ 
	int arq//pos do arq na lista de arqs processados
	vector<int> linhas;//linhas onde a palavra foi encontrada
};

struct palavra{
	string letras;//a palavra
	vector<int> oco;//lista contendo informações da ocorrencia de uma palavra em um arq
};

struct indice{
	vector<string> arq;//lista contendo os nomes dos arqs de texto ja processados
	vector<struct palavra> p;//lista contendo todas as palavras encontradas
};

void liberaind(struct indice i){
	i.arq.clear();
	for (int k=0; k<i.p.size(); k++){
		for (int j=0; j<i.p[k].oco.size(); j++)
		i.p[k].oco[j].linhas.clear();
	}
	i.p[k].clear();
}

void inserepalavra(string pal, int lin, struct indice &ind){
	//inserir em ordem alfabetica, linha da palavra tem de ser registrada
	int posicao=0; //posição onde a palavra deve entrar
	struct palavra pa;
	pa.oco.push_back(lin);//guarda ocorrencia
	pa.letras = pal;

    while (posicao < ind.p.size() && (pa.letras > ind.p[posicao].letras)) //se não chegou na posisção correta, continua
        posicao++;

    ind.p.insert(ind.p.begin() + posicao, pa);//insere na posição correta
}

bool palavraexiste(string palavra, struct indice &ind){
	//palavra existe na lista? 
	for(int i=0; i<ind.p.size(); i++) {
    	if (ind.p[i].letras==palavra){
    		return true;
		}
	}
	return false;
}

string removepun(string palavra){
	for (int i=0, len=palavra.size(); i<len; i++){
        if (ispunct(palavra[i])){
            palavra.erase(i--, 1);
            len = palavra.size();
        }
    }
    return palavra;
}

void processaLinha(struct indice &ind, string linha, int numLinha){
	struct palavra pa;
	stringstream ss(linha);
	//para cada palavra
	while(getline(ss, pa.letras, ' ')){
		//cout << "palavra: " << palavra <<endl;
		pa.letras = removepun(pa.letras); //remove pontuação
		//verificar se pal existe
		if (!palavraexiste(pa.letras, ind)){
			//se n existir, inserir a nova palavra na lista de palavra do indice //inserir tb a primeira ocorrencia dessa palavra(size do vector)
			inserepalavra(pa.letras, numLinha, ind);
		} else {
			//se existir, acrescentar nova ocorrencia a palavra
			for (int i= 0; i < ind.p.size(); i++){
    			if (ind.p[i].letras == pa.letras ){
    				ind.p[i].oco.push_back(numLinha);
    				break;
				}
			}
		}
	}
}

bool pexiste(vector<string> arq, string palavra){
    for (int i=0; i<arq.size(); i++)
        if (arq[i] == palavra)
            return true;
    return false;
}

void processaarq(struct indice &ind){
	//perguntar nome do arq txt
	string nomeArq, linha;
	cout << "Digite o nome do arquivo a ser processado:" << endl;
    cin >> nomeArq;
	//verificar se o arq n foi processado
	if (!pexiste(ind.arq, nomeArq)){
		//inserir nome do arq no indice
		ind.arq.push_back(nomeArq);
		//abrir arq
		fstream arq(nomeArq, fstream::in);
		//enquanto n acabou de ler
		int numlinha = 0;
		while(arq.eof()==0){
			//ler uma linha
			getline(arq, linha);
			processaLinha(ind, linha, numlinha);
			numlinha++;
		}
		//fechaarq
		arq.close();
	}
}

void escreveocorrencia(fstream &arq, struct ocorrencia oc){
	//escreve o num do arq binario
	//escreve as linhas onde a palavra apareceu no arq
}

void escrevepalavra(fstream &arq, struct palavra pal){
	//escreve no arq bin as letras da palavra
	// para cada ocorrencia
		escreveocorrencia(arq, pal.oco[j])
}

void escrevenomarq(fstream &arq, string nomearq){
	//escrever a qtd de letras
	//escreve as letras
}

void salvarindice(struct indice ind){
	//abrir o arq bin
	//escreve a qtd de arquivos
	//para cada arq de ind.arquivos
		escrevenomearq(arquivo, ind.arquivo[t]);
	//escreve a qtd de palavras
	//para cada palavra de ind.p
		escrevepalavra(arquivo, ind.p[i]);
	//fecha arq bin
		
}

void lerindice(struct indice &ind){
	liberaind(ind);
	//abrir arq bin
	//ler a qtd de arqs no indice
	//p cada arq
		lenomearq(arq, ind);
	//ler a qtd de palavras do indice
	//p cada palavra
		lepalavra(arq, ind);
	//fechar o arq bin
}

void mostraoco(struct palavra){
	//p cada ocorrencia
}

void mostrap(struct indice ind){
	//p cada palavra no ind
		//escreva a palavra na tela
		mostraoco(ind.p[i]);
}

void mostraind(struct indice ind){
	mostranomarq(ind);
	mostrap(ind);
}
int main(){
	struct indice ind;
	int op;
	op = Menu();
	while (op !=5){
		switch(op){
			case 1: processaarq(ind); break;
			case 2: salvarindice(ind); break;
			case 3: lerindice(ind); break;
			case 4: mostraind(ind); break;
		}
		op = Menu();
	}
	liberaind(ind);
}
