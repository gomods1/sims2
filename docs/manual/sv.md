---
title: Handbok för Tillgänglighet för The Sims 2
lang: sv
toc-title: Innehåll
---

Tillgänglighet för The Sims 2 gör The Sims 2 Legacy Collection spelbart med en skärmläsare och bara tangentbordet. Modden läser upp varje skärm, dialogruta och panel genom din skärmläsare (NVDA, JAWS, Skärmläsaren och andra), låter dig röra dig genom dem med samma tangenter överallt och lägger till verktyg som en blind spelare behöver: en lista över tomtens föremål, en karta över tomten, en berättare för det spelet bara visar, platser där möbler får plats och en markör för att bygga.

Den här handboken öppnas med **Ctrl+Skift+F1** i spelet, på spelets språk. **Ctrl+F1** säger tangenterna på skärmen du är på.

## Kom igång

1. Kopiera `version.dll` och mappen `Sims2Access` till spelets mapp `EP9\TSBin` (den med `Sims2EP9.exe`), inuti `The Sims 2 Legacy Collection` i ditt Steam-bibliotek.
2. Starta din skärmläsare och starta sedan The Sims 2 Legacy Collection från Steam.
3. När spelfönstret öppnas hör du "Tillgänglighet för The Sims 2 laddad", och var hjälpen finns.

Modden talar spelets språk. För att byta det öppnar du Steam, sedan spelets Egenskaper, Allmänt, Språk. Modden talar engelska, franska, tyska, italienska, spanska, nederländska, danska, svenska, norska, finska, portugisiska, japanska, polska, förenklad och traditionell kinesiska, thai, koreanska, tjeckiska, ungerska och ryska.

Modden fungerar med den nuvarande Steam-versionen av spelet. Om spelet uppdateras och modden säger "spelversionen stöds inte", installera moddens nästa version.

## Röra sig: samma tangenter överallt

Varje skärm, dialogruta, panel och läge fungerar på samma sätt. Det du rör dig genom är en ring av **grupper** (en dialogrutas knappar, behoven, familjen, katalogen...). En grupp innehåller **objekt**, och vissa objekt öppnar en djupare **nivå**.

| Tangent | Vad den gör |
|---|---|
| Tabb, Skift+Tabb | Nästa eller föregående grupp. Gruppens namn och antal objekt sägs först. |
| Pil upp, Pil ned (eller Pil vänster, Pil höger) | Föregående eller nästa objekt i gruppen. |
| Home, End | Första eller sista objektet i gruppen. |
| Enter | Aktiverar objektet. På en nivå (en sort eller ett rum bland tomtens föremål): öppnar den. |
| Backspace | En nivå tillbaka. |
| R | Säger objektet igen, med dess grupp. |
| Alt+bokstav | Nästa grupp vars namn börjar på den bokstaven. |
| Ctrl+F1 | Tangenterna på skärmen du är på. |
| Ctrl+Skift+F1 | Den här handboken. |

Kontroller säger vad de är efter sitt namn, som en skärmläsare gör: "Godkänn, knapp", "Alternativ, knapp, vald", "Blyg till Utåtriktad, skjutreglage, 6 av 10".

- **Skjutreglage** (personligheten i Skapa en sim): Pil vänster och Pil höger ändrar värdet och säger hur många poäng som är kvar.
- **Textfält** (namn på familjer och simmar, fuskkonsolen): gå till fältet med pilarna och skriv: det får tangentbordet så snart fokus är på det. Varje bokstav upprepas, och Backspace säger vad den raderar. Pil vänster, Pil höger, Home och End flyttar i fältet och säger tecknet där; Delete säger tecknet den tar bort.
- **Skrivskyddade objekt** som spelet förklarar när musen hålls över dem (en färdighet, ett intresse, ett yrkesområde): Enter läser upp förklaringen.
- **Inaktiverade kontroller** säger "inte tillgänglig" och aktiveras inte.

Huvudskärmens sista grupp är **modhjälp**: dess knappar säger tangenterna på den skärmen (som Ctrl+F1) och öppnar den här handboken (som Ctrl+Skift+F1).

## Läsa mer: granskningsbuffertarna

Allt får inte plats på en talad rad. Modden har två buffertar som du kan läsa rad för rad:

- **detaljer**: allt om det som just sades: en produkts beskrivning, en kontrolls verktygstips, en plats ruta och riktning, ett föremåls pris och dess plats i gruppen ("3 av 12").
- **händelser**: det som har hänt: köp, försäljningar, texter från dialogrutor, allt berättaren har sagt.

