# TP2 - Método Simplex

Implementação em C++ do método Simplex para resolver problemas de programação linear.

O programa lê um problema a partir de um arquivo de texto, constrói um tableau auxiliar com variáveis de folga, executa as iterações do Simplex e informa se o problema possui solução ótima, é inviável ou ilimitado.

## Funcionalidades

- Leitura de problemas de programação linear a partir de arquivos `.txt`;
- Construção automática do tableau auxiliar;
- Inclusão de variáveis de folga;
- Busca de pivô pelo critério do menor coeficiente negativo;
- Aplicação do teste da razão mínima;
- Impressão dos tableaux durante a execução;
- Identificação dos casos:
  - solução ótima;
  - problema inviável;
  - problema ilimitado.

## Estrutura do repositório

| Arquivo | Descrição |
| --- | --- |
| `tp2_2023028552.cpp` | Código-fonte principal da implementação do Simplex. |
| `teste.txt` | Exemplo pequeno usado como entrada padrão. |
| `example01.txt` a `example12.txt` | Arquivos de entrada com diferentes problemas de programação linear. |
| `sol_example01.txt` a `sol_example12.txt` | Arquivos com soluções esperadas para os exemplos correspondentes. |
| `tp2_2023028552` | Versão compilada do programa, presente no repositório. |

## Requisitos

- Compilador compatível com C++11 ou superior;
- Sistema operacional capaz de executar programas compilados em C++.

O programa utiliza somente bibliotecas padrão da linguagem:

- `iostream`;
- `fstream`;
- `string`;
- `limits`.

## Compilação

Na raiz do repositório, execute:

```bash
g++ -std=c++11 -Wall -Wextra -O2 tp2_2023028552.cpp -o tp2_2023028552
```

## Execução

Para executar usando o arquivo padrão `teste.txt`:

```bash
./tp2_2023028552
```

Para informar outro arquivo de entrada:

```bash
./tp2_2023028552 example01.txt
```

No Windows, usando MinGW, o comando equivalente pode ser:

```powershell
g++ -std=c++11 -Wall -Wextra -O2 tp2_2023028552.cpp -o tp2_2023028552.exe
.\\tp2_2023028552.exe example01.txt
```

## Formato do arquivo de entrada

O arquivo deve seguir o formato abaixo:

```text
n m
c1 c2 ... cm
 a11 a12 ... a1m b1
 a21 a22 ... a2m b2
...
 an1 an2 ... anm bn
```

Onde:

- `n` é o número de restrições;
- `m` é o número de variáveis de decisão;
- a segunda linha contém os `m` coeficientes da função objetivo;
- cada uma das `n` linhas seguintes contém `m` coeficientes da restrição e o termo independente `b`.

### Exemplo

O arquivo `teste.txt` contém:

```text
3 5
3 3 3 3 3
1 2 1 0 0 20
5 4 0 0 1 50
0 0 0 1 0 30
```

Esse arquivo representa um problema com 3 restrições e 5 variáveis.

## Saída

Durante a execução, o programa imprime o tableau original, o tableau auxiliar, o tableau canonizado e os tableaux de cada iteração. Ao final, uma das mensagens abaixo é exibida:

```text
Solução ótima encontrada: <valor>
```

```text
Problema inviável.
```

```text
Problema ilimitado.
```

## Funcionamento resumido

1. O arquivo de entrada é lido pela função `leitor`.
2. As restrições com termo independente negativo são multiplicadas por `-1`.
3. É criado um tableau auxiliar com variáveis de folga.
4. O tableau auxiliar é canonizado.
5. O algoritmo escolhe uma coluna pivô com coeficiente negativo na função objetivo.
6. A linha pivô é selecionada pelo teste da razão mínima.
7. O tableau é atualizado até que não existam mais coeficientes negativos ou não seja possível encontrar uma linha pivô.
8. O tableau original é processado para obter a solução final.

## Observações

- O programa considera problemas em formato compatível com o tableau utilizado na implementação.
- Os arquivos `sol_exampleXX.txt` servem como referência para comparar os resultados dos arquivos `exampleXX.txt`.
- A saída contém informações de depuração dos tableaux, o que é útil para acompanhar as iterações do algoritmo.
- A implementação usa alocação dinâmica manual com `new`; portanto, recomenda-se revisar o gerenciamento de memória e adicionar a liberação dos recursos em uma evolução futura do projeto.
- Para maior robustez numérica, uma versão futura pode incluir tolerância para comparações envolvendo valores `double` próximos de zero.

## Autor

Repositório desenvolvido por [HaniBR01](https://github.com/HaniBR01).
