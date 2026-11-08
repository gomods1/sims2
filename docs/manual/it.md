---
title: Manuale di Accessibilità per The Sims 2
lang: it
toc-title: Indice
---

Accessibilità per The Sims 2 rende The Sims 2 Legacy Collection giocabile con uno screen reader e la sola tastiera. Legge ogni schermata, finestra di dialogo e pannello con il tuo screen reader (NVDA, JAWS, Assistente vocale e altri), ti permette di scorrerli con gli stessi tasti ovunque e aggiunge gli strumenti di cui ha bisogno un giocatore cieco: un elenco degli oggetti del lotto, una mappa del lotto, un narratore per ciò che il gioco si limita a mostrare, i posti dove entra un mobile e un cursore per costruire.

Nel gioco, **Control+Maiusc+F1** apre questo manuale, nella lingua del gioco. **Control+F1** dice i tasti della schermata in cui ti trovi.

## Per iniziare

1. Copia `version.dll` e la cartella `Sims2Access` nella cartella `EP9\TSBin` del gioco (quella con `Sims2EP9.exe`), dentro `The Sims 2 Legacy Collection` nella tua libreria di Steam.
2. Avvia lo screen reader, poi avvia The Sims 2 Legacy Collection da Steam.
3. Quando si apre la finestra del gioco senti "Accessibilità per The Sims 2 caricata", e dove trovare l'aiuto.

La mod parla nella lingua del gioco. Per cambiarla, apri Steam, poi le Proprietà del gioco, Generali, Lingua. La mod parla inglese, francese, tedesco, italiano, spagnolo, olandese, danese, svedese, norvegese, finlandese, portoghese, giapponese, polacco, cinese semplificato e tradizionale, thailandese, coreano, ceco, ungherese e russo.

La mod funziona con la versione attuale del gioco su Steam. Se il gioco si aggiorna e la mod dice "versione del gioco non supportata", installa la versione successiva della mod.

## Muoversi: gli stessi tasti ovunque

Ogni schermata, finestra di dialogo, pannello e modalità funziona allo stesso modo. Ciò che scorri è un anello di **gruppi** (i pulsanti di una finestra di dialogo, i bisogni, la famiglia, il catalogo...). Un gruppo contiene **elementi**, e alcuni elementi aprono un **livello** più profondo.

| Tasto | Cosa fa |
|---|---|
| Tab, Maiusc+Tab | Gruppo successivo o precedente. Prima vengono detti il nome del gruppo e il numero dei suoi elementi. |
| Freccia su, freccia giù (o freccia sinistra, freccia destra) | Elemento precedente o successivo del gruppo. |
| Inizio, Fine | Primo o ultimo elemento del gruppo. |
| Invio | Attiva l'elemento. Su un livello (un tipo o una stanza degli oggetti del lotto) lo apre. |
| Backspace | Torna indietro di un livello. |
| R | Ripete l'elemento, con il suo gruppo. |
| Alt+lettera | Il gruppo successivo il cui nome inizia con quella lettera. |
| Control+F1 | I tasti della schermata in cui ti trovi. |
| Control+Maiusc+F1 | Questo manuale. |

I controlli dicono che cosa sono dopo il loro nome, come fa uno screen reader: "Accetta, pulsante", "Opzioni, pulsante, selezionato", "da Timido a Estroverso, cursore, 6 di 10".