| Tangent | Vad den gör |
|---|---|
| Ctrl+Pil upp | Nästa rad, djupare in i bufferten. |
| Ctrl+Pil ned | Tillbaka mot toppen. |
| Ctrl+Pil vänster, Ctrl+Pil höger | Växlar mellan buffertarna. |

## Grannskapet

Grannskapets första grupp är **tomter**: varje tomt säger sitt namn, sin sort, vem som bor där (lägenheter med sina hyresgäster) och sin storlek ("40 gånger 30 rutor"). Enter på en tomt öppnar spelets tomtpanel.

För att flytta in en familj väljer du familjen i familjekorgen (familjepanelen) och trycker sedan Enter på en tomt: spelet frågar om de ska flytta in och vad det kostar.

## Skapa en familj och simmar

Skapa en familj, Skapa en sim och Skapa ett husdjur är grupper som vilken skärm som helst:

- Skriv familjenamnet och varje sims namn i deras textfält.
- Ålder, kön, hy, kropp och rutnäten med huvuden, frisyrer och kläder ("3 av 6"; husdjursraser med namn).
- **Personlighet**: Pil vänster och Pil höger lägger till eller tar bort ett poäng från varje par av egenskaper ("6 av 10, 4 poäng kvar").
- Ambition, stjärntecken och preferenser är knappar.
- **Släktträdet** (Familjerelationer): Enter på en sim plockar upp den, Enter på en annan sim förenar dem (spelet frågar sedan hur de är släkt), Enter på samma sim lägger ner den igen.

## Tomten: Boendeläge

På en tomt är skärmen en ring av grupper som vilken annan som helst. Tabb går genom dem i den här ordningen:

1. **behov** för den valda simmen, vart och ett med sin nivå.
2. **åtgärdskö**: vad den valda simmen ska göra; Enter avbryter en handling.
3. **föremål efter sort** och **föremål efter rum**: tomtens föremål (se nedan).
4. Spelets paneler: kontrollpanelen, Sim-panelen (behov, relationer, jobb, färdigheter, personlighet, intressen...), familjen, önskningar och rädslor, och varje panel du öppnar.
5. **modhjälp**.

Sim-panelen säger simmens ambition ("Ambition: 24 procent"); belöningspanelen (Belöningar, sedan Ambitionsbelöningar) säger ambitionspoängen du kan spendera.

Spelets alternativ (knappen Alternativ på kontrollpanelen) läses på samma sätt: varje val säger sin inställning ("Skuggor: Hög") och skjutreglagen, som musikens volym, ändras med Pil vänster och Pil höger ("Musik, skjutreglage, 35 av 100"). En radiostations låtar är en egen grupp; Enter slår på eller av en låt.

Jobbsidan säger simmens arbetstider; Enter på dem lägger till när samåkningen kommer. För att säga upp ett jobb, ring till jobbet på telefonen. Varje färdighet säger sin nivå och, när nästa befordran kräver mer, hur mycket: "Matlagning, 2 av 10, nästa befordran kräver 4".

### Välja simmar och kameran

| Tangent | Vad den gör |
|---|---|
| Ctrl+1 till Ctrl+9 | Väljer familjens första till nionde sim. |
| Ctrl+E | Den valda simmen på en rad: var, vad den gör, akuta behov. |
| Ctrl+Skift+E | Hur en Sim ser ut: ålder, hår, hud, ögon, glasögon, kropp och kläder, med spelets ord. Den Sim som har fokus bland tomtens saker, annars den valda. En Sims detaljer (Ctrl+Pil upp på den) säger det också. |
| Ctrl+T | Säger varje ny timme på klockan ("klockan är 14:00"), eller slutar. Användbart på högsta hastighet. |
| Ctrl+Skift+F | Kameran följer den valda simmen, eller förblir fri. |

Kameran följer den valda simmen, eftersom spelets ljud kommer från det som syns på skärmen. När fokus visas på skärmen (Ctrl+Skift+V) glider kameran till varje sak du bläddrar till i stället för att hoppa.

Zoomen och kamerans riktning sägs när de ändras (Z och X zoomar), och kontrollpanelen har dem också: "kamera: nära, 5 av 6, mot nordost". Riktningen är dit skärmens överkant tittar.

### Tomtens föremål

Allt på tomten finns i två grupper: **föremål efter sort** (simmar, sedan köplägets kategorier: sittmöbler, ytor, apparater..., sedan övrigt) och **föremål efter rum** (den valda simmens rum först, sedan de andra, vart och ett med sin våning). Möbler över flera rutor (sängar, soffor, bord) räknas en gång. Enter på en sort eller ett rum listar dess föremål, de närmaste först, som "Kylskåp, kök, 6 rutor åt nordost" från den valda simmen. Backspace går tillbaka till sorterna eller rummen. Rummens namn är moddens gissning utifrån vad som finns i dem.

