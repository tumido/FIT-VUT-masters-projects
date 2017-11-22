# ZADANI
# Vytvorte skript, ktery najde v proteinu par atomu
# (krome sousednich v primarni strukture)
# 1. minimalni
# 2. maximalni
# vzdialenosti a vyznaci je v zobrazeni barvou a vzdalenosti.

# RESENI
# Nalezneme vzechny dvojice atomu, takovych ze atom neni ve dvojici sam se sebou ani se svym sousedem a dvojice jsou unikatni
atoms = set()
for a in cmd.get_model('all').atom:\
    for b in cmd.get_model('all').atom:\
        if a.id == b.id: continue\
        if b.id in map(lambda x: x.id, cmd.get_model('neighbor id %d' % a.id).atom): continue\
        atoms.add(frozenset((a.id, b.id)))

# Kriterium usporadani dvojic je jejich vzdalenost
def distance(x): return cmd.get_distance(atom1='id %d' % x[0], atom2='id %d' % x[1])

# A podle toho urcime maximum a minimum
minimal = min(atoms, key=lambda x: distance(list(x)))
maximal = max(atoms, key=lambda x: distance(list(x)))

# Nakonec tyto atomu oznacime a obarvime
x,y = list(minimal)
cmd.distance('MINIMAL_DISTANCE_%d_%d' % (x, y), 'id %d' % x, 'id %d' % y)
cmd.color('white', 'id %d' % x)
cmd.color('white', 'id %d' % y)

x,y = list(maximal)
cmd.distance('MAXIMAL_DISTANCE_%d_%d' % (x, y), 'id %d' % x, 'id %d' % y)
cmd.color('violet', 'id %d' % x)
cmd.color('violet', 'id %d' % y)