- **Cursori** (la personalità in Crea un Sim): freccia sinistra e freccia destra cambiano il valore e dicono i punti rimasti.
- **Campi di testo** (nomi delle famiglie e dei Sim, la console dei trucchi): vai sul campo con le frecce e scrivi: riceve la tastiera appena la selezione è su di esso. Ogni lettera viene ripetuta, e Backspace dice cosa cancella. Freccia sinistra, freccia destra, Inizio e Fine si spostano nel campo e dicono il carattere che c'è; Canc dice il carattere che toglie.
- **Elementi di sola lettura** che il gioco spiega al passaggio del mouse (un'abilità, un interesse, un settore lavorativo): Invio legge la spiegazione.
- **I controlli disattivati** dicono "non disponibile" e non si attivano.

L'ultimo gruppo della schermata principale è **aiuto della mod**: i suoi pulsanti dicono i tasti di quella schermata (come Control+F1) e aprono questo manuale (come Control+Maiusc+F1).

## Leggere di più: i buffer di revisione

Non tutto sta in una frase. La mod tiene due buffer che puoi leggere riga per riga:

- **dettagli**: tutto su ciò che è appena stato detto: la descrizione di un prodotto, il suggerimento di un controllo, la casella e l'orientamento di un posto, il prezzo di un oggetto, e la sua posizione nel gruppo ("3 di 12").
- **eventi**: ciò che è successo: acquisti, vendite, testi delle finestre di dialogo, tutto ciò che ha detto il narratore.

| Tasto | Cosa fa |
|---|---|
| Control+freccia su | La riga successiva, più avanti nel buffer. |
| Control+freccia giù | Torna verso l'inizio. |
| Control+freccia sinistra, Control+freccia destra | Passa da un buffer all'altro. |

## Il quartiere

Il primo gruppo del quartiere è **lotti**: ognuno dice il suo nome, il suo tipo, chi ci abita (i condomini con i loro inquilini) e la sua grandezza («40 per 30 caselle»). Invio su un lotto apre il pannello del lotto del gioco.

Per trasferire una famiglia, sceglila nel contenitore delle famiglie (il pannello delle famiglie), poi premi Invio su un lotto: il gioco chiede se trasferirla e quanto costa.

## Creare una famiglia e i Sim

Crea una famiglia, Crea un Sim e Crea un animale sono gruppi come qualsiasi schermata:

- Scrivi il cognome della famiglia e i nomi di ogni Sim nei loro campi di testo.
- Età, sesso, carnagione, corporatura e le griglie di teste, acconciature e vestiti ("3 di 6"; le razze degli animali per nome).
- **Personalità**: freccia sinistra e freccia destra aggiungono o tolgono un punto a ogni coppia di tratti ("6 di 10, 4 punti rimasti").
- L'aspirazione, il segno zodiacale e le attrazioni sono pulsanti.
- **L'albero genealogico** (Relazioni familiari): Invio su un Sim lo prende, Invio su un altro Sim li unisce (il gioco chiede poi che parentela hanno), Invio sullo stesso Sim lo posa.

## Il lotto: la modalità Vivi

In un lotto la schermata è un anello di gruppi come qualsiasi altra. Tab li scorre in questo ordine:

1. **bisogni** del Sim selezionato, ognuno con il suo livello.
2. **coda delle azioni**: ciò che farà il Sim selezionato; Invio annulla un'azione.
3. **oggetti per tipo** e **oggetti per stanza**: gli oggetti del lotto (vedi sotto).
4. I pannelli del gioco: il pannello di controllo, il pannello del Sim (bisogni, relazioni, lavoro, abilità, personalità, interessi...), la famiglia, i desideri e le paure, e qualsiasi pannello tu apra.
5. **aiuto della mod**.

Il pannello del Sim dice l'aspirazione del Sim («Aspirazione: 24 per cento»); il pannello dei premi (Premi e Inventario, poi Premi per le aspirazioni) dice i punti di aspirazione da spendere.

Le opzioni del gioco (il pulsante Opzioni del pannello di controllo) si leggono allo stesso modo: ogni scelta dice la sua impostazione («Ombre: Alto») e i cursori, come il volume della musica, cambiano con freccia sinistra e freccia destra («Musica, cursore, 35 su 100»). I brani di una stazione radio sono un gruppo a parte; Invio attiva o disattiva un brano.

La pagina del lavoro dice gli orari del Sim; Invio su di essi aggiunge quando arriva l'auto di servizio. Per lasciare un lavoro, chiama il lavoro al telefono. Ogni abilità dice il suo livello e, quando la prossima promozione chiede di più, quanto: «Cucina, 2 su 10, la prossima promozione chiede 4».

### Selezionare i Sim e la telecamera

| Tasto | Cosa fa |
|---|---|
| Da Control+1 a Control+9 | Seleziona dal primo al nono Sim della famiglia. |
| Control+E | Il Sim selezionato in una frase: dove si trova, cosa sta facendo, bisogni urgenti. |
| Control+Maiusc+E | Com'è fatto un Sim: età, capelli, pelle, occhi, occhiali, corpo e abito, con le parole del gioco. Il Sim selezionato tra le cose del lotto, altrimenti quello attivo. Anche i dettagli di un Sim (Control+Freccia su su di lui) lo dicono. |
| Control+T | Annuncia ogni nuova ora dell'orologio («sono le 14:00»), o smette. Utile alla velocità massima. |
| Control+Maiusc+F | La telecamera segue il Sim selezionato, o resta libera. |

La telecamera segue il Sim selezionato, perché i suoni del gioco vengono da ciò che è sullo schermo. Quando il focus è mostrato sullo schermo (Control+Maiusc+V), la telecamera scivola verso ogni cosa che scorri invece di saltare.

Lo zoom e la direzione della telecamera vengono detti quando cambiano (Z e X zoomano), e li ha anche il pannello di controllo: "telecamera: vista vicina, 5 di 6, rivolta a nord-est". La direzione è quella verso cui guarda il lato alto dello schermo.

### Gli oggetti del lotto

Tutto ciò che c'è nel lotto sta in due gruppi: **oggetti per tipo** (i Sim, poi le categorie della modalità Compra: sedute, superfici, elettrodomestici..., poi altro) e **oggetti per stanza** (prima la stanza del Sim selezionato, poi le altre, ognuna con il suo livello). I mobili di più caselle (letti, divani, tavoli) contano una volta sola. Invio su un tipo o una stanza elenca i suoi oggetti, prima i più vicini, come "Frigorifero, cucina, a 6 caselle a nord-est" dal Sim selezionato. Backspace torna ai tipi o alle stanze. I nomi delle stanze sono dedotti dalla mod in base a ciò che contengono.

Non serve arrivarci con Tab: i tasti Pagina scorrono gli oggetti da qualsiasi punto del lotto, in modalità Vivi, Compra e Costruisci.

| Tasto | Cosa fa |
|---|---|
| Pagina giù, Pagina su | Oggetto successivo o precedente del tipo o della stanza scelti per ultimi. |
| Maiusc+Pagina giù, Maiusc+Pagina su | Tipo di oggetti successivo o precedente. |
| Control+Pagina giù, Control+Pagina su | Stanza successiva o precedente (alla prima pressione: la stanza del Sim selezionato). |
| Invio | Le interazioni dell'oggetto (in modalità Compra: lo prende per spostarlo). |
| Control+Invio | Manda il Sim selezionato fino all'oggetto. |
| Control+Maiusc+Invio | Ci va di corsa. |
| Control+F | Trova una cosa: digita parte del nome ("tel" trova il telefono, gli accenti non contano). Su e Giù scorrono i risultati, Invio agisce su di essa, Control+Invio ci va, Canc rimuove una porta o una finestra, Esc torna dov'eri. |
| Canc | Su una porta o una finestra, in modalità Acquisto o con la mano della modalità Costruzione: la prende e la vende, con il rimborso. |

I Sim con un lavoro sul lotto lo dicono: «Ravi Pérez, postino», il cameriere, il cuoco, il giardiniere, la domestica. La ciotola di un animale dice se c'è cibo. Il taxi e le altre auto sul marciapiede sono tra gli altri oggetti mentre aspettano.

Dopo i Sim viene **problemi**: ciò che richiede attenzione, come cose rotte, da pulire, una ciotola per animali vuota, pozzanghere, rifiuti, cenere e fuoco. Per ultimo viene **porte, finestre e scale**. Una cosa che nessun Sim può raggiungere dall'ingresso del lotto dice «nessun Sim può raggiungerlo».

### Menu delle interazioni

Invio su un oggetto (o su un Sim) apre il menu delle interazioni del gioco. Freccia su e freccia giù scelgono, Invio esegue. Le voci che offrono altre scelte aprono un menu più profondo. Quando il menu si chiude, la selezione torna sull'oggetto.

### Le stanze

| Tasto | Cosa fa |
|---|---|
| Control+L | La pianta della stanza (al cursore di costruzione o a quello della mappa, altrimenti dove si trova il Sim selezionato): nome, livello, caselle, confini, porte e finestre; ogni porta e finestra, con il suo muro e la stanza che c'è dietro, è nel buffer dei dettagli. |

### La mappa del lotto

**Control+M** attiva la mappa: un cursore che percorre il lotto casella per casella, partendo dal Sim selezionato. Ogni passo emette un suono per il pavimento e per ogni muro, recinto, ringhiera, porta o finestra intorno.

| Tasto | Cosa fa |
|---|---|
| Frecce | Una casella; con Maiusc, cinque. Dice cosa c'è e verso dove è rivolto, la stanza e il pavimento quando cambiano, i muri, i recinti, le ringhiere, le porte e le finestre su ogni lato, le scale e le pendenze. Muri, recinzioni e finestre fermano il cursore; le porte lo lasciano passare. |
| Alt+Frecce | Lo stesso, attraverso i muri. |
| Virgola, punto | Cosa dice ogni passo: tutto, solo oggetti, solo pavimenti (per nome, «Piastrella di granito chiaro»), solo muri, solo stanze. |
| Inizio | Torna al Sim selezionato. |
| Control+Maiusc+Pagina su, Pagina giù | Il livello di sopra o di sotto. |
| Invio | Il menu delle interazioni di ciò che c'è. Una cosa su un tavolo o una scrivania viene prima del tavolo: il computer, non la sua scrivania. |
| Control+Invio | Manda lì il Sim selezionato (Control+Maiusc+Invio: di corsa). |
| Pagina giù, Pagina su (e Maiusc, Control) | Gli oggetti del lotto come sempre; la mappa va su ognuno. |
| R | Tutto sulla casella. Control+L la pianta della stanza. |
| Esc o Control+M | Chiude la mappa. |

### Il narratore

Il gioco mostra molte cose solo con immagini. La mod le dice quando accadono e le tiene nel buffer degli eventi:

- **I fumetti** sopra i Sim: "Carlos pensa: Energia", "Carlos dice: Politica", un'icona barrata ("niente ..."), le urla, e i titoli delle relazioni ("la relazione migliora", "nuovo amico", "primo bacio").
- **Cosa manca per un pasto**: "Carlos pensa: serve un bancone" (un forno, un fornello, un barbecue, un microonde).
- **I percorsi**: "Carlos non riesce ad arrivarci".
- **I visitatori alla porta**: "Elvira è alla porta e aspetta di essere salutata"; salutala (il suo menu delle interazioni) per farla entrare.
- **Desideri e timori**: "Carlos ha esaudito un desiderio: Andare a nuotare, 500 punti di aspirazione", "Carlos: un timore si è avverato: ...".
- **Le azioni** della famiglia, **i bisogni** che calano, **i visitatori**, e il denaro pagato o ricevuto. Quando un'azione viene interrotta e aspetta in coda, viene detto: «Carlos ha interrotto Sedersi per Panico; aspetta in coda».
- **I mobili che lasciano il lotto** (un ladro, l'ufficiale giudiziario), con il loro valore.

Vengono detti i pensieri e le urla della tua famiglia, e le urla di chiunque. Le conversazioni (cosa dicono i Sim e i titoli sulle relazioni) vengono dette come sceglie Control+Maiusc+N; il resto va solo nel buffer degli eventi.

**Control+N** cambia quanto viene detto: tutto, solo gli avvisi o niente. Le righe non dette finiscono comunque nel buffer degli eventi.

**Control+Maiusc+N** sceglie quali conversazioni vengono dette: chiunque parli con la tua famiglia (all'inizio), tutti sul lotto, solo la tua famiglia o nessuno. Ciò che non viene detto va comunque nel buffer degli eventi.

Le azioni con un altro Sim nominano quel Sim, chiunque le abbia iniziate: "Carlos: Saluta, con Elvira".

## La modalità Compra

F2 apre la modalità Compra. I prodotti del catalogo sono pulsanti: ogni prodotto dice il suo nome, il prezzo e le valutazioni, e il buffer dei dettagli contiene la sua descrizione. Andando oltre l'ultimo prodotto di una pagina (o prima del primo) si cambia pagina.

### Posizionare un oggetto

Invio su un prodotto lo prende. La mod trova tutti i posti del lotto dove entra, in ogni direzione, prima i più vicini al Sim selezionato.

| Tasto | Cosa fa |
|---|---|
| Pagina giù, Pagina su (o Tab) | I posti, prima i più vicini; ognuno dice la sua stanza, le direzioni in cui l'oggetto entra e quanto è lontano. |
| Control+Pagina giù, Control+Pagina su | Le stanze che hanno posti. |
| Maiusc+Pagina giù, Maiusc+Pagina su (o virgola, punto) | Lo girano; viene detto l'orientamento. |
| Control+Maiusc+Pagina giù, Pagina su | Accanto agli oggetti del lotto, prima i più vicini, rivolto verso di loro quando entra. |
| Frecce | Lo spostano invece di una casella (Maiusc: cinque): se entra, contro cosa urta, la stanza quando cambia, i muri intorno, il suo orientamento e verso cosa è rivolto (anche più lontano: "verso Divano, a 3 caselle"), la casella; poi i suoni della casella. Muri, recinzioni e finestre lo fermano; le porte lo lasciano passare. |
| Alt+Frecce | Lo stesso, attraverso i muri. |
| Inizio | Torna al Sim selezionato. |
| Invio | Lo compra lì; il gioco te ne dà un'altra copia fino a Esc. |
| Esc | Smette di posizionare (un oggetto spostato torna al suo posto). |
| Canc | Vende l'oggetto che tieni. |
| R | Ripete il posto, per intero. |

Un oggetto di più caselle dice quanto pavimento occupa ("occupa 2 caselle: 2 da ovest a est, 1 da nord a sud").

I posti di una seduta iniziano dove qualcosa si usa da seduti, come un computer su una scrivania, e la seduta viene girata verso di esso: «rivolto a ovest, per usare Computer da seduti». Dopo aver posizionato un oggetto il mod dice come verrà usato: «per usare Computer da seduti», «si usa da Sedia da ufficio», «i Sim possono raggiungerlo» o «nessun Sim può raggiungerlo».

### Spostare e vendere

In modalità Compra, vai con Pagina giù su un oggetto del lotto e premi Invio: viene preso, e gli stessi tasti lo posizionano altrove. Canc lo vende. I suoi posti iniziano dai più vicini a dove si trovava.

## La modalità Costruisci

F3 apre la modalità Costruisci. I suoi strumenti e stili sono pulsanti.

Premere uno strumento porta ai suoi stili, in un gruppo con il nome dello strumento; Tab torna agli strumenti.

### Il cursore di costruzione

Invio su uno stile di muro, recinto, fondamenta, pavimento o piscina avvia il cursore di costruzione sul Sim selezionato. Muri, recinti e fondamenta vanno da angolo ad angolo; pavimenti e piscine coprono caselle.

| Tasto | Cosa fa |
|---|---|
| Frecce | Un angolo o una casella; con Maiusc, cinque. Dice cosa c'è, la stanza quando cambia, il pavimento (pavimenti e piscine) o i muri, i recinti, le porte e le finestre che si incontrano nell'angolo, una pendenza, poi dove si trova. |
| Pagina su, Pagina giù | Il livello di sopra o di sotto (0 il terreno, 1 il pavimento di una fondamenta). |
| Spazio | Segna l'inizio. |
| Invio | Costruisce dall'inizio al cursore; viene detto il costo. |
| Control+Invio | Rimuove dall'inizio al cursore. |
| Inizio | Torna al Sim selezionato. |
| R | Tutto sul cursore. |
| Esc | Esce dal cursore. |

Dopo la costruzione di un muro, viene annunciato un Sim della famiglia chiuso in una stanza senza porta; il Control+Z del gioco annulla il muro.

### Porte, finestre, scale e tetti

Porte, finestre e scale si posizionano come i mobili, dai posti dove entrano: il posto di una porta dice su quale muro si trova e in quale stanza porta. Tetti: i pulsanti del tetto automatico nella pagina dei tetti.

Dopo aver posato una porta, la mod dice se i Sim possono attraversarla o cosa la ostacola. Per cambiarla in seguito, trovala in **porte, finestre e scale** o con Control+F: Invio la prende per spostarla, in modalità Acquisto o con la mano della modalità Costruzione, e Canc la rimuove.

## Segnali sonori

Il cursore di costruzione, le frecce del posizionamento e la mappa del lotto emettono un breve suono a ogni passo: il pavimento (erba, legno, piastrelle, moquette, pietra, il bordo della piscina, l'acqua, niente pavimento ai piani superiori: il materiale del pavimento, secondo la sua definizione nel catalogo), il muro, il recinto, la ringhiera, la porta o la finestra di ogni lato (l'ovest a sinistra, l'est a destra, il nord più acuto, il sud più grave), le scale, un Sim o un oggetto sulla casella, una pendenza, un cambio di stanza, il bordo del lotto e, durante il posizionamento, se l'oggetto entra. **Control+Maiusc+S** li disattiva e li riattiva.

## Altri tasti

| Tasto | Cosa fa |
|---|---|
| Control+Maiusc+C | La console dei trucchi del gioco: il suo campo di inserimento è un campo di testo; Invio su di esso gli dà la tastiera, scrivi il trucco, Invio lo esegue, e ciò che la console scrive viene detto. |
| Control+Maiusc+V | Evidenzia sullo schermo il controllo selezionato, per chi ti aiuta e vede. In modalità Vivi, con la finestra del gioco in primo piano, la telecamera guarda anche la cosa del lotto che ha il focus e il puntatore del mouse ci va sopra. |

## I tasti del gioco

Sono del gioco, e funzionano ancora:

| Tasto | Cosa fa |
|---|---|
| F1, F2, F3, F4, F5 | Modalità Vivi, Compra, Costruisci, Telecamera e Opzioni. |
| P | Pausa. |
| 1, 2, 3 | Velocità normale, alta, massima. |
| Spazio, N | Seleziona il Sim successivo. |
| Z, X | Zoom avanti e indietro. |
| Control+Z | Annulla in modalità Compra e Costruisci. |

## Impostazioni

Le impostazioni della mod sono in `Sims2Access\Sims2Access.ini` nella cartella del gioco, sezione `[mod]`. I tasti che le cambiano le salvano subito.

| Impostazione | Valori |
|---|---|
| `narrator` | 2 tutto, 1 solo avvisi, 0 niente (Control+N). |
| `cues` | 1 segnali sonori attivi, 0 disattivati (Control+Maiusc+S). |
| `cue_volume` | Volume dei segnali sonori, da 0 a 100 (predefinito 80). |
| `show_focus` | 1 evidenzia sullo schermo il controllo selezionato (Control+Maiusc+V). |
| `social` | Quali conversazioni vengono dette: 3 tutti, 2 chiunque con la tua famiglia, 1 la tua famiglia, 0 nessuno (Control+Maiusc+N). |
| `hours` | 1 annuncia ogni nuova ora (Control+T). |

## Se qualcosa non va

- **Nessuna voce**: avvia lo screen reader prima del gioco. Il registro della mod è `Sims2Access\Sims2Access.log`; invialo con la tua segnalazione.
- **"Versione del gioco non supportata"**: il gioco si è aggiornato; installa la versione successiva della mod.
- **Un tasto non fa niente**: Control+F1 dice quali tasti funzionano dove ti trovi.

## Non ancora fatto

La carta da parati, i tetti disegnati e il terreno in modalità Costruisci; i nomi dei premi per le aspirazioni; mettere sul lotto un oggetto dell'inventario di un Sim; i nomi di teste, acconciature e vestiti in Crea un Sim; le schermate proprie delle espansioni (università, vita notturna, attività commerciali, stagioni, vacanze, appartamenti, magia) per ora sono lette solo dalla navigazione generica.