Du behöver inte gå dit med Tabb: Page-tangenterna bläddrar genom föremålen var som helst på tomten, i boende-, köp- och byggläge.

| Tangent | Vad den gör |
|---|---|
| Page Down, Page Up | Nästa eller föregående föremål av den sort eller i det rum som valdes senast. |
| Skift+Page Down, Skift+Page Up | Nästa eller föregående sort av föremål. |
| Ctrl+Page Down, Ctrl+Page Up | Nästa eller föregående rum (första trycket: den valda simmens rum). |
| Enter | Föremålets interaktioner (i köpläge: plockar upp det för att flytta det). |
| Ctrl+Enter | Skickar den valda simmen till föremålet. |
| Ctrl+Skift+Enter | Spring dit. |
| Ctrl+F | Hitta en sak: skriv en del av namnet ("tel" hittar telefonen, accenter spelar ingen roll). Upp och Ned går genom träffarna, Enter agerar på den, Ctrl+Enter går dit, Delete tar bort en dörr eller ett fönster, Escape går tillbaka dit du var. |
| Delete | På en dörr eller ett fönster, i köpläge eller med byggläges hand: plockar upp den och säljer den med återbetalning. |

Simmar med ett jobb på tomten säger det: "Ravi Pérez, brevbärare", servitören, kocken, trädgårdsmästaren, hembiträdet. Ett husdjurs matskål säger om det finns mat i den. Taxin och andra bilar vid trottoarkanten finns bland övrigt medan de väntar.

Efter simmarna kommer **problem**: det som kräver uppmärksamhet, som trasiga saker, saker som behöver städas, en tom matskål, pölar, skräp, aska och eld. Sist kommer **dörrar, fönster och trappor**. En sak som ingen Sim kan nå från tomtens ingång säger "ingen Sim kan nå den".

### Interaktionsmenyer

Enter på ett föremål (eller på en sim) öppnar spelets interaktionsmeny. Pil upp och Pil ned väljer, Enter utför. Poster med fler val öppnar en djupare meny. När menyn stängs är fokus tillbaka på föremålet.

### Rum

| Tangent | Vad den gör |
|---|---|
| Ctrl+L | Rummets planlösning (vid byggmarkören eller kartans markör, annars där den valda simmen står): namn, våning, rutor, gränser, dörrar och fönster; varje dörr och fönster, med sin vägg och rummet bakom, finns i detaljbufferten. |

### Tomtens karta

**Ctrl+M** sätter på kartan: en markör som går genom tomten ruta för ruta, med start vid den valda simmen. Varje steg spelar ett ljud för golvet och för varje vägg, staket, räcke, dörr eller fönster runt om.

| Tangent | Vad den gör |
|---|---|
| Pilar | En ruta; med Skift fem. Säger vad som står där och åt vilket håll det är vänt, rummet och golvet när de byts, väggarna, staketen, räckena, dörrarna och fönstren efter sida, trappor och sluttningar. Väggar, staket och fönster stoppar markören; dörrar släpper igenom den. |
| Alt+Pilar | Samma sak, genom väggar. |
| Komma, punkt | Vad varje steg säger: allt, bara föremål, bara golv (med namn, "Ljus granitplatta"), bara väggar, bara rum. |
| Home | Tillbaka till den valda simmen. |
| Ctrl+Skift+Page Up, Page Down | Våningen ovanför eller nedanför. |
| Enter | Interaktionsmenyn för det som står där. En sak på ett bord eller skrivbord kommer före bordet: datorn, inte dess skrivbord. |
| Ctrl+Enter | Skickar dit den valda simmen (Ctrl+Skift+Enter: spring). |
| Page Down, Page Up (och Skift, Ctrl) | Tomtens föremål som vanligt; kartan går till vart och ett. |
| R | Allt om rutan. Ctrl+L rummets planlösning. |
| Escape eller Ctrl+M | Stänger kartan. |

### Berättaren

Spelet visar många saker bara som bilder. Modden säger dem när de händer och sparar dem i händelsebufferten:

