# ZADANI
# Napiste skript pro Pymol, ktery v libovolne strukture:
# 1. identifikuje cysteiny
# 2. obarvi je na zluto
# 3. spocita jejich vzdalenost
# 4. ty, ktere by mohly tvorit cysteinove mostiky, obarvi oranzovou a vypise ve strukture jejich identifikator a delku vazby

# RESENI
# Nejdrive vyhledame vsechny cysteiny a obarvime je
select CYSTEINES, resn cys
color yellow, CYSTEINES

# Nasledne najdeme jejich disulfidy a spocitame vzdalenosti mezi cysteiny
select DISULPHIDES, cys/sg
distance DISTANCES, DISULPHIDES

# Nakonec hledame potencialni mosty
# Vyhledame vsechny dvojice disulfidu (tak aby nebyly dvojice sam se sebou a duplikaty obracenych dvojic)
disulphides = cmd.get_model('DISULPHIDES').atom
disulphides = set(frozenset((a.id,b.id)) for a in disulphides for b in disulphides if a != b)

# Za maximalni vzdalenost povazujeme 2.2 angstormu (kvuli toleranci, prestoze ve skutecnosti je to 2.05)
max_distance = 2.2

# Pro kazdou dvojici spocitame vzdalenost a pokud je tato kratsi nez maximalni dovolena obarvime a popiseme
for x,y in disulphides:\
    if cmd.get_distance(atom1='id %d' % x, atom2='id %d' % y) < max_distance:\
        cmd.color('orange', 'id %d' % x)\
        cmd.color('orange', 'id %d' % y)\
        cmd.label('id %d' % x, str(x))\
        cmd.label('id %d' % y, str(y))\
        cmd.distance('DISTANCE_%d_%d' % (x, y), 'id %d' % x, 'id %d' % y)
