#include"iostream"
#include"string"
#include"vector"
#include"fstream"
#include"sstream"

using namespace std;

typedef struct Ocorrencia{//struct Verde.
    int arquivo; //posição do arquivo na lista de arquivos processados.
    //qtdOcorrencias é o size do vector <int> linhas.
    vector <int> linhas; //linhas onde foram encontrada.
    //ponteiro para a próxima ocorrência substituído pelo vector <Ocorrencias> no struct palavra.
}Ocorrencia;

typedef struct Palavras{//struct Vermelho.
    string letras; //a palavra
    //qtdOcorrencias é o size do vector <Ocorrencias>
    vector <Ocorrencia> ocorrencias; //vetor com os arquivos onde a palavra foi lida.
    //ponteiro substituido pelo vector <Palavras> no struct Indice.
}Palavras;

typedef struct Indice{ //struct Amarelo.
    //qtdArquivos é o size do vector <Arquivo>
    vector <string> arquivosLidos; //vetor com o nome dos arquivos lidos //struct Roxo.
    //qtdPalavras é o size do vector <Palavras>
    vector <Palavras> palavras; //vetor contendo as todas as palavras lidas.
}Indice;

bool palavraexiste(Indice ind,string pl){
    int i;
    for(i=0;i<ind.palavras.size();i++){
        if(ind.palavras[i].letras.compare(pl)==0){
            return true;
        }
    }
    return false;
}

void InsereEmOrdem(Indice &ind,Palavras pala){
    int pos=0;
    while(pos<ind.palavras.size() && (pala.letras > ind.palavras[pos].letras)){
        pos++;
    }
    ind.palavras.insert(ind.palavras.begin() + pos, pala);
}

bool ExisteOcorrencia(Indice ind,string p){
    int i,j;
    for(i=0;i<ind.palavras.size();i++){
        if(ind.palavras[i].letras.compare(p)==0){
            for(int j=0;j<ind.palavras[i].ocorrencias.size();j++){
                if(ind.palavras[i].ocorrencias[j].arquivo==ind.arquivosLidos.size()){
                    return true;
                }
            }
        }
    }
    return false;
}

void NovaOcorrencia(Indice &ind,string pl,Ocorrencia oc){
    int i;
    for(i=0;i<ind.palavras.size();i++){
        if(ind.palavras[i].letras.compare(pl)==0){
            ind.palavras[i].ocorrencias.push_back(oc);
        }
    }
}