- **Pratbubblor** över simmar: "Carlos tänker: Energi", "Carlos säger: Politik", en överkryssad ikon ("ingen ..."), skrik och rubrikerna om relationer ("relationen förbättras", "ny vän", "första kyssen").
- **Vad en måltid saknar**: "Carlos tänker: det behövs en köksbänk" (en ugn, en spis, en grill, en mikrovågsugn).
- **Vägar**: "Carlos kan inte ta sig dit".
- **Besökare vid dörren**: "Elvira står vid dörren och väntar på att bli hälsad på"; hälsa på henne (hennes interaktionsmeny) för att släppa in henne.
- **Önskningar och farhågor**: "Carlos uppfyllde en önskan: Ta en simtur, 500 ambitionspoäng", "Carlos: en farhåga besannades: ...".
- Familjens **handlingar**, **behov** som blir låga, **besökare** och pengar som betalas eller tas emot. När en handling avbryts och väntar i kön sägs det: "Carlos slutade Sätta sig för Panik; den väntar i kön".
- **Möbler som lämnar tomten** (en inbrottstjuv, utmätningsmannen), med sitt värde.

Din familjs tankar och skrik sägs, och allas skrik. Samtal (vad simmar säger och relationsrubrikerna) sägs som Ctrl+Skift+N väljer; resten hamnar bara i händelsebufferten.

**Ctrl+N** ändrar hur mycket som sägs: allt, bara varningar eller inget. Rader som inte sägs hamnar ändå i händelsebufferten.

**Ctrl+Skift+N** väljer vems samtal som sägs: alla som pratar med din familj (från början), alla på tomten, bara din familj, eller ingen. Det som inte sägs hamnar ändå i händelsebufferten.

Handlingar med en annan sim nämner den simmen, vem som än började: "Carlos: Hälsa, med Elvira".

## Köpläge

F2 öppnar köpläget. Katalogens produkter är knappar: varje produkt säger sitt namn, sitt pris och sina betyg, och detaljbufferten innehåller dess beskrivning. Går du förbi en sidas sista produkt (eller före den första) bläddras sidan.

### Placera ett föremål

Enter på en produkt plockar upp den. Modden hittar varje plats på tomten där den får plats, åt alla håll, de närmaste den valda simmen först.

| Tangent | Vad den gör |
|---|---|
| Page Down, Page Up (eller Tabb) | Platserna, de närmaste först; varje plats säger sitt rum, de riktningar den får plats i och hur långt bort den är. |
| Ctrl+Page Down, Ctrl+Page Up | De rum som har platser. |
| Skift+Page Down, Skift+Page Up (eller komma, punkt) | Vrider den; riktningen sägs. |
| Ctrl+Skift+Page Down, Page Up | Bredvid tomtens föremål, de närmaste först, vänd mot dem när det går. |
| Pilar | Flyttar den i stället en ruta (Skift: fem): om den får plats, vad den krockar med, rummet när det byts, väggarna runt den, åt vilket håll den är vänd och vad den är vänd mot (även längre bort: "mot Soffa, 3 rutor bort"), rutan; sedan rutans ljud. Väggar, staket och fönster stoppar det; dörrar släpper igenom det. |
| Alt+Pilar | Samma sak, genom väggar. |
| Home | Tillbaka till den valda simmen. |
| Enter | Köper den där; spelet ger dig ett nytt exemplar tills du trycker Escape. |
| Escape | Slutar placera (ett flyttat föremål går tillbaka). |
| Delete | Säljer föremålet du håller. |
| R | Säger platsen igen, i sin helhet. |

Ett föremål som tar flera rutor säger hur mycket golv det tar ("tar 2 rutor: 2 från väst till öst, 1 från norr till söder").

En sittplats ställen börjar där något används sittande, som en dator på ett skrivbord, och sätet vrids mot det: "vänd mot väster, för att använda Dator sittande". När ett föremål har placerats säger moddet hur det kommer att användas: "för att använda Dator sittande", "används från Kontorsstol", "simmar kan nå den" eller "ingen Sim kan nå den".

### Flytta och sälja

I köpläge: gå med Page Down till ett av tomtens föremål och tryck Enter: det plockas upp, och samma tangenter ställer det någon annanstans. Delete säljer det. Dess ställen börjar närmast där det stod.

## Byggläge

F3 öppnar byggläget. Dess verktyg och stilar är knappar.

Att trycka på ett verktyg leder till dess stilar, i en grupp med verktygets namn; Tab går tillbaka till verktygen.

### Byggmarkören

Enter på en stil för väggar, staket, underlag, golv eller pooler startar byggmarkören vid den valda simmen. Väggar, staket och underlag går från hörn till hörn; golv och pooler täcker rutor.

