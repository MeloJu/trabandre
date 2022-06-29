#include"iostream"
#include"string"
#include"vector"
#include"fstream"
#include"sstream"
#include"windows.h"

/*
integrantes do grupo:
felipe bastos;
gabriel moura;
juan melo;
*/

using namespace std;

typedef struct Ocorrencia {
    int arquivo;
    vector <int> linhas;
} Ocorrencia;

typedef struct Palavras {
    string letras;
    vector <Ocorrencia> ocorrencias;
} Palavras;

typedef struct Indice {
    vector <string> arquivosLidos;
    vector <vector <Palavras>> palavras;
} Indice;

bool palavraexiste(Indice ind,string pl) {
    char LetInicial=pl.at(0);
    int i, pos=LetInicial-'a';
    for(i=0; i<ind.palavras[pos].size(); i++) {
        if(ind.palavras[pos][i].letras.compare(pl)==0) {
            return true;
        }
    }
    return false;
}

void InsereEmOrdem(Indice &ind,Palavras pala) {
    char LetInicial=pala.letras.at(0);
    int pos=LetInicial-'a';
    int i=0;
    while(i<ind.palavras[pos].size() && pala.letras > ind.palavras[pos][i].letras) {
        i++;
    }
    ind.palavras[pos].insert(ind.palavras[pos].begin() + i, pala);
}

