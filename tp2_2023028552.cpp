#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

typedef struct{
    int linha;
    int coluna;
}pivo;

typedef struct{
    double valor;
    int certificado;
}certificado;


typedef struct{
    double** A;
    double* c;
    int* base;
    int n;
    int m;

    void print(string nome){
        cout << "Tabela Simplex: " << nome << endl;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m + 1; j++){
                cout << A[i][j] << " ";
            }
            cout << endl;
        }
        cout << "Função objetivo: ";
        for(int j = 0; j < m + 1; j++){
            cout << c[j] << " ";
        }
        cout << endl;
    }

    pivo find_pivot(){
        pivo p;
        p.coluna = -1;
        p.linha = -1;

        double valor_mais_negativo = 0.0;
        for(int j = 0; j < m; j++){
            if(c[j] < valor_mais_negativo){
                valor_mais_negativo = c[j];
                p.coluna = j;
            }
        }
        if (p.coluna == -1){
            return p;
        }
        double razao_minima = std::numeric_limits<double>::infinity();
        p.linha = -1;
        for(int i = 0; i < n; i++){
            if(A[i][p.coluna] > 0){
                double razao = A[i][m - 1] / A[i][p.coluna];
                if(razao < razao_minima){
                    razao_minima = razao;
                    p.linha = i;
                }
            }
        }
        if (p.linha == -1){
            return p;
        }
        return p;
    }

    void canonize(){
        for (int i = 0; i < n; i++) {
            int linha_base = i;
            int coluna_base = base[i];
            pivo p = {linha_base, coluna_base};
            pivot(p);
        }
    }

    void pivot(pivo p){
        double fator = A[p.linha][p.coluna];
        double objetive_fator = c[p.coluna];
        for(int j = 0; j < m + 1; j++){
            A[p.linha][j] /= fator;
            c[j] -= objetive_fator * A[p.linha][j];
        }
        for(int i = 0; i < n; i++){
            if(i != p.linha){
                fator = A[i][p.coluna];
                if (fator == 0) break;
                for(int j = 0; j < m + 1; j++){
                    A[i][j] -= fator * A[p.linha][j];
                }
            }
        }
        base[p.linha] = p.coluna;
    }

}tableu;

tableu leitor(string arquivo){
    ifstream in(arquivo);
    if (!in) {
        cerr << "Não foi possível abrir o arquivo: " << arquivo << '\n';
    }
    
    int n, m;
    if (!(in >> n >> m)) {
        cerr << "Falha ao ler n e m da primeira linha\n";
    }
    tableu T;
    T.n = n;
    T.m = m;

    T.c = new double[T.m+1];
    double v;
    for(int i = 0; i < T.m; i++){
        if (!(in >> v)) {
            cerr << "Falha ao ler os coeficientes da função objetivo\n";
        }
        T.c[i] = v;
    }
    T.c[T.m] = 0; // Termo independente da função objetivo

    T.A = new double*[T.n];
    for (int i = 0; i < T.n; i++) {
        T.A[i] = new double[T.m+1];
    }
    for(int i = 0; i < T.n; i++){
        for(int j = 0; j < T.m+1; j++){
            if (!(in >> T.A[i][j])) {
                cerr << "Falha ao ler os coeficientes das restrições\n";
            }
        }
    }
    for(int i = 0; i < n; i++){
        if(T.A[i][m] < 0){
            for(int j = 0; j < m + 1; j++){
                T.A[i][j] = -T.A[i][j];
            }
        }
    }
    return T;
}

class Simplex{
    public:
    tableu Original;
    tableu Auxiliar;
    Simplex(tableu T){
        Original = T;
        tableu_auxiliar();
    }

    void tableu_auxiliar(){
        int n = Original.n;
        int m = Original.m;
        Auxiliar.n = n;
        Auxiliar.m = m + n;
        Auxiliar.A = new double*[n];
        for (int i = 0; i < n; i++) {
            Auxiliar.A[i] = new double[Auxiliar.m + 1];
            for (int j = 0; j < m; j++) {
                Auxiliar.A[i][j] = Original.A[i][j];
            }
            for (int j = m; j < m + n; j++) {
                Auxiliar.A[i][j] = (j - m == i) ? 1.0 : 0.0; // Adiciona variáveis de folga
            }
            Auxiliar.A[i][m + n] = Original.A[i][m]; // Termo independente
        }
        Auxiliar.c = new double[m + n + 1];
        for (int j = 0; j < m; j++) {
            Auxiliar.c[j] = 0.0; // Coeficientes das variáveis originais
        }
        for (int j = m; j < m + n; j++) {
            Auxiliar.c[j] = - 1.0; // Coeficientes das variáveis de folga
        }
        Auxiliar.c[m + n] = 0.0; // Termo independente da função objetivo

        Auxiliar.base = new int[n];
        for (int i = 0; i < n; i++) {
            Auxiliar.base[i] = m + i; // As variáveis de folga são as básicas
        }
    }

    void iteracao(){
        Original.print("Original");
        Auxiliar.print("Auxiliar");
        Auxiliar.canonize();
        Auxiliar.print("Auxiliar Canonizado");
        pivo p;
        certificado cert;

        for(int i = 0; i < Auxiliar.m+1; i++){
            Auxiliar.c[i] *= -1;
        }
        int i = 0;
        while (true){
            Auxiliar.print("Iteração " + to_string(i++));
            p = Auxiliar.find_pivot();

            if (p.coluna == -1){
                Original.base = Auxiliar.base;
                break;
            }else if (p.linha == -1){
                break;
            }else{
                Auxiliar.pivot(p);
            }
        }
        
        if (Auxiliar.c[Auxiliar.m] != 0){
            cert.valor = -std::numeric_limits<double>::infinity();
            cert.certificado = 0;
        }else{
            Original.canonize();
            for(int i = 0; i < Original.m; i++){
                Original.c[i] *= -1;
            }
            while (true){
                p = Original.find_pivot();

                if (p.coluna == -1){
                    cert.valor = Original.c[Original.m];
                    cert.certificado = 1;
                    break;
                }else if (p.linha == -1){
                    cert.valor = std::numeric_limits<double>::infinity();
                    cert.certificado = 2;
                    break;
                }else{
                    Original.pivot(p);
                }
            }
        }
        if (cert.certificado == 1){
            cout << "Solução ótima encontrada: " << cert.valor << endl;
        }else if (cert.certificado == 0){
            cout << "Problema inviável." << endl;
        }else if (cert.certificado == 2){
            cout << "Problema ilimitado." << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    string nome_arquivo;
    
    if (argc > 1) {
        nome_arquivo = argv[1];
    } else {
        nome_arquivo = "teste.txt"; // Arquivo padrão
    }
    
    cout << "Testando arquivo: " << nome_arquivo << endl;
    tableu o = leitor(nome_arquivo);
    Simplex s(o);
    s.iteracao();
    return 0;
}