void AtualizaLinha(Indice &ind,string p,int linha){
    int i,j;
    for(i=0;i<ind.palavras.size();i++){
        if(ind.palavras[i].letras.compare(p)==0){
            for(int j=0;j<ind.palavras[i].ocorrencias.size();j++){
                if(ind.palavras[i].ocorrencias[j].arquivo==ind.arquivosLidos.size()){
                    ind.palavras[i].ocorrencias[j].linhas.push_back(linha);
                }
            }
        }
    }
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

void ProcessaLinha(Indice &ind,string linha,int numL){
     stringstream linhaLida(linha);
     string palavra;
     while(getline(linhaLida,palavra,' ')){
        palavra=removepun(palavra); ///remove pontuação
        if(!palavraexiste(ind,palavra)){ ///se n existir, inserir a nova palavra na lista de palavra do indice
            Ocorrencia aux;
            Palavras pl;
            aux.arquivo=ind.arquivosLidos.size();
            aux.linhas.push_back(numL);
            pl.ocorrencias.push_back(aux);
            pl.letras=palavra;
            InsereEmOrdem(ind,pl);
        }else{
            if(!ExisteOcorrencia(ind,palavra)){ ///se a palavra n tiver um struct ocorrencia
                Ocorrencia aux;
                aux.linhas.push_back(numL);
                aux.arquivo=ind.arquivosLidos.size();
                NovaOcorrencia(ind,palavra,aux);
            }else{
                AtualizaLinha(ind,palavra,numL); ///se a palavra ja tiver um struct ocorrencia, adiciona uma nova linha
            }
        }
     }
 }

bool FoiLido(string nomeArq,Indice ind){
    int i;
    for(i=0;i<ind.arquivosLidos.size();i++){
        if(ind.arquivosLidos[i].compare(nomeArq)==0){
            return true;
        }
    }
    return false;
}

void ProcessaArquivo(Indice &ind){
    string nomeA;
    cout<<"Digite o nome do arquivo: ";
    cin>>nomeA;
    if(!FoiLido(nomeA,ind)){ ///verificar se o arq n foi processado
        ///abrir arq
        fstream arq(nomeA,fstream::in);
        if(!arq.is_open()){
            cout<< "O arquivo "<<nomeA<<" nao foi encontrado."<<endl;
        }else{
            ind.arquivosLidos.push_back(nomeA);
            string linha;
            int numlinha=1;
            while(!arq.eof()){
                ///ler uma linha
                getline(arq,linha);
                ProcessaLinha(ind,linha,numlinha);
                numlinha++;
            }
            arq.close();
        }
    }else{
        cout<<"O arquivo "<<nomeA<<" ja foi lido"<<endl;
    }
}

void SalvarIndice(Indice &ind){
    fstream Arq;
    Arq.open("indice.dat",fstream::out);
    int totalArq,tamNomeArq,totalPalavras,tamLetrasP,ocorrenciaPalavra,numArq,qtdPalavrasArq,linhas;
    totalArq=ind.arquivosLidos.size();
    Arq.write((char*)&totalArq,sizeof(int));

    for(int i=0;i<totalArq;i++){
        tamNomeArq=ind.arquivosLidos[i].length() + 1;

        Arq.write((char*)&tamNomeArq,sizeof(int));
        Arq.write(ind.arquivosLidos[i].c_str(),tamNomeArq);
    }
    totalPalavras=ind.palavras.size();
    Arq.write((char*)&totalPalavras,sizeof(int));

    for(int i=0;i<totalPalavras;i++){
        tamLetrasP=ind.palavras[i].letras.length() + 1;
        ocorrenciaPalavra=ind.palavras[i].ocorrencias.size();
        Arq.write((char*)&tamLetrasP,sizeof(int));
        Arq.write(ind.palavras[i].letras.c_str(),tamLetrasP);
        Arq.write((char*)&ocorrenciaPalavra,sizeof(int));

        for(int j=0;j<ocorrenciaPalavra;j++){
            numArq=ind.palavras[i].ocorrencias[j].arquivo;
            qtdPalavrasArq=ind.palavras[i].ocorrencias[j].linhas.size();
            Arq.write((char*)&numArq,sizeof(int));
            Arq.write((char*)&qtdPalavrasArq,sizeof(int));

            for(int k=0;k<qtdPalavrasArq;k++){
                linhas=ind.palavras[i].ocorrencias[j].linhas[k];
                Arq.write((char*)&linhas,sizeof(int));
            }
        }
    }
    Arq.close();
}

void ImprimeLinhas(vector<int>l){
    for(int i=0;i<l.size();i++){
        cout<<l[i]<<" ";
    }
}

void ImprimeOcorrencias(vector<Ocorrencia>ocr){
    int i;
    for(i=0;i<ocr.size();i++){
        cout<<"arq: "<<ocr[i].arquivo<<endl<<"aparicoes nesse arquivo: "<<ocr[i].linhas.size()<<endl<<"linhas: ";
        ImprimeLinhas(ocr[i].linhas);
        cout<<endl;
    }
}

void ImprimeArqLidos(Indice ind){
    int i;
    for(i=0;i<ind.arquivosLidos.size();i++){
        cout<<ind.arquivosLidos[i]<<"  ";
    }
}

void MostraIndice(Indice ind){
    int i;
    ImprimeArqLidos(ind);
    cout<<endl<<endl<<endl;
    for(i=0;i<ind.palavras.size();i++){
        cout<<ind.palavras[i].letras<<" structs ocorrencias: "<<ind.palavras[i].ocorrencias.size()<<endl;
        ImprimeOcorrencias(ind.palavras[i].ocorrencias);
        cout<<endl;
    }
}

int Menu(){
    int resp;
    cout<<"[1] Processar arquvo"<<endl<<"[2] Salvar Indice"<<endl<<"[3] Ler Indice"<<endl<<"[4] Mostrar Indice atual"<<endl<<"[5] Encerrar"<<endl<<"Digite: ";
    cin>>resp;
    return resp;
}

int main(){
    Indice ind;
    int op;
    op = Menu();
    while(op!=5){
        switch(op){
            case 1: ProcessaArquivo(ind); break;
            case 2: SalvarIndice(ind); break;
            //case 3: LerDat(ind); break;
            case 4: MostraIndice(ind); break;
        }
        op = Menu();
    }
    return 0;
}
