# LifeCycle

## Projekto vizija

### Projekto aprašymas

dinaminės ekosistemos simuliatorius, kuriame augalai, žolėdžiai ir plėšrūnai minta, juda, dauginasi ir miršta pagal paprastus biologinius dėsnius.

### Sistemos aukšto lygio struktūra

#### Sistemos komponentai 
![Component diagram](./diagrams/component-diagram.png)

Dinaminė ekosistemos simuliacijos sistema susideda iš komponentų:
- Naudotojas sąveikauja su grafiniu interfeisu.
- Grafinis naudotojo interfeisas atvaizduoja tinklelį, kuriame matomas simuliacijos veikimas, ir leidžia sąveikauti su simuliacija.
- Simuliacija modeliuoja organizmų sąveiką tarpusavyje.

#### Duomenų tėkmė, įvestis ir išvestis
Žemiau esančiame paveikslėlyje pavaistuota „LifeCycle“ sistemos duomenų tėkmė. Vartotojas pasirenka pradinius parametrus (organizmų skaičių, pasaulio dydį), tuomet sistema sukuria pradinę ekosistemos būseną. Kiekvieno ciklo metu „Simulation Handler“ apskaičiuoja organizmų daromus veiksmus ir atitinkamais pakeitimais atnaujina vartotojo sąsają.

![Data flow diagram](./diagrams/data-flow.png)

### Verslo logika

#### Vartotojo panaudos atvejai
Vartotojas gali nustatyti simuliacijos parametrus kaip pasaulio dydis, augalų bei gyvūnų kiekį ir po to pradėti simuliaciją. Simuliacijos metu vartotojas gali sustabdyti  ir paleisti iš naujo simuliaciją, pereiti prie kito ciklo, pakeisti langelį (pakeisti organizmo stadiją).

![Data flow diagram](./diagrams/use-case.png)

#### Veiklos
Vartotojui pradėjus ekosistemos simuliaciją su pradiniais duomenimis, ji vyksta kol bus sustabdyta vartotojo. Po kiekvieno ciklo, sistema atnaujina vartotojo grafinę sąsają pagal visų organizmų atliktus veiksmus.

![Data flow diagram](./diagrams/simulation-activity.png)

Kiekvieno ciklo metu kiekvienas simuliacijoje esantis organizmas atlieka vieną iš leistinų veiksmų:
- mišta;
- dauginasi;
- maitinasi;
- juda į kitą vietą;
- stovi vietoje.

![Data flow diagram](./diagrams/organism-activity.png)

### Technologijos ir infrastruktūra
- C++
- CMake - projekto konfigūravimui ir kompiliavimui.
- Catch2 - testų rašymui.
- SFML - grafinės sąsajos kūrimui.

## Įgyvendintas Funkcionalumas

### Pagrindiniai komponentai

**Organizmų tipai:**
- **Augalai (Plants)** - auga naudodami maistingąsias medžiagas, dauginasi kai pasiekia tam tikrą maistingumo slenkstį
- **Gyvūnai (Animals)** - gali būti žolėdžiai, mėsėdžiai ar visaėdžiai, juda po aplinką ieškodami maisto, dauginasi kai turi pakankamai maisto medžiagų.

**Aplinkos valdymas:**
- **Position sistema** - tikslus objektų pozicijos valdymas erdvėje
- **Grid sistema** - dvimatė erdvė padalinta į langelius (angl. *tiles*)
- **WorldManager** - valdo visus organizmus ir aplinkos procesus

### Simuliacijos mechanizmai

1. **Augalų elgesys:**
   - Sugeria maistingąsias medžiagas iš aplinkos
   - Dauginasi į gretimus tuščius langelius
   - Miršta pasiekus maksimalų amžių

2. **Gyvūnų elgesys:**
   - Juda po aplinką ieškodami maisto
   - Medžioja pagal savo tipą (žolėdžiai valgo augalus, mėsėdžiai - kitus gyvūnus)
   - Dauginasi turėdami pakankamai maistingumo
   - Vartoja energiją judėdami ir atlikdami kitus veiksmus

3. **Ekosistemos ciklai:**
   - Mirusių organizmų vietoje atsiranda nauji augalai

## Naudojimasis

1. **Paleidus programą** atsidaro vartotojo sąsaja, kur vartotojas gali pasirinkti pradinius simuliacijos nustatymus.
2. 🟢**Žali kvadratėliai** - augalai
3. 🔵**Mėlyni kvadratėliai** - žolėdžiai gyvūnai
4. 🔴**Raudoni kvadratėliai** - mėsėdžiai gyvūnai
5. ⬜**Pilki kvadratėliai** - tušti langeliai

Simuliacija pagal nutylėjimą vyksta 2 FPS greičiu, kad būtų galima stebėti kiekvieno ciklo veiksmus.

## Projektavimo šablonai

### 1. Pimpl (Pointer to Implementation) Idiom
**Vieta:** `Grid.h/GridImpl.h`, `Position.h/PositionImpl.h`, `Tile.h/TileImpl.h`

**Paskirtis:** Atskiria interfeisą nuo implementacijos, sumažina kompiliavimo priklausomybes ir leidžia keisti implementaciją nekeičiant kliento kodo.

### 2. Singleton Pattern
**Vieta:** `WorldManager.h/WorldManager.cpp`

**Paskirtis:** Užtikrina, kad egzistuoja tik vienas WorldManager egzempliorius, kuris valdo visą simuliacijos būseną.