# Agenda telefônica circular
Agenda de contatos com lista circular, permite navegação contínua entre os contatos.

Modelo base de inspiração:

<img width="1024" height="500" alt="image" src="https://github.com/user-attachments/assets/0a3c99ef-1a48-4b88-847e-27b2393cb368" />


Prévia:

<img width="362" height="640" alt="image" src="https://github.com/user-attachments/assets/db58cd9c-dd77-4083-bfdb-505dd27eb442" />
<img width="359" height="643" alt="image" src="https://github.com/user-attachments/assets/3956ac7a-3d02-4f6c-bc52-99f5ed7cf0e3" />

Modo Tela Cheia:

<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/779b9da8-7f3b-4417-bd71-5dfb0f0048a3" />
<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/152555c6-952e-452e-b850-86cb714577c3" />

# Compilação
Para compilar o arquivo binário do programa em um ambiente Linux, os seguintes pacotes devem estar instalados:

+ gcc
+ make
+ libsdl2-dev
+ libsdl2-image-dev
+ libsdl2-ttf-dev

## Debian/Ubuntu
### Instalação dos pacotes
Em um sistema ou ambiente Debian ou Ubuntu, digite os seguintes comandos no terminal, um por um:

```
$ sudo apt update
$ sudo apt install -y gcc make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

### Compilando o executável
Faça o download desse repositório e extraia na pasta de sua preferência. Caso você tenha o Git instalado e queira usá-lo para clonar o repositório diretamente, digite o seguinte no terminal:

```
$ git clone https://github.com/SamG3073/agenda-telefonica-circular.git
```

Em seguida, abra uma janela do terminal na raiz do repositório, na mesma pasta onde estão este Readme e o Makefile, e execute:

```
$ make
```

Isso compilará o binário do programa, que aparecerá na pasta raiz do repositório.

Para abri-lo, primeiro certifique-se de que ele possui permissão de execução com:
```
$ chmod +x agenda_contatos
```

Agora você pode executar o programa dando dois cliques nele em um explorador de arquivos ou pelo terminal com:

```
$ ./agenda_contatos
```