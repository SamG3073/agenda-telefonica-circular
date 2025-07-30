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
Para clonar esse repositório e compilar o código-fonte, os seguintes pacotes devem estar instalados:

+ git
+ gcc
+ make
+ libsdl2-dev
+ libsdl2-image-dev
+ libsdl2-ttf-dev

## Debian/Ubuntu
### Preparação
Em um sistema ou ambiente Debian ou Ubuntu, digite os seguintes comandos no terminal, um por um:

```
$ sudo apt update
$ sudo apt install -y git gcc make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
$ git clone https://github.com/SamG3073/agenda-telefonica-circular.git
$ cd agenda-telefonica-circular
```

### Compilação e execução

Na mesma janela de terminal de antes, digite o seguinte:

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

## Windows
### Preparação
Faça o download e instalação do [MSYS2](https://www.msys2.org/). Em seguida, abra o terminal "MSYS2 MINGW64" e digite esses comandos, um por um:

```
$ pacman -Syu
$ pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf git
$ git clone https://github.com/SamG3073/agenda-telefonica-circular.git
$ cd agenda-telefonica-circular
```

Observações:

+ Para qualquer pergunta que aparecer na tela, basta pressionar Enter para aceitar os valores padrão.
+ Por algum motivo, o terminal do MSYS2 adiciona um caractere oculto que pode impossibilitar a execução do comando ao colar usando `Ctrl + Shift + V`. Por isso, certifique-se de usar o menu de contexto `(Botão direito do mouse > Paste)` ao colar os comandos.

### Compilação e execução
No mesmo terminal do MSYS2, basta digitar:

```
$ mingw32-make
```

Isso compilará o binário do programa, que aparecerá na pasta raiz do repositório.

Em seguida, para executar o programa, digite:

```
$ ./agenda_contatos.exe
```