Tomas Coufal, xcoufa09

# Zadani
V prednasce jsme se seznamili se systemem Biomart/TableBrowser. Pomoci baliku biomaRt v R ziskejte pozice v genomu a
sekvence vsech HOX genu cloveka (HOXA1, HOXB2,... Pokud ziskate pozice odpovidajici vicero prekryvajicim se
transkriptum, vemte nejdelsi z nich / pozice s nejvetsim rozdilem "end" a "start".).  Ziskejte obdobny seznam pres
Table Browser. Posudte jestli jsou ve vysledcich nejake rozdily, pokud ano co by mohlo byt jejich pricinou. Dal
pracujte s jednim z techto seznamu. Pomoci baliku ggplot2 zobrazte histogram delek (rozdeleny priblizne na 10
intervalu). V dalsim kroku analyzujte kazdou sekvenci tak, ze pro kazdou pozici v sekvenci zjistite jak daleko je
nejblizsi cytozin v dinukleotidu CG (potencialni pozice pro metylaci) a tuto informaci vyneseme jako graf (pozice na
osi x, vzdalenost k C na ose y). Pozice v genomu pro ucely analyzy prepocitejte na pozice v genu tak, aby pocatek
transkripce byl 1. Data o HOX genech (pozice exonu v genove DNA) importujte do datove struktury GRanges (balik
GenomicRanges) a spolu s vysledky analyzy sekvence vizualizujte vhodnym nastrojem v R/Bioconductor (viz napr.
http://www.sthda.com/english/wiki/gviz-visualize-genomic-data nebo
https://www.bioconductor.org/packages/devel/bioc/vignettes/ggbio/inst/doc/ggbio.pdf). Pro kazdy gen tedy bude anotace
a graf s vynesenymi ciselnymi hodnotami vzdalenosti. Tyto dvojice bude optimalne umisteny pod sebe do spolecneho
obrazku jako bitmapy nebo PDF.

Odevzdavejte:
- text s kratkym postupem
- tabulka HOX genu s nazvy, jmeny chromosomu, absolutni pozici 5' a 3' konce transkriptu, kodujici sekvence (ATG a STOP kodon), poctem exonu
- obrazek vizualizace genu a vysledku analyzy (pokud se Vam nepovede skloubit, analyzu dejte do zvlastniho obrazku, -1bod; kvuli umisteni na stranku muzete limitovat vizualizaci jenom na stejne dlouhe pocatky genu,radove kolem 1000bp).
- zdrojovy kod vsech skriptu, ktere jste pouzili

# Reseni
## Stazeni dat
Podobne jako jsme postupovali na cviceni, stahneme si data genomu z 'hsapiens_gene_ensembl', z nej zjistime vsechny
geny, ktere v 'external_gene_name' anotaci obsahuji 'HOX'.

## Prekryvani transkriptu
Pomoci IRanges (vytvorene podle zacatku a konce transkriptu), serazenych podle delky, najdeme takove transkripty,
ktere se neprekrivaji. Nasledne vytvorime novy data.frame, do ktereho nahrajeme ty geny, jeichz transkipty jsme nasli
jako neprekryvajici. Nalezene neprekryvajici transkipty vyuzijeme pri tvorbe histogramu.

## Nejblizsi cytozin
Postupne zpracujeme kazdy gen v data framu. Vzdy v jeho sekvenci nalezneme vsechny dinukleotidy CG a pak pro kazdou
pozici v sekvenci nalezneme ten nejblizsi. Zjistena data pote vyneseme do grafu. Zaroven nalezenou sequenci pridame
do tabulky pro export

## Export dat
Pro export vytvorime novy data frame, do ktereho importujeme zadane data z genu a pridame sequence a delku exonu.
Stejne tak vyexportujeme grafy, ulozenim do PNG.
