---
title: Manual de Acessibilidade para The Sims 2
lang: pt
toc-title: Sumário
---

Acessibilidade para The Sims 2 torna The Sims 2 Legacy Collection jogável com um leitor de tela e só com o teclado. Ele lê cada tela, caixa de diálogo e painel pelo seu leitor de tela (NVDA, JAWS, Narrador e outros), permite percorrê-los com as mesmas teclas em todo lugar e acrescenta as ferramentas de que um jogador cego precisa: uma lista dos objetos do lote, um mapa do lote, um narrador para o que o jogo só mostra, os lugares onde um móvel cabe e um cursor para construir.

No jogo, **Control+Shift+F1** abre este manual, no idioma do jogo. **Control+F1** diz as teclas da tela em que você está.

## Primeiros passos

1. Copie `version.dll` e a pasta `Sims2Access` para a pasta `EP9\TSBin` do jogo (a que tem `Sims2EP9.exe`), dentro de `The Sims 2 Legacy Collection` na sua biblioteca do Steam.
2. Inicie seu leitor de tela e depois inicie The Sims 2 Legacy Collection pelo Steam.
3. Quando a janela do jogo abrir, você ouve "Acessibilidade para The Sims 2 carregada" e onde fica a ajuda.

O mod fala no idioma do jogo. Para mudá-lo, abra o Steam, depois as Propriedades do jogo, Geral, Idioma. O mod fala inglês, francês, alemão, italiano, espanhol, holandês, dinamarquês, sueco, norueguês, finlandês, português, japonês, polonês, chinês simplificado e tradicional, tailandês, coreano, tcheco, húngaro e russo.

O mod funciona com a versão atual do jogo no Steam. Se o jogo for atualizado e o mod disser "versão do jogo não suportada", instale a próxima versão do mod.

## Como se mover: as mesmas teclas em todo lugar

Toda tela, caixa de diálogo, painel e modo funciona do mesmo jeito. O que você percorre é um anel de **grupos** (os botões de uma caixa de diálogo, as necessidades, a família, o catálogo...). Um grupo contém **itens**, e alguns itens abrem um **nível** mais profundo.

| Tecla | O que faz |
|---|---|
| Tab, Shift+Tab | Grupo seguinte ou anterior. Primeiro são ditos o nome do grupo e o número de itens. |
| Seta para cima, Seta para baixo (ou Seta para a esquerda, Seta para a direita) | Item anterior ou seguinte do grupo. |
| Home, End | Primeiro ou último item do grupo. |
| Enter | Aciona o item. Em um nível (um tipo ou um cômodo dos objetos do lote), abre-o. |
| Backspace | Volta um nível. |
| R | Diz o item de novo, com o grupo dele. |
| Alt+letra | O próximo grupo cujo nome começa com essa letra. |
| Control+F1 | As teclas da tela em que você está. |
| Control+Shift+F1 | Este manual. |

Os controles dizem o que são depois do nome, como faz um leitor de tela: "Aceitar, botão", "Opções, botão, selecionado", "de Tímido a Extrovertido, controle deslizante, 6 de 10".

- **Controles deslizantes** (a personalidade em Criar um Sim): Seta para a esquerda e Seta para a direita mudam o valor e dizem os pontos que restam.
- **Campos de texto** (nomes de famílias e de Sims, o console de trapaças): vá até o campo com as setas e digite: ele recebe o teclado assim que a seleção está nele. Cada letra é repetida, e Backspace diz o que apaga. Seta para a esquerda, Seta para a direita, Home e End se movem no campo e dizem o caractere que está ali; Delete diz o caractere que remove.
- **Itens somente leitura** que o jogo explica ao passar o mouse (uma habilidade, um interesse, uma área de carreira): Enter fala a explicação.
- **Controles desativados** dizem "indisponível" e não são acionados.

O último grupo da tela principal é **ajuda do mod**: seus botões dizem as teclas daquela tela (como Control+F1) e abrem este manual (como Control+Shift+F1).

## Ler mais: os buffers de revisão

Nem tudo cabe em uma frase. O mod guarda dois buffers que você pode ler linha por linha:

- **detalhes**: tudo sobre o que acabou de ser dito: a descrição de um produto, a dica de um controle, o quadrado e a direção de um lugar, o preço de um objeto, e a posição dele no grupo ("3 de 12").
- **eventos**: o que aconteceu: compras, vendas, textos de caixas de diálogo, tudo o que o narrador disse.

