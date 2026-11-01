---
title: Manual til Sims 2-tilgængelighed
lang: da
toc-title: Indhold
---

Sims 2-tilgængelighed gør The Sims 2 Legacy Collection spilbart med en skærmlæser og kun tastaturet. Modden læser hver skærm, hver dialogboks og hvert panel op gennem din skærmlæser (NVDA, JAWS, Oplæser og andre), lader dig bevæge dig rundt i dem med de samme taster overalt og tilføjer de værktøjer, en blind spiller har brug for: en liste over grundens genstande, et kort over grunden, en fortæller for det, spillet kun viser, steder hvor møbler kan stå, og en markør til at bygge.

Denne manual åbnes med **Ctrl+Skift+F1** i spillet, på spillets sprog. **Ctrl+F1** siger tasterne på den skærm, du er på.

## Kom i gang

1. Kopiér `version.dll` og mappen `Sims2Access` ind i spillets mappe `EP9\TSBin` (den med `Sims2EP9.exe`), inde i `The Sims 2 Legacy Collection` i dit Steam-bibliotek.
2. Start din skærmlæser, og start derefter The Sims 2 Legacy Collection fra Steam.
3. Når spilvinduet åbner, hører du "Sims 2-tilgængelighed indlæst", og hvor hjælpen er.

Modden taler spillets sprog. For at skifte det skal du åbne Steam, så spillets Egenskaber, Generelt, Sprog. Modden taler engelsk, fransk, tysk, italiensk, spansk, hollandsk, dansk, svensk, norsk, finsk, portugisisk, japansk, polsk, forenklet og traditionelt kinesisk, thai, koreansk, tjekkisk, ungarsk og russisk.

Modden virker med den nuværende Steam-version af spillet. Hvis spillet bliver opdateret, og modden siger "spilversionen understøttes ikke", skal du installere moddens næste version.

## Bevæg dig rundt: de samme taster overalt

