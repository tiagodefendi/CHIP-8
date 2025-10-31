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
Para carregar uma ROM na memória e executar o emulador, utilize o comando:
```bash
./chip8_emulator <caminho_para_o_arquivo_chip8>
```

Substitua `<caminho_para_o_arquivo_chip8>` pelo caminho do arquivo CHIP-8 que deseja executar.

### Flags opcionais
- `--clock <velocidade>`: Define a velocidade do clock da CPU em Hz. Padrão: 500Hz.
- `--scale <fator>`:  Define o fator de escala da janela. Um fator de 10 resulta em uma janela de 640x320. Padrão: 10.
- `--help`: Exibe esta mensagem de ajuda.

## Instruções Implementadas

- `00E0` - CLS: limpa a tela.
- `00EE` - RET: retorna de sub-rotina.
- `0nnn` - SYS addr: chamada de máquina (ignorada/emulada como NOP).
- `1nnn` - JP addr: pula para o endereço.
- `2nnn` - CALL addr: chama sub-rotina.
- `3xkk` - SE Vx, byte: pula se Vx == byte.
- `4xkk` - SNE Vx, byte: pula se Vx != byte.
- `5xy0` - SE Vx, Vy: pula se Vx == Vy.
- `6xkk` - LD Vx, byte: carrega byte em Vx.
- `7xkk` - ADD Vx, byte: soma byte a Vx (sem carry).
- `8xy0` - LD Vx, Vy: copia Vy para Vx.
- `8xy1` - OR Vx, Vy: Vx = Vx OR Vy.
- `8xy2` - AND Vx, Vy: Vx = Vx AND Vy.
- `8xy3` - XOR Vx, Vy: Vx = Vx XOR Vy.
- `8xy4` - ADD Vx, Vy: soma Vy em Vx, VF = carry.
- `8xy5` - SUB Vx, Vy: Vx = Vx - Vy, VF = NOT borrow.
- `8xy6` - SHR Vx {, Vy}: desloca Vx para a direita, VF = bit menos significativo.
- `8xy7` - SUBN Vx, Vy: Vx = Vy - Vx, VF = NOT borrow.
- `8xyE` - SHL Vx {, Vy}: desloca Vx para a esquerda, VF = bit mais significativo.
- `9xy0` - SNE Vx, Vy: pula se Vx != Vy.
- `Annn` - LD I, addr: carrega endereço em I.
- `Bnnn` - JP V0, addr: pula para addr + V0.
- `Cxkk` - RND Vx, byte: Vx = (rand & byte).
- `Dxyn` - DRW Vx, Vy, nibble: desenha sprite na tela, VF = colisão.
- `Ex9E` - SKP Vx: pula se tecla Vx pressionada.
- `ExA1` - SKNP Vx: pula se tecla Vx não pressionada.
- `Fx07` - LD Vx, DT: Vx = valor do delay timer.
- `Fx0A` - LD Vx, K: espera por tecla e armazena em Vx.
- `Fx15` - LD DT, Vx: define delay timer = Vx.
- `Fx18` - LD ST, Vx: define sound timer = Vx.
- `Fx1E` - ADD I, Vx: I = I + Vx.
- `Fx29` - LD F, Vx: I aponta para sprite de dígito em Vx.
- `Fx33` - LD B, Vx: armazena BCD de Vx em I, I+1, I+2.
- `Fx55` - LD [I], Vx: armazena V0..Vx em memória em I.
- `Fx65` - LD Vx, [I]: carrega V0..Vx da memória em I.

Estas instruções foram implementadas conforme a especificação padrão do CHIP-8.