| Tecla | O que faz |
|---|---|
| Control+Seta para cima | A linha seguinte, mais fundo no buffer. |
| Control+Seta para baixo | De volta para o começo. |
| Control+Seta para a esquerda, Control+Seta para a direita | Alterna entre os buffers. |

## A vizinhança

O primeiro grupo da vizinhança é **lotes**: cada um diz seu nome, seu tipo, quem mora nele (os prédios de apartamentos com seus inquilinos) e seu tamanho ("40 por 30 quadrados"). Enter em um lote abre o painel do lote do jogo.

Para mudar uma família, escolha-a na caixa de famílias (o painel de famílias) e pressione Enter em um lote: o jogo pergunta se deve mudá-la para lá e quanto custa.

## Criar uma família e Sims

Criar uma Família, Criar um Sim e Criar um Animal são grupos como qualquer tela:

- Digite o sobrenome da família e os nomes de cada Sim nos campos de texto.
- Idade, sexo, pele, corpo e as grades de cabeças, cabelos e roupas ("3 de 6"; as raças dos animais pelo nome).
- **Personalidade**: Seta para a esquerda e Seta para a direita somam ou tiram um ponto de cada par de traços ("6 de 10, restam 4 pontos").
- Aspiração, signo e atrações são botões.
- **A árvore genealógica** (Relações familiares): Enter em um Sim o pega, Enter em outro Sim os une (o jogo então pergunta qual é o parentesco), Enter no mesmo Sim o solta.

## O lote: o modo Simulação

Em um lote a tela é um anel de grupos como qualquer outra. Tab percorre os grupos nesta ordem:

1. **necessidades** do Sim selecionado, cada uma com seu nível.
2. **fila de ações**: o que o Sim selecionado vai fazer; Enter cancela uma ação.
3. **objetos por tipo** e **objetos por cômodo**: os objetos do lote (veja abaixo).
4. Os painéis do jogo: o painel de controle, o painel do Sim (necessidades, relacionamentos, emprego, habilidades, personalidade, interesses...), a família, os desejos e medos, e qualquer painel que você abrir.
5. **ajuda do mod**.

O painel do Sim diz a aspiração do Sim ("Aspiração: 24 por cento"); o painel de recompensas (Recompensas, depois Recompensas de aspiração) diz os pontos de aspiração para gastar.

As opções do jogo (o botão Opções do painel de controle) são lidas da mesma forma: cada escolha diz sua configuração ("Sombras: Alta") e os controles deslizantes, como o volume da música, mudam com Seta para a esquerda e Seta para a direita ("Música, controle deslizante, 35 de 100"). As músicas de uma estação de rádio são um grupo próprio; Enter liga ou desliga uma música.

A página do emprego diz o horário do Sim; Enter sobre ele acrescenta quando chega a carona. Para deixar um emprego, ligue para o trabalho pelo telefone. Cada habilidade diz o seu nível e, quando a próxima promoção pede mais, quanto: «Culinária, 2 de 10, a próxima promoção pede 4».

### Selecionar Sims e a câmera

| Tecla | O que faz |
|---|---|
| Control+1 a Control+9 | Seleciona do primeiro ao nono Sim da família. |
| Control+E | O Sim selecionado em uma frase: onde está, o que está fazendo, necessidades urgentes. |
| Control+Shift+E | Como é um Sim: idade, cabelo, pele, olhos, óculos, corpo e roupa, com as palavras do jogo. O Sim em foco entre as coisas do lote, senão o selecionado. Os detalhes de um Sim (Control+Seta para cima nele) também o dizem. |
| Control+T | Diz cada nova hora do relógio («são 14:00»), ou deixa de dizer. Útil na velocidade máxima. |
| Control+Shift+F | A câmera segue o Sim selecionado ou fica livre. |

A câmera segue o Sim selecionado, porque os sons do jogo vêm do que está na tela. Quando o foco é mostrado na tela (Control+Shift+V), a câmera desliza até cada coisa que você percorre em vez de saltar.

O zoom e a direção da câmera são ditos quando mudam (Z e X aproximam e afastam), e o painel de controle também os mostra: "câmera: vista próxima, 5 de 6, voltada para nordeste". A direção é para onde olha a parte de cima da tela.

### Os objetos do lote

Tudo o que há no lote está em dois grupos: **objetos por tipo** (os Sims, depois as categorias do modo Compra: assentos, superfícies, eletrodomésticos..., depois outros) e **objetos por cômodo** (primeiro o cômodo do Sim selecionado, depois os outros, cada um com seu andar). Móveis de vários quadrados (camas, sofás, mesas) contam uma vez só. Enter em um tipo ou cômodo lista seus objetos, os mais próximos primeiro, como "Geladeira, cozinha, a 6 quadrados ao nordeste" a partir do Sim selecionado. Backspace volta aos tipos ou cômodos. Os nomes dos cômodos são deduzidos pelo mod a partir do que há neles.