Hver skærm, dialogboks, hvert panel og hver tilstand virker på samme måde. Det, du bevæger dig igennem, er en ring af **grupper** (en dialogboks' knapper, behovene, familien, kataloget...). En gruppe indeholder **elementer**, og nogle elementer åbner et dybere **niveau**.

| Tast | Hvad den gør |
|---|---|
| Tab, Skift+Tab | Næste eller forrige gruppe. Gruppens navn og antal elementer siges først. |
| Pil op, Pil ned (eller Venstre pil, Højre pil) | Forrige eller næste element i gruppen. |
| Home, End | Første eller sidste element i gruppen. |
| Enter | Aktiverer elementet. På et niveau (en slags eller et værelse blandt grundens genstande): åbner det. |
| Backspace | Et niveau tilbage. |
| R | Siger elementet igen, med dets gruppe. |
| Alt+bogstav | Den næste gruppe, hvis navn begynder med det bogstav. |
| Ctrl+F1 | Tasterne på den skærm, du er på. |
| Ctrl+Skift+F1 | Denne manual. |

Kontrolelementer siger, hvad de er, efter deres navn, som en skærmlæser gør: "Acceptér, knap", "Indstillinger, knap, valgt", "Genert til Udadvendt, skyder, 6 af 10".

- **Skydere** (personligheden i Lav en simmer): Venstre pil og Højre pil ændrer værdien og siger, hvor mange point der er tilbage.
- **Tekstfelter** (navne på familier og Sims, snydekonsollen): gå til feltet med pilene, og skriv: det får tastaturet, så snart fokus er på det. Hvert bogstav gentages, og Backspace siger, hvad den sletter. Venstre pil, Højre pil, Home og End flytter i feltet og siger tegnet der; Delete siger det tegn, den fjerner.
- **Skrivebeskyttede elementer**, som spillet forklarer, når musen holdes over dem (en færdighed, en interesse, et jobområde): Enter siger forklaringen.
- **Deaktiverede kontrolelementer** siger "ikke tilgængelig" og aktiveres ikke.

Den sidste gruppe på hovedskærmen er **hjælp til modden**: dens knapper siger tasterne på den skærm (som Ctrl+F1) og åbner denne manual (som Ctrl+Skift+F1).

## Læs mere: gennemgangsbufferne

Ikke alt kan være i én talt linje. Modden har to buffere, som du kan læse linje for linje:

- **detaljer**: alt om det, der lige blev sagt: et produkts beskrivelse, et kontrolelements værktøjstip, en plads' felt og retning, en genstands pris og dens plads i gruppen ("3 af 12").
- **hændelser**: det, der er sket: køb, salg, tekster fra dialogbokse, alt hvad fortælleren har sagt.

| Tast | Hvad den gør |
|---|---|
| Ctrl+Pil op | Næste linje, dybere i bufferen. |
| Ctrl+Pil ned | Tilbage mod toppen. |
| Ctrl+Venstre pil, Ctrl+Højre pil | Skifter mellem bufferne. |

## Nabolaget

Nabolagets første gruppe er **grunde**: hver siger sit navn, sin slags og hvem der bor der (lejligheder med deres lejere). Enter på en grund åbner spillets grundpanel.

For at flytte en familie ind skal du vælge familien i familiebeholderen (familiepanelet) og derefter trykke Enter på en grund: spillet spørger, om de skal flytte ind, og hvad det koster.

## Skab en familie og Sims

Lav familie, Lav en simmer og Lav et kæledyr er grupper som enhver skærm:

- Skriv familienavnet og hver Sims navne i deres tekstfelter.
- Alder, køn, hud, krop og gitrene med hoveder, frisurer og tøj ("3 af 6"; kæledyrsracer med navn).
- **Personlighed**: Venstre pil og Højre pil giver eller tager et point fra hvert par af træk ("6 af 10, 4 point tilbage").
- Stræben, stjernetegn og tiltrækning er knapper.
- **Stamtræet** (Familieforhold): Enter på en Sim samler den op, Enter på en anden Sim forbinder dem (spillet spørger så, hvordan de er i familie), Enter på den samme Sim lægger den ned igen.

## Grunden: Lev!

På en grund er skærmen en ring af grupper som enhver anden. Tab går gennem dem i denne rækkefølge:

1. **behov** for den valgte Sim, hver med sit niveau.
2. **handlingskø**: hvad den valgte Sim vil gøre; Enter annullerer en handling.
3. **genstande efter slags** og **genstande efter værelse**: grundens genstande (se nedenfor).
4. Spillets paneler: kontrolpanelet, Sim-panelet (behov, forhold, job, færdigheder, personlighed, interesser...), familien, ønsker og frygt, og ethvert panel du åbner.
5. **hjælp til modden**.

### Vælg Sims og kameraet

| Tast | Hvad den gør |
|---|---|
| Ctrl+1 til Ctrl+9 | Vælger familiens første til niende Sim. |
| Ctrl+E | Den valgte Sim på én linje: hvor, hvad den laver, akutte behov. |
| Ctrl+F | Kameraet følger den valgte Sim, eller forbliver frit. |

Kameraet følger den valgte Sim, fordi spillets lyde kommer fra det, der er på skærmen.

### Grundens genstande

Alt på grunden er i to grupper: **genstande efter slags** (Simmere, så købetilstandens kategorier: siddemøbler, overflader, apparater..., så andet) og **genstande efter værelse** (den valgte Sims værelse først, så de andre, hver med sin etage). Møbler over flere felter (senge, sofaer, borde) tælles én gang. Enter på en slags eller et værelse viser dens genstande, de nærmeste først, som "Køleskab, køkken, 6 fliser mod nordøst" fra den valgte Sim. Backspace går tilbage til slagsene eller værelserne. Værelsernes navne er moddens gæt ud fra det, der står i dem.

Du behøver ikke at gå derhen med Tab: Page-tasterne går gennem genstandene fra hvor som helst på grunden, i Lev!, Køb og Byg.

| Tast | Hvad den gør |
|---|---|
| Page Down, Page Up | Næste eller forrige genstand af den slags eller i det værelse, der sidst blev valgt. |
| Skift+Page Down, Skift+Page Up | Næste eller forrige slags genstande. |
| Ctrl+Page Down, Ctrl+Page Up | Næste eller forrige værelse (første tryk: den valgte Sims værelse). |
| Enter | Genstandens handlinger (i købetilstand: samler den op for at flytte den). |
| Ctrl+Enter | Sender den valgte Sim hen til genstanden. |
| Ctrl+Skift+Enter | Løb derhen. |

### Handlingsmenuer

Enter på en genstand (eller på en Sim) åbner spillets handlingsmenu. Pil op og Pil ned vælger, Enter udfører. Punkter med flere valg åbner en dybere menu. Når menuen lukker, er fokus tilbage på genstanden.

### Værelser

| Tast | Hvad den gør |
|---|---|
| Ctrl+L | Værelsets indretning (ved byggemarkøren eller kortets markør, ellers hvor den valgte Sim står): navn, etage, felter, grænser, døre og vinduer; hver dør og hvert vindue, med sin væg og værelset bagved, er i detaljebufferen. |

### Grundens kort

**Ctrl+M** slår kortet til: en markør, der går gennem grunden felt for felt, med start ved den valgte Sim. Hvert skridt spiller en lyd for gulvet og for hver væg, hvert hegn, hvert rækværk, hver dør eller hvert vindue omkring.

| Tast | Hvad den gør |
|---|---|
| Piletaster | Ét felt; med Skift fem. Siger, hvad der står der og hvilken vej det vender, værelset og gulvet når de skifter, væggene, hegnene, rækværkerne, dørene og vinduerne efter side, trapper og skråninger. |
| Home | Tilbage til den valgte Sim. |
| Ctrl+Skift+Page Up, Page Down | Etagen over eller under. |
| Enter | Handlingsmenuen for det, der står der. |
| Ctrl+Enter | Sender den valgte Sim derhen (Ctrl+Skift+Enter: løb). |
| Page Down, Page Up (og Skift, Ctrl) | Grundens genstande som altid; kortet går til hver af dem. |
| R | Alt om feltet. Ctrl+L værelsets indretning. |
| Escape eller Ctrl+M | Lukker kortet. |

### Fortælleren

Spillet viser mange ting kun som billeder. Modden siger dem, når de sker, og gemmer dem i hændelsesbufferen:

- **Bobler** over Sims: "Carlos tænker: Energi", "Carlos siger: Politik", et overstreget ikon ("ingen ..."), skrig og overskrifterne om forhold ("forholdet bliver bedre", "ny ven", "første kys").
- **Ruter**: "Carlos kan ikke komme derhen".
- Familiens **handlinger**, **behov** der bliver lave, **besøgende** og penge, der betales eller modtages.
- **Møbler der forlader grunden** (en indbrudstyv, fogeden), med deres værdi.

Familiens bobler bliver sagt; en besøgendes går kun til hændelsesbufferen.

**Ctrl+N** ændrer, hvor meget der bliver sagt: alt, kun advarsler eller intet. Linjer, der ikke siges, går stadig til hændelsesbufferen.

## Købetilstand

F2 åbner købetilstanden. Katalogets produkter er knapper: hvert produkt siger sit navn, sin pris og sine vurderinger, og detaljebufferen indeholder beskrivelsen. Går du forbi en sides sidste produkt (eller før det første), bladres der om.

### Placér en genstand

Enter på et produkt samler det op. Modden finder hver plads på grunden, hvor den kan stå, i alle retninger, de nærmeste til den valgte Sim først.

| Tast | Hvad den gør |
|---|---|
| Page Down, Page Up (eller Tab) | Pladserne, de nærmeste først; hver siger sit værelse, de retninger den kan stå i, og hvor langt væk den er. |
| Ctrl+Page Down, Ctrl+Page Up | De værelser, der har pladser. |
| Skift+Page Down, Skift+Page Up (eller komma, punktum) | Drejer den; retningen siges. |
| Ctrl+Skift+Page Down, Page Up | Ved siden af grundens genstande, de nærmeste først, vendt mod dem når det kan lade sig gøre. |
| Piletaster | Flytter den i stedet ét felt (Skift: fem): om den kan stå der, hvad den støder mod, værelset når det skifter, væggene omkring den, hvilken vej den vender og hvad den vender mod, feltet; derefter feltets lyde. |
| Home | Tilbage til den valgte Sim. |
| Enter | Køber den der; spillet giver dig et nyt eksemplar, indtil du trykker Escape. |
| Escape | Stopper placeringen (en flyttet genstand går tilbage). |
| Delete | Sælger den genstand, du holder. |
| R | Siger pladsen igen, helt. |

En genstand på flere felter siger, hvor meget gulv den fylder ("fylder 2 felter: 2 fra vest til øst, 1 fra nord til syd").

### Flyt og sælg

I købetilstand: gå med Page Down til en af grundens genstande, og tryk Enter: den bliver samlet op, og de samme taster stiller den et andet sted. Delete sælger den.

## Byggetilstand

F3 åbner byggetilstanden. Dens værktøjer og stilarter er knapper.

### Byggemarkøren

Enter på en stil for vægge, hegn, sokler, gulve eller pools starter byggemarkøren ved den valgte Sim. Vægge, hegn og sokler går fra hjørne til hjørne; gulve og pools dækker felter.

| Tast | Hvad den gør |
|---|---|
| Piletaster | Ét hjørne eller felt; med Skift fem. Siger, hvad der står der, værelset når det skifter, gulvet (ved gulve og pools) eller væggene, hegnene, dørene og vinduerne, der mødes i hjørnet, en skråning, og derefter hvor den er. |
| Page Up, Page Down | Niveauet over eller under (0 terrænet, 1 gulvet på en sokkel). |
| Mellemrum | Markerer starten. |
| Enter | Bygger fra starten til markøren; prisen siges. |
| Ctrl+Enter | Fjerner fra starten til markøren. |
| Home | Tilbage til den valgte Sim. |
| R | Alt om markøren. |
| Escape | Forlader markøren. |

Når en væg er bygget, meldes det, hvis en Sim fra familien er lukket inde i et værelse uden dør; spillets Ctrl+Z fortryder væggen.

### Døre, vinduer, trapper og tage

Døre, vinduer og trapper placeres som møbler, fra de pladser hvor de kan stå: en dørs plads siger, hvilken væg den sidder i, og hvilket værelse den fører til. Tage: knapperne til automatiske tage på tagsiden.

## Lydsignaler

Byggemarkøren, pilene ved placering og grundens kort spiller en kort lyd ved hvert skridt: gulvet (græs, træ, fliser, tæppe, sten, poolkanten, vand, intet gulv ovenpå: hvad gulvet er lavet af, ifølge dets katalogdefinition), hver sides væg, hegn, rækværk, dør eller vindue (vest til venstre, øst til højre, nord højere, syd dybere), trapper, en Sim eller en genstand på feltet, en skråning, et skift af værelse, grundens kant og, ved placering, om genstanden kan stå der. **Ctrl+Skift+S** slår dem fra og til.

## Andre taster

| Tast | Hvad den gør |
|---|---|
| Ctrl+Skift+C | Spillets snydekonsol: dens indtastning er et tekstfelt; Enter på det giver det tastaturet, skriv snydekoden, Enter kører den, og det, den skriver, bliver sagt. |
| Ctrl+Skift+V | Fremhæver det valgte kontrolelement på skærmen for en seende hjælper. |

## Spillets egne taster

Disse hører til spillet og virker stadig:

| Tast | Hvad den gør |
|---|---|
| F1, F2, F3, F4, F5 | Lev!, Køb, Byg, kamera og indstillinger. |
| P | Pause. |
| 1, 2, 3 | Normal, hurtig, hurtigste hastighed. |
| Mellemrum, N | Vælger den næste Sim. |
| Z, X | Zoomer ind og ud. |
| Ctrl+Z | Fortryd i købe- og byggetilstand. |

## Indstillinger

Moddens indstillinger ligger i `Sims2Access\Sims2Access.ini` i spillets mappe, afsnittet `[mod]`. De taster, der ændrer dem, gemmer dem med det samme.

| Indstilling | Værdier |
|---|---|
| `narrator` | 2 alt, 1 kun advarsler, 0 fra (Ctrl+N). |
| `cues` | 1 lydsignaler til, 0 fra (Ctrl+Skift+S). |
| `cue_volume` | Lydsignalernes lydstyrke, 0 til 100 (standard 80). |
| `show_focus` | 1 fremhæver det valgte kontrolelement på skærmen (Ctrl+Skift+V). |

## Når noget går galt

- **Ingen tale overhovedet**: start din skærmlæser før spillet. Moddens log er `Sims2Access\Sims2Access.log`; send den med din rapport.
- **"Spilversionen understøttes ikke"**: spillet er blevet opdateret; installér moddens næste version.
- **En tast gør ingenting**: Ctrl+F1 siger, hvilke taster der virker, hvor du er.

## Ikke færdigt endnu

Tapet, tegnede tage og terræn i byggetilstand; navne på hoveder, frisurer og tøj i Lav en simmer; udvidelsernes egne skærme (universitet, natteliv, forretning, årstider, ferie, lejligheder, magi) læses indtil videre kun af den generelle navigation.
