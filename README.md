# CHIP8

Este é um emulador simples de CHIP-8 escrito em C utilizando a biblioteca SDL2 para gráficos e áudio.

## Requisitos

- SDL2
- gcc
- make


## Compilação

Para compilação do projeto, utilize o comando:

```bash
make
```

## Execução
Para executar o emulador, utilize o comando:
```bash
./chip8 <caminho_para_o_arquivo_chip8>
```

Substitua `<caminho_para_o_arquivo_chip8>` pelo caminho do arquivo CHIP-8 que deseja executar.

### Flags opcionais
- `--clock <velocidade>`: Define a velocidade do clock da CPU em Hz. Padrão: 500Hz.
- `--scale <fator>`:  Define o fator de escala da janela. Um fator de 10 resulta em uma janela de 640x320. Padrão: 10.
- `--help`: Exibe esta mensagem de ajuda.