Você não precisa ir até lá com Tab: as teclas Page percorrem os objetos de qualquer ponto do lote, nos modos Simulação, Compra e Construção.

| Tecla | O que faz |
|---|---|
| Page Down, Page Up | Objeto seguinte ou anterior do tipo ou cômodo escolhido por último. |
| Shift+Page Down, Shift+Page Up | Tipo de objeto seguinte ou anterior. |
| Control+Page Down, Control+Page Up | Cômodo seguinte ou anterior (o primeiro toque: o cômodo do Sim selecionado). |
| Enter | As interações do objeto (no modo Compra: pega o objeto para movê-lo). |
| Control+Enter | Leva o Sim selecionado até o objeto. |
| Control+Shift+Enter | Vai correndo. |
| Control+F | Procurar algo: digite parte do nome ("tel" encontra o telefone, acentos não importam). Cima e Baixo percorrem os resultados, Enter age sobre ele, Control+Enter vai até lá, Delete remove uma porta ou janela, Escape volta para onde você estava. |
| Delete | Em uma porta ou janela, no modo Comprar ou com a mão do modo Construir: pega e vende, com reembolso. |

Os Sims com um trabalho no lote dizem qual: "Ravi Pérez, carteiro", o garçom, o chef, o jardineiro, a empregada. A tigela de comida de um animal diz se tem comida. O táxi e outros carros junto à calçada estão entre os outros objetos enquanto esperam.

Depois dos Sims vem **problemas**: o que pede atenção, como coisas quebradas, coisas para limpar, uma tigela de animal vazia, poças, lixo, cinzas e fogo. Por último vem **portas, janelas e escadas**. Uma coisa a que nenhum Sim consegue chegar a partir da entrada do lote diz «nenhum Sim consegue chegar».

### Menus de interações

Enter em um objeto (ou em um Sim) abre o menu de interações do jogo. Seta para cima e Seta para baixo escolhem, Enter executa. As opções que oferecem mais escolhas abrem um menu mais profundo. Quando o menu fecha, a seleção volta ao objeto.

### Cômodos

| Tecla | O que faz |
|---|---|
| Control+L | A planta do cômodo (no cursor de construção ou no cursor do mapa, senão onde o Sim selecionado está): nome, andar, quadrados, limites, portas e janelas; cada porta e janela, com sua parede e o cômodo que fica atrás, está no buffer de detalhes. |

### O mapa do lote

**Control+M** liga o mapa: um cursor que percorre o lote quadrado por quadrado, começando no Sim selecionado. Cada passo toca um som para o piso e para cada parede, cerca, grade, porta ou janela em volta.

| Tecla | O que faz |
|---|---|
| Setas | Um quadrado; com Shift, cinco. Diz o que há ali e para onde está virado, o cômodo e o piso quando mudam, as paredes, cercas, grades, portas e janelas de cada lado, as escadas e os declives. Paredes, cercas e janelas param o cursor; as portas deixam-no passar. |
| Alt+Setas | O mesmo, através das paredes. |
| Vírgula, ponto | O que cada passo diz: tudo, só objetos, só pisos (pelo nome, «Ladrilho de granito claro»), só paredes, só divisões. |
| Home | Volta ao Sim selecionado. |
| Control+Shift+Page Up, Page Down | O andar de cima ou de baixo. |
| Enter | O menu de interações do que está ali. Uma coisa sobre uma mesa ou secretária vem antes da mesa: o computador, não a sua secretária. |
| Control+Enter | Leva o Sim selecionado até lá (Control+Shift+Enter: correndo). |
| Page Down, Page Up (e Shift, Control) | Os objetos do lote, como sempre; o mapa vai até cada um. |
| R | Tudo sobre o quadrado. Control+L, a planta do cômodo. |
| Esc ou Control+M | Sai do mapa. |

### O narrador

O jogo mostra muitas coisas só como imagens. O mod as diz quando acontecem e as guarda no buffer de eventos:

