# ZADANI
# Vytvorte skript, ktery pro aktualne zobrazeny protein vypocita stred proteinu

# RESENI
# Posbirame si souradnice vsech atomu v proteinu a rozdelime je na X, Y a Z souradnice
coords = cmd.get_model('all').get_coord_list()
x = map(lambda i: i[0], coords)
y = map(lambda i: i[1], coords)
z = map(lambda i: i[2], coords)

# Spocitame prumer v kazde ose
x = sum(x) / len(coords)
y = sum(y) / len(coords)
z = sum(z) / len(coords)

# Nalezeny bod ma kazdou souradnici prumernou, tzn. je to geometricky stred
print([x, y, z])