bool ExisteOcorrencia(Indice ind,string p) {
    char LetInicial=p.at(0);
    int i,j, pos=LetInicial-'a';
    for(i=0; i<ind.palavras[pos].size(); i++) {
        if(ind.palavras[pos][i].letras.compare(p)==0) {
            for(int j=0; j<ind.palavras[pos][i].ocorrencias.size(); j++) {
                if(ind.palavras[pos][i].ocorrencias[j].arquivo==ind.arquivosLidos.size()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void NovaOcorrencia(Indice &ind,string pl,Ocorrencia oc) {
    char LetInicial=pl.at(0);
    int i,pos=LetInicial-'a';
    for(i=0; i<ind.palavras[pos].size(); i++) {
        if(ind.palavras[pos][i].letras.compare(pl)==0) {
            ind.palavras[pos][i].ocorrencias.push_back(oc);
        }
    }
}

void AtualizaLinha(Indice &ind,string p,int linha) {
    char LetInicial=p.at(0);
    int i,j,pos=LetInicial-'a';
    for(i=0; i<ind.palavras[pos].size(); i++) {
        if(ind.palavras[pos][i].letras.compare(p)==0) {
            for(int j=0; j<ind.palavras[pos][i].ocorrencias.size(); j++) {
                if(ind.palavras[pos][i].ocorrencias[j].arquivo==ind.arquivosLidos.size()) {
                    ind.palavras[pos][i].ocorrencias[j].linhas.push_back(linha);
                }
            }
        }
    }
}

string removepun(string palavra) {
    for (int i=0, len=palavra.size(); i<len; i++) {
        if (ispunct(palavra[i])) {
            palavra.erase(i--, 1);
            len = palavra.size();
        }
    }
    return palavra;
}

bool jogaforapun(string palavra) {
    int flag=0;
    for (int i=0, len=palavra.size(); i<len; i++) {
        if(ispunct(palavra[i]) ) {
            flag++;
        }
    }
    if (flag == palavra.size()) {
        return false;
    } else {
        return true;
    }
}

void ProcessaLinha(Indice &ind,string linha,int numL) {
    stringstream linhaLida(linha);
    string palavra;
    while(getline(linhaLida,palavra,' ')) {
        if (jogaforapun(palavra)) {
            palavra=removepun(palavra);
            if(!palavraexiste(ind,palavra)) {
                Ocorrencia aux;
                Palavras pl;
                aux.arquivo=ind.arquivosLidos.size();
                aux.linhas.push_back(numL);
                pl.ocorrencias.push_back(aux);
                pl.letras=palavra;
                InsereEmOrdem(ind,pl);
            } else {
                if(!ExisteOcorrencia(ind,palavra)) {
                    Ocorrencia aux;
                    aux.linhas.push_back(numL);
                    aux.arquivo=ind.arquivosLidos.size();
                    NovaOcorrencia(ind,palavra,aux);
                } else {
                    AtualizaLinha(ind,palavra,numL);
                }
            }
        }
    }
}

bool FoiLido(string nomeArq,Indice ind) {
    int i;
    for(i=0; i<ind.arquivosLidos.size(); i++) {
        if(ind.arquivosLidos[i].compare(nomeArq)==0) {
            return true;
        }
    }
    return false;
}

void ProcessaArquivo(Indice &ind) {
    string nomeA;
    cout<<"Digite o nome do arquivo: ";
    cin>>nomeA;
    if(!FoiLido(nomeA,ind)) {
        fstream arq(nomeA,fstream::in);
        if(!arq.is_open()) {
            cout<< "O arquivo "<<nomeA<<" nao foi encontrado."<<endl;
        } else {
            ind.arquivosLidos.push_back(nomeA);
            string linha;
            int numlinha=1;
            while(!arq.eof()) {
                getline(arq,linha);
                ProcessaLinha(ind,linha,numlinha);
                numlinha++;
            }
            arq.close();
        }
    } else {
        cout<<"O arquivo "<<nomeA<<" ja foi lido"<<endl;
    }
}

void SalvarIndice(Indice &ind) {
    if(ind.arquivosLidos.empty()) {
        cout<<"Indice vazio, nada para salvar."<<endl;
    } else {
        fstream Arq;
        Arq.open("indice.dat",fstream::out|fstream::binary);
        int totalArq,tamNomeArq,totalPalavras,tamLetrasP,ocorrenciaPalavra,numArq,qtdPalavrasArq,linhas;
        totalArq=ind.arquivosLidos.size();
        Arq.write((char*)&totalArq,sizeof(int));

        for(int i=0; i<totalArq; i++) {
            tamNomeArq=ind.arquivosLidos[i].length() + 1;
            Arq.write((char*)&tamNomeArq,sizeof(int));
            Arq.write(ind.arquivosLidos[i].c_str(),sizeof(char)*tamNomeArq);
        }

        for(int g=0;g<ind.palavras.size();g++){
            totalPalavras=ind.palavras[g].size();
            Arq.write((char*)&totalPalavras,sizeof(int));

            for(int i=0; i<totalPalavras; i++) {
                tamLetrasP=ind.palavras[g][i].letras.length() + 1;
                Arq.write((char*)&tamLetrasP,sizeof(int));
                Arq.write(ind.palavras[g][i].letras.c_str(),sizeof(char)*tamLetrasP);
                ocorrenciaPalavra=ind.palavras[g][i].ocorrencias.size();
                Arq.write((char*)&ocorrenciaPalavra,sizeof(int));

                for(int j=0; j<ocorrenciaPalavra; j++) {
                    numArq=ind.palavras[g][i].ocorrencias[j].arquivo;
                    qtdPalavrasArq=ind.palavras[g][i].ocorrencias[j].linhas.size();
                    Arq.write((char*)&numArq,sizeof(int));
                    Arq.write((char*)&qtdPalavrasArq,sizeof(int));

                    for(int k=0; k<qtdPalavrasArq; k++) {
                        linhas=ind.palavras[g][i].ocorrencias[j].linhas[k];
                        Arq.write((char*)&linhas,sizeof(int));
                    }
                }
            }
        }
        Arq.close();
        cout<<"O indice foi salvo com sucesso."<<endl;
    }
}

void LiberaMemoria(Indice &ind) {
    ind.arquivosLidos.clear();
    for(int g=0;g<ind.palavras.size();g++){
        for(int i=0; i<ind.palavras[g].size(); i++) {
            for(int j=0; j<ind.palavras[g][i].ocorrencias.size(); j++) {
                ind.palavras[g][i].ocorrencias[j].linhas.clear();
            }
        }
        ind.palavras[g].clear();
    }
    ind.palavras.clear();
}

void LerIndice(Indice &ind) {
    if(!ind.arquivosLidos.empty()) {
        LiberaMemoria(ind);
    }
    fstream Arq;
    Arq.open("indice.dat",fstream::in|fstream::binary);
    int totalArq,tamNomeArq,totalPalavras,tamLetrasP,ocorrenciaPalavra,numArq,qtdPalavrasArq,linha;
    string nomeArq;
    Arq.read((char*)&totalArq,sizeof(int));

    for(int i=0; i<totalArq; i++) {
        Arq.read((char*)&tamNomeArq,sizeof(int));
        nomeArq.resize(tamNomeArq-1);
        Arq.read(&nomeArq[0],sizeof(char)*tamNomeArq);
        ind.arquivosLidos.push_back(nomeArq);;
    }

    for(int h=0;h<ind.palavras.size();h++){
        Arq.read((char*)&totalPalavras,sizeof(int));

        for(int i=0; i<totalPalavras; i++) {
            Palavras plvAux;
            Arq.read((char*)&tamLetrasP,sizeof(int));
            plvAux.letras.resize(tamLetrasP-1);
            Arq.read(&plvAux.letras[0],sizeof(char)*tamLetrasP);
            Arq.read((char*)&ocorrenciaPalavra,sizeof(int));

            for(int j=0; j<ocorrenciaPalavra; j++) {
                Ocorrencia ocrAux;
                Arq.read((char*)&numArq,sizeof(int));
                Arq.read((char*)&qtdPalavrasArq,sizeof(int));
                ocrAux.arquivo=numArq;

                for(int k=0; k<qtdPalavrasArq; k++) {
                    Arq.read((char*)&linha,sizeof(int));
                    ocrAux.linhas.push_back(linha);
                }
                plvAux.ocorrencias.push_back(ocrAux);
            }
            ind.palavras[h].push_back(plvAux);
        }
    }

    Arq.close();
    cout<<"O indice foi lido com sucesso."<<endl;
}

void ImprimeLinhas(vector<int>l) {
    for(int i=0; i<l.size(); i++) {
        cout<<l[i]<<" ";
    }
}

void ImprimeOcorrencias(vector<Ocorrencia>ocr) {
    int i;
    for(i=0; i<ocr.size(); i++) {
        cout<<"arq: "<<ocr[i].arquivo<<endl<<"aparicoes nesse arquivo: "<<ocr[i].linhas.size()<<endl<<"linhas: ";
        ImprimeLinhas(ocr[i].linhas);
        cout<<endl;
    }
}

void ImprimeArqLidos(Indice ind) {
    int i;
    for(i=0; i<ind.arquivosLidos.size(); i++) {
        cout<<ind.arquivosLidos[i]<<"  ";
    }
}

void MostraIndice(Indice ind) {
    int i,j;
    cout<<endl<<"Arquivos lidos: ";
    ImprimeArqLidos(ind);
    cout<<endl<<endl<<endl;
    for(i=0; i<ind.palavras.size(); i++) {
        char Letra='a'+i;
        cout<<"Lista da letra ( "<<Letra<<" )"<<endl<<endl;
        for(j=0;j<ind.palavras[i].size();j++){
            cout<<ind.palavras[i][j].letras<<"\t| structs ocorrencias: "<<ind.palavras[i][j].ocorrencias.size()<<endl;
            ImprimeOcorrencias(ind.palavras[i][j].ocorrencias);
            cout<<endl;
        }
    }
}

int Menu() {
    int resp;
    cout<<"[1] Processar arquivo"<<endl<<"[2] Salvar Indice"<<endl<<"[3] Ler Indice"<<endl<<"[4] Mostrar Indice atual"<<endl<<"[5] Encerrar"<<endl<<"Digite: ";
    cin>>resp;
    return resp;
}

int main() {
    SetConsoleOutputCP(65001);
    Indice ind;
    ind.palavras.resize(26);

    int op;
    op = Menu();
    while(op!=5) {
        switch(op) {
        case 1:
            ProcessaArquivo(ind);
            system("pause");
            break;
        case 2:
            SalvarIndice(ind);
            system("pause");
            break;
        case 3:
            LerIndice(ind);
            system("pause");
            break;
        case 4:
            MostraIndice(ind);
            system("pause");
            break;
        }
        system("cls");
        op = Menu();
    }
    LiberaMemoria(ind);

    return 0;
}
