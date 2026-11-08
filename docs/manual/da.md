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

Nabolagets første gruppe er **grunde**: hver siger sit navn, sin slags, hvem der bor der (lejligheder med deres lejere) og sin størrelse ("40 gange 30 felter"). Enter på en grund åbner spillets grundpanel.

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

Sim-panelet siger simmens stræben ("Stræben: 24 procent"); belønningspanelet (Belønninger, så Stræberbelønninger) siger de stræberpoint, der kan bruges.

Spillets indstillinger (knappen Indstillinger på kontrolpanelet) læses på samme måde: hvert valg siger sin indstilling ("Skygger: Høj"), og skyderne, som musikkens lydstyrke, ændres med Venstre pil og Højre pil ("Musik, skyder, 35 af 100"). En radiostations sange er en gruppe for sig; Enter slår en sang til eller fra.

Jobsiden siger simmens arbejdstider; Enter på dem tilføjer, hvornår samkørslen kommer. For at sige et job op, ring til arbejdet på telefonen. Hver færdighed siger sit niveau og, når den næste forfremmelse kræver mere, hvor meget: "Madlavning, 2 af 10, den næste forfremmelse kræver 4".

### Vælg Sims og kameraet

| Tast | Hvad den gør |
|---|---|
| Ctrl+1 til Ctrl+9 | Vælger familiens første til niende Sim. |
| Ctrl+E | Den valgte Sim på én linje: hvor, hvad den laver, akutte behov. |
| Ctrl+Skift+E | Hvordan en Sim ser ud: alder, hår, hud, øjne, briller, krop og tøj, med spillets ord. Den Sim der har fokus blandt grundens ting, ellers den valgte. En Sims detaljer (Ctrl+Pil op på den) siger det også. |
| Ctrl+T | Siger hver ny time på uret ("klokken er 14:00"), eller holder op. Nyttigt ved højeste hastighed. |
| Ctrl+Skift+F | Kameraet følger den valgte Sim, eller forbliver frit. |

Kameraet følger den valgte Sim, fordi spillets lyde kommer fra det, der er på skærmen. Når fokus vises på skærmen (Ctrl+Skift+V), glider kameraet hen til hver ting, du går igennem, i stedet for at springe.

Zoom og kameraets retning siges, når de ændrer sig (Z og X zoomer), og kontrolpanelet har dem også: "kamera: tæt på, 5 af 6, mod nordøst". Retningen er den, skærmens top kigger mod.

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
| Ctrl+F | Find en ting: skriv en del af navnet ("tel" finder telefonen, accenter er ligegyldige). Op og Ned går gennem resultaterne, Enter handler på den, Ctrl+Enter går derhen, Delete fjerner en dør eller et vindue, Escape går tilbage, hvor du var. |
| Delete | På en dør eller et vindue, i købstilstand eller med byggetilstandens hånd: samler den op og sælger den med refusion. |

Sims med et job på grunden siger det: "Ravi Pérez, postbud", tjeneren, kokken, gartneren, rengøringshjælpen. Et kæledyrs madskål siger, om der er mad i den. Taxaen og andre biler ved kantstenen er blandt andet, mens de venter.

Efter simmerne kommer **problemer**: det, der kræver opmærksomhed, såsom ødelagte ting, ting der skal rengøres, en tom madskål, vandpytter, affald, aske og ild. Til sidst kommer **døre, vinduer og trapper**. En ting som ingen Sim kan nå fra grundens indgang siger "ingen Sim kan nå den".

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
| Piletaster | Ét felt; med Skift fem. Siger, hvad der står der og hvilken vej det vender, værelset og gulvet når de skifter, væggene, hegnene, rækværkerne, dørene og vinduerne efter side, trapper og skråninger. Vægge, hegn og vinduer stopper markøren; døre lader den komme igennem. |
| Alt+Piletaster | Det samme, gennem vægge. |
| Komma, punktum | Hvad hvert skridt siger: alt, kun genstande, kun gulve (ved navn, "Lys granitflise"), kun vægge, kun rum. |
| Home | Tilbage til den valgte Sim. |
| Ctrl+Skift+Page Up, Page Down | Etagen over eller under. |
| Enter | Handlingsmenuen for det, der står der. En ting på et bord eller skrivebord kommer før bordet: computeren, ikke dens skrivebord. |
| Ctrl+Enter | Sender den valgte Sim derhen (Ctrl+Skift+Enter: løb). |
| Page Down, Page Up (og Skift, Ctrl) | Grundens genstande som altid; kortet går til hver af dem. |
| R | Alt om feltet. Ctrl+L værelsets indretning. |
| Escape eller Ctrl+M | Lukker kortet. |

### Fortælleren

Spillet viser mange ting kun som billeder. Modden siger dem, når de sker, og gemmer dem i hændelsesbufferen:

- **Bobler** over Sims: "Carlos tænker: Energi", "Carlos siger: Politik", et overstreget ikon ("ingen ..."), skrig og overskrifterne om forhold ("forholdet bliver bedre", "ny ven", "første kys").
- **Hvad et måltid mangler**: "Carlos tænker: der mangler et køkkenbord" (en ovn, et komfur, en grill, en mikroovn).
- **Ruter**: "Carlos kan ikke komme derhen".
- **Besøgende ved døren**: "Elvira står ved døren og venter på at blive hilst på"; hils på hende (hendes handlingsmenu) for at lukke hende ind.
- **Ønsker og bekymringer**: "Carlos opfyldte et ønske: Tag ud at svømme, 500 stræberpoint", "Carlos: en bekymring gik i opfyldelse: ...".
- Familiens **handlinger**, **behov** der bliver lave, **besøgende** og penge, der betales eller modtages. Når en handling afbrydes og venter i køen, siges det: "Carlos stoppede Sæt dig for Panik; den venter i køen".
- **Møbler der forlader grunden** (en indbrudstyv, fogeden), med deres værdi.

