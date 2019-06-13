from math import radians, cos, sin, asin, sqrt

def haversineFormula(latI, lonI, lat, lon):
    """
        Essa formula irá calcular a distância entre dois pontos na terra
        (com lon e lat especificados em graus decimais). Os calculos são
        baseados na terra circular, desconsiderando os efeitos elipsionáis
    """

    #convertendo os graus decimais para radianos
    latI, lonI, lat, lon = map(radians, [latI, lonI, lat, lon])

    #aplicando a fórmula
    dlon = lon - lonI
    dlat = lat - latI
    
    # Quadrado da metade do comprimento do acorde entre os pontos
    a = sin(dlat / 2) ** 2 + cos(latI) * cos(lat) * sin(dlon / 2)  ** 2

    # Distância angular em radianos
    c = 2 * asin(min(1, sqrt(a)))

    R = 6371 # Raio da terra em quilômetros

    return R * c

print(haversineFormula(-22.213794, -54.828892, -22.221791, -54.801958))