- **Os balões** sobre os Sims: "Carlos pensa: Energia", "Carlos diz: Política", um ícone riscado ("sem ..."), os gritos, e as manchetes de relacionamento ("o relacionamento melhora", "novo amigo", "primeiro beijo").
- **O que falta a uma refeição**: "Carlos pensa: é preciso uma bancada" (um forno, um fogão, uma churrasqueira, um micro-ondas).
- **Os trajetos**: "Carlos não consegue chegar lá".
- **As visitas à porta**: "Elvira está à porta esperando ser cumprimentada"; cumprimente-a (o menu de interações dela) para deixá-la entrar.
- **Os desejos e medos**: "Carlos realizou um desejo: Ir nadar, 500 pontos de aspiração", "Carlos: um medo se realizou: ...".
- **As ações** da família, **as necessidades** que caem, **as visitas**, e o dinheiro pago ou recebido. Quando uma ação é interrompida e espera na fila, isso é dito: «Carlos parou Sentar por Pânico; espera na fila».
- **Os móveis que saem do lote** (um ladrão, o oficial de justiça), com o seu valor.

Os pensamentos e gritos da tua família são ditos, e os gritos de qualquer um. As conversas (o que os Sims dizem e os títulos de relacionamento) são ditas conforme Control+Shift+N escolhe; o resto vai só para o buffer de eventos.

**Control+N** muda quanto é dito: tudo, só avisos ou nada. As linhas não faladas vão mesmo assim para o buffer de eventos.

**Control+Shift+N** escolhe de quem as conversas são ditas: qualquer um que fale com a tua família (no início), todos no lote, só a tua família, ou ninguém. O que não é dito vai na mesma para o buffer de eventos.

Ações com outro Sim nomeiam esse Sim, seja quem for que as começou: "Carlos: Cumprimentar, com Elvira".

## O modo Compra

F2 abre o modo Compra. Os produtos do catálogo são botões: cada produto diz seu nome, preço e avaliações, e o buffer de detalhes tem sua descrição. Passar do último produto de uma página (ou voltar antes do primeiro) muda de página.

### Posicionar um objeto

Enter em um produto o pega. O mod encontra todos os lugares do lote onde ele cabe, em todas as direções, os mais próximos do Sim selecionado primeiro.

| Tecla | O que faz |
|---|---|
| Page Down, Page Up (ou Tab) | Os lugares, os mais próximos primeiro; cada um diz seu cômodo, as direções em que cabe e a que distância está. |
| Control+Page Down, Control+Page Up | Os cômodos que têm lugares. |
| Shift+Page Down, Shift+Page Up (ou vírgula, ponto) | Giram o objeto; a direção é dita. |
| Control+Shift+Page Down, Page Up | Ao lado dos objetos do lote, os mais próximos primeiro, virado para eles quando cabe. |
| Setas | Movem o objeto um quadrado em vez disso (Shift: cinco): se cabe, no que esbarra, o cômodo quando muda, as paredes em volta, sua direção e para o que está virado (também mais longe: "para Sofá, a 3 quadrados"), o quadrado; depois os sons do quadrado. Paredes, cercas e janelas param-no; as portas deixam-no passar. |
| Alt+Setas | O mesmo, através das paredes. |
| Home | Volta ao Sim selecionado. |
| Enter | Compra ali; o jogo entrega outra cópia até você pressionar Esc. |
| Esc | Para de posicionar (um objeto movido volta ao lugar). |
| Delete | Vende o objeto que você está segurando. |
| R | Diz o lugar de novo, completo. |

Um objeto de vários quadrados diz quanto piso ocupa ("ocupa 2 quadrados: 2 de oeste a leste, 1 de norte a sul").

Os lugares de um assento começam onde algo se usa sentado, como um computador numa secretária, e o assento é virado para ele: «virado a oeste, para usar Computador sentado». Depois de colocar um objeto, o mod diz como vai ser usado: «para usar Computador sentado», «usa-se a partir de Cadeira de escritório», «os Sims conseguem chegar» ou «nenhum Sim consegue chegar».

### Mover e vender

No modo Compra, vá com Page Down até um objeto do lote e pressione Enter: ele é pego, e as mesmas teclas o colocam em outro lugar. Delete o vende. Os seus lugares começam pelos mais perto de onde estava.

## O modo Construção

F3 abre o modo Construção. Suas ferramentas e estilos são botões.

Pressionar uma ferramenta leva aos seus estilos, em um grupo com o nome da ferramenta; Tab volta às ferramentas.

### O cursor de construção

Enter em um estilo de parede, cerca, fundação, piso ou piscina inicia o cursor de construção no Sim selecionado. Paredes, cercas e fundações vão de canto a canto; pisos e piscinas cobrem quadrados.