Din families tanker og skrig siges, og alles skrig. Samtaler (hvad Sims siger og forholdsoverskrifterne) siges som Ctrl+Skift+N vælger; resten går kun til hændelsesbufferen.

**Ctrl+N** ændrer, hvor meget der bliver sagt: alt, kun advarsler eller intet. Linjer, der ikke siges, går stadig til hændelsesbufferen.

**Ctrl+Skift+N** vælger hvis samtaler der siges: alle der taler med din familie (fra start), alle på grunden, kun din familie, eller ingen. Det der ikke siges, går stadig til hændelsesbufferen.

Handlinger med en anden sim nævner den sim, uanset hvem der startede dem: "Carlos: Hilse, med Elvira".

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
| Piletaster | Flytter den i stedet ét felt (Skift: fem): om den kan stå der, hvad den støder mod, værelset når det skifter, væggene omkring den, hvilken vej den vender og hvad den vender mod (også længere væk: "mod Sofa, 3 felter væk"), feltet; derefter feltets lyde. Vægge, hegn og vinduer stopper den; døre lader den komme igennem. |
| Alt+Piletaster | Det samme, gennem vægge. |
| Home | Tilbage til den valgte Sim. |
| Enter | Køber den der; spillet giver dig et nyt eksemplar, indtil du trykker Escape. |
| Escape | Stopper placeringen (en flyttet genstand går tilbage). |
| Delete | Sælger den genstand, du holder. |
| R | Siger pladsen igen, helt. |

En genstand på flere felter siger, hvor meget gulv den fylder ("fylder 2 felter: 2 fra vest til øst, 1 fra nord til syd").

Et siddemøbels pladser begynder der hvor noget bruges siddende, som en computer på et skrivebord, og sædet drejes mod det: "vendt mod vest, til at bruge Computer siddende". Efter at en genstand er placeret siger mod'en hvordan den bruges: "til at bruge Computer siddende", "bruges fra Kontorstol", "Sims kan nå den" eller "ingen Sim kan nå den".

### Flyt og sælg

I købetilstand: gå med Page Down til en af grundens genstande, og tryk Enter: den bliver samlet op, og de samme taster stiller den et andet sted. Delete sælger den. Dens pladser begynder nærmest der hvor den stod.

## Byggetilstand

F3 åbner byggetilstanden. Dens værktøjer og stilarter er knapper.

Et tryk på et værktøj fører til dets stilarter, i en gruppe med værktøjets navn; Tab går tilbage til værktøjerne.

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

Når en dør er placeret, siger modden, om simmer kan gå igennem den, eller hvad der er i vejen. For at ændre den senere, find den i **døre, vinduer og trapper** eller med Ctrl+F: Enter samler den op for at flytte den, i købstilstand eller med byggetilstandens hånd, og Delete fjerner den.

## Lydsignaler

Byggemarkøren, pilene ved placering og grundens kort spiller en kort lyd ved hvert skridt: gulvet (græs, træ, fliser, tæppe, sten, poolkanten, vand, intet gulv ovenpå: hvad gulvet er lavet af, ifølge dets katalogdefinition), hver sides væg, hegn, rækværk, dør eller vindue (vest til venstre, øst til højre, nord højere, syd dybere), trapper, en Sim eller en genstand på feltet, en skråning, et skift af værelse, grundens kant og, ved placering, om genstanden kan stå der. **Ctrl+Skift+S** slår dem fra og til.

## Andre taster

| Tast | Hvad den gør |
|---|---|
| Ctrl+Skift+C | Spillets snydekonsol: dens indtastning er et tekstfelt; Enter på det giver det tastaturet, skriv snydekoden, Enter kører den, og det, den skriver, bliver sagt. |
| Ctrl+Skift+V | Fremhæver det valgte kontrolelement på skærmen for en seende hjælper. I live-tilstand, med spillets vindue forrest, ser kameraet også på den ting på grunden der har fokus, og musemarkøren går hen på den. |

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
| `social` | Hvis samtaler der siges: 3 alle, 2 alle med din familie, 1 din familie, 0 ingen (Ctrl+Skift+N). |
| `hours` | 1 siger hver ny time (Ctrl+T). |

## Når noget går galt

- **Ingen tale overhovedet**: start din skærmlæser før spillet. Moddens log er `Sims2Access\Sims2Access.log`; send den med din rapport.
- **"Spilversionen understøttes ikke"**: spillet er blevet opdateret; installér moddens næste version.
- **En tast gør ingenting**: Ctrl+F1 siger, hvilke taster der virker, hvor du er.

## Ikke færdigt endnu

Tapet, tegnede tage og terræn i byggetilstand; navnene på stræberbelønningerne; at stille en genstand fra en sims inventar på grunden; navne på hoveder, frisurer og tøj i Lav en simmer; udvidelsernes egne skærme (universitet, natteliv, forretning, årstider, ferie, lejligheder, magi) læses indtil videre kun af den generelle navigation.
