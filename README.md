# stvrtok-12-00-d-stupavsky-parny-pavol6999

Projekt na git+refaktoring som si vybral Projekt2, na ktorom som 
pracoval na predmete Procedurálne Programovanie (PrPr). Je to projekt, v ktorom pracujeme so spájaným zoznamom.

### Disclaimer
Celý projekt som robil v súlade so zadaním, ktoré sme dostali. To znamená, že som nepridával funkcie, ktoré
by tam podľa zadania (projekt2 PrPr) nemali byť. Nechcel som si takto nahnať viacej commitov.



## Zadanie v skratke
Projekt pracuje so spájaným zoznamom štruktúr (Nehnuteľnosti), pri zadaní určitých
písmen sa spustí funkcia, ktorá je naviazaná na tieto písmená.
Cielom projektu bolo, aby sme sa naučili základné operácie so zoznamom ako je napríklad
mazanie v zozname, vyhľadávanie, aktualizovanie a pridanie novej štruktúry. Funkcie v zdrojovom kóde su okomentované
tak ako aj dôležité riadky v nich.

#### Stručné zadanie projektu, je pridané vo forme PDF 



## Refaktoring
Majoritné zmeny v kóde boli napríklad:
- zjednotenie jazyka premenných
- prevedenie podmienok v cykle v maine na switch, 
- každá funkcia robí jednú vec
  -> napríklad funkcia loadList_file naraz mazala zoznam, 
     počítala záznamy a načítavala nový zoznam
- okomentovanie každej funkcie
- vytvorenie nových funkcií na lepšiu čitatelnosť kódu

## Git + Source Control 
Pomocou gitu som sa snažil použiť čo najviac príkazov. Používal som čisto len konzolu (git bash)
a na vizualizáciu merge-ovania branchov som využíval [Visualize GIT](https://git-school.github.io/visualizing-git/)
Snažil som sa moje zadanie spestriť a tak som použil:
- mergeovanie branchov bez fast-forwardu (--no-ff)
- pull request na gite
- obyčajný merge cez konzolu 
- strom branche-ov som pozeral cez git log --oneline --graph  
- vytvorenie readme cez konzolu
- reset Hlavičky na určity commit(pri merge sa mi master iba fast-forwardol a pri priebežnom odovzdaní som mal iba jeden branch namiesto dvoch)

# Nové funkcie

### freeList
funkcia, ktorá uvoľní zoznam a na koniec nastaví hlavičku na NULL.
bola použitá dvakrát v kóde a to v loadList a v endProgram, takto je tam iba raz
a hocikedy sa na ňu môžem odvolať

### totalReality
spočíta počet štruktúr v súbore reality.txt

### new_node
načíta zo súboru novú štruktúru, funkcia bola potrebná pre lepšiu čitateľnosť funkcie loadList_file

### new_node_input
to isté ako funkcia new_node ale tentokrát je input zo štandartného vstupu a nie zo súboru.
funkcia bola použitá dvakrát pred tým, teraz sa viem na ňu hocikedy odvolať