| Tecla | O que faz |
|---|---|
| Setas | Um canto ou quadrado; com Shift, cinco. Diz o que há ali, o cômodo quando muda, o piso (pisos e piscinas) ou as paredes, cercas, portas e janelas que se encontram no canto, um declive, e depois onde está. |
| Page Up, Page Down | O nível de cima ou de baixo (0 o terreno, 1 o piso de uma fundação). |
| Espaço | Marca o início. |
| Enter | Constrói do início até o cursor; o custo é dito. |
| Control+Enter | Remove do início até o cursor. |
| Home | Volta ao Sim selecionado. |
| R | Tudo sobre o cursor. |
| Esc | Sai do cursor. |

Depois que uma parede é construída, um Sim da família preso em um cômodo sem porta é anunciado; o Control+Z do jogo desfaz a parede.

### Portas, janelas, escadas e telhados

Portas, janelas e escadas são posicionadas como os móveis, a partir dos lugares onde cabem: o lugar de uma porta diz em que parede ela está e para que cômodo ela leva. Telhados: os botões de telhado automático da página de telhados.

Depois de colocar uma porta, o mod diz se os Sims podem passar por ela ou o que atrapalha. Para mudá-la depois, encontre-a em **portas, janelas e escadas** ou com Control+F: Enter a pega para mover, no modo Comprar ou com a mão do modo Construir, e Delete a remove.

## Sinais sonoros

O cursor de construção, as setas ao posicionar e o mapa do lote tocam um som curto a cada passo: o piso (grama, madeira, cerâmica, carpete, pedra, a borda da piscina, água, sem piso nos andares de cima: do que o piso é feito, segundo sua definição no catálogo), a parede, cerca, grade, porta ou janela de cada lado (o oeste à esquerda, o leste à direita, o norte mais agudo, o sul mais grave), as escadas, um Sim ou um objeto no quadrado, um declive, uma mudança de cômodo, a borda do lote e, ao posicionar, se o objeto cabe. **Control+Shift+S** os desliga e liga.

## Outras teclas

| Tecla | O que faz |
|---|---|
| Control+Shift+C | O console de trapaças do jogo: sua entrada é um campo de texto; Enter nela lhe dá o teclado, digite a trapaça, Enter a executa, e o que o console escreve é falado. |
| Control+Shift+V | Destaca na tela o controle selecionado, para alguém que enxerga e está ajudando. No modo Viver, com a janela do jogo à frente, a câmara também olha para a coisa do lote em foco e o ponteiro do rato vai para cima dela. |

## As teclas do próprio jogo

Elas são do jogo e continuam funcionando:

| Tecla | O que faz |
|---|---|
| F1, F2, F3, F4, F5 | Modo Simulação, Compra, Construção, Câmera e Opções. |
| P | Pausa. |
| 1, 2, 3 | Velocidade normal, rápida, mais rápida. |
| Espaço, N | Seleciona o próximo Sim. |
| Z, X | Aproxima e afasta o zoom. |
| Control+Z | Desfaz nos modos Compra e Construção. |

## Configurações

As configurações do mod ficam em `Sims2Access\Sims2Access.ini` na pasta do jogo, seção `[mod]`. As teclas que as mudam as salvam na hora.

| Configuração | Valores |
|---|---|
| `narrator` | 2 tudo, 1 só avisos, 0 desligado (Control+N). |
| `cues` | 1 sinais sonoros ligados, 0 desligados (Control+Shift+S). |
| `cue_volume` | Volume dos sinais sonoros, de 0 a 100 (padrão 80). |
| `show_focus` | 1 destaca na tela o controle selecionado (Control+Shift+V). |
| `social` | De quem as conversas são ditas: 3 todos, 2 qualquer um com a tua família, 1 a tua família, 0 ninguém (Control+Shift+N). |
| `hours` | 1 diz cada nova hora (Control+T). |

## Quando algo dá errado

- **Nenhuma fala**: inicie o leitor de tela antes do jogo. O log do mod é `Sims2Access\Sims2Access.log`; envie-o com o seu relato.
- **"Versão do jogo não suportada"**: o jogo foi atualizado; instale a próxima versão do mod.
- **Uma tecla não faz nada**: Control+F1 diz quais teclas funcionam onde você está.

## Ainda não feito

Papel de parede, telhados desenhados e terreno no modo Construção; os nomes das recompensas de aspiração; colocar no lote um objeto do inventário de um Sim; nomes para as cabeças, cabelos e roupas de Criar um Sim; as telas próprias das expansões (universidade, vida noturna, negócios, estações, férias, apartamentos, magia) por enquanto só são lidas pela navegação genérica.