| Tangent | Vad den gör |
|---|---|
| Pilar | Ett hörn eller en ruta; med Skift fem. Säger vad som står där, rummet när det byts, golvet (för golv och pooler) eller väggarna, staketen, dörrarna och fönstren som möts i hörnet, en sluttning, sedan var den är. |
| Page Up, Page Down | Nivån ovanför eller nedanför (0 terrängen, 1 golvet på ett underlag). |
| Mellanslag | Markerar början. |
| Enter | Bygger från början till markören; kostnaden sägs. |
| Ctrl+Enter | Tar bort från början till markören. |
| Home | Tillbaka till den valda simmen. |
| R | Allt om markören. |
| Escape | Lämnar markören. |

När en vägg har byggts meddelas det om en sim i familjen är instängd i ett rum utan dörr; spelets Ctrl+Z ångrar väggen.

### Dörrar, fönster, trappor och tak

Dörrar, fönster och trappor placeras som möbler, från platserna där de får plats: en dörrs plats säger vilken vägg den sitter i och vilket rum den leder till. Tak: knapparna för automatiska tak på taksidan.

När en dörr har placerats säger modden om simmar kan gå igenom den eller vad som är i vägen. För att ändra den senare, hitta den i **dörrar, fönster och trappor** eller med Ctrl+F: Enter plockar upp den för att flytta den, i köpläge eller med byggläges hand, och Delete tar bort den.

## Ljudsignaler

Byggmarkören, pilarna vid placering och tomtens karta spelar ett kort ljud vid varje steg: golvet (gräs, trä, kakel, heltäckningsmatta, sten, poolkanten, vatten, inget golv på övervåningen: vad golvet är gjort av, enligt dess katalogdefinition), varje sidas vägg, staket, räcke, dörr eller fönster (väst till vänster, öst till höger, norr högre, söder lägre), trappor, en sim eller ett föremål på rutan, en sluttning, ett rumsbyte, tomtens kant och, vid placering, om föremålet får plats. **Ctrl+Skift+S** stänger av och sätter på dem.

## Andra tangenter

| Tangent | Vad den gör |
|---|---|
| Ctrl+Skift+C | Spelets fuskkonsol: dess inmatning är ett textfält; Enter på det ger det tangentbordet, skriv fusket, Enter kör det, och det den skriver läses upp. |
| Ctrl+Skift+V | Lyser upp den valda kontrollen på skärmen, för en seende hjälpare. I leva-läget, med spelets fönster främst, tittar kameran också på den sak på tomten som har fokus och muspekaren går till den. |

## Spelets egna tangenter

Dessa hör till spelet och fungerar fortfarande:

| Tangent | Vad den gör |
|---|---|
| F1, F2, F3, F4, F5 | Boende-, köp-, bygg-, kamera- och alternativläge. |
| P | Paus. |
| 1, 2, 3 | Normal, snabb, snabbaste hastighet. |
| Mellanslag, N | Väljer nästa sim. |
| Z, X | Zoomar in och ut. |
| Ctrl+Z | Ångra i köp- och byggläge. |

## Inställningar

Moddens inställningar finns i `Sims2Access\Sims2Access.ini` i spelets mapp, avsnittet `[mod]`. Tangenterna som ändrar dem sparar dem direkt.

| Inställning | Värden |
|---|---|
| `narrator` | 2 allt, 1 bara varningar, 0 av (Ctrl+N). |
| `cues` | 1 ljudsignaler på, 0 av (Ctrl+Skift+S). |
| `cue_volume` | Ljudsignalernas volym, 0 till 100 (standard 80). |
| `show_focus` | 1 lyser upp den valda kontrollen på skärmen (Ctrl+Skift+V). |
| `social` | Vems samtal som sägs: 3 allas, 2 alla med din familj, 1 din familj, 0 ingen (Ctrl+Skift+N). |
| `hours` | 1 säger varje ny timme (Ctrl+T). |

## När något går fel

- **Inget tal alls**: starta din skärmläsare före spelet. Moddens logg är `Sims2Access\Sims2Access.log`; skicka med den i din rapport.
- **"Spelversionen stöds inte"**: spelet har uppdaterats; installera moddens nästa version.
- **En tangent gör ingenting**: Ctrl+F1 säger vilka tangenter som fungerar där du är.

## Inte klart än

Tapeter, ritade tak och terräng i byggläge; namnen på ambitionsbelöningarna; att ställa ett föremål från en sims inventarie på tomten; namn på huvuden, frisyrer och kläder i Skapa en sim; expansionernas egna skärmar (universitet, nattliv, företag, årstider, semester, lägenheter, magi) läses än så länge bara av den allmänna navigeringen.
