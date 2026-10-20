# Caja Detective (Arduino Uno + PlatformIO)

## Estructura
- `include/Config.h`: pines y ajustes (único lugar a editar).
- `include/Hardware.h`: agrupa botones, LEDs, buzzer, cerradura y EEPROM.
- `lib/`: un módulo por componente (`Button`, `Leds`, `Buzzer`, `Lock`, `KeyStore`) y `Logic` (lógica pura).
- `src/Game.*`: máquina de estados del juego (sin `delay`). `src/main.cpp`: arranque.
- `tools/`: pruebas de hardware. `test/`: pruebas unitarias en la PC.

## Pruebas de hardware (subir y abrir el monitor serial)
    pio run -e test_leds    -t upload && pio device monitor
    pio run -e test_buttons -t upload && pio device monitor
    pio run -e test_buzzer  -t upload && pio device monitor
    pio run -e test_relay   -t upload && pio device monitor

## Pruebas en la PC (sin placa): lógica y juego completo (Game) con Arduino simulado
    pio test -e native

## Programa final
    pio run -t upload              # normal
    pio run -e uno_debug -t upload # con mensajes por serial

## Historias y preguntas

Cada caso tiene 5 preguntas de opción múltiple (A, B, C, D). La secuencia de respuestas correctas es la contraseña de ese caso. Las respuestas correctas están en **negritas**.

El caso 7 (B-B-B-B-B) es solo de prueba y no tiene historia.

### Caso 1: El perro desaparecido

**Contraseña:** C-C-A-B-D

Ayer por la tarde, Luna, la perra de Sofía, desapareció del jardín. Sofía vio que la reja estaba abierta y en el piso había huellas de patas pequeñas y un pedazo de salchicha. Las huellas llevaban al parque. Cerca de la banca, el vendedor de globos dijo: «Vi una perrita café con un listón rojo comiendo salchicha. Corrió hacia la panadería». En la panadería, la dueña contó que la perrita se escondió debajo de una mesa porque le asustó la tormenta. Sofía la encontró ahí, mojada y temblando, y la abrazó.

1. ¿Cómo se llama la perra?
   - A) Mocha
   - B) Nube
   - C) **Luna**
   - D) Canela
2. ¿Por dónde se escapó?
   - A) Por la ventana
   - B) Por el techo
   - C) **Por la reja abierta**
   - D) Por la puerta de la casa
3. ¿Qué había en el piso, además de huellas?
   - A) **Un pedazo de salchicha**
   - B) Un hueso
   - C) Un listón
   - D) Un globo
4. ¿Quién vio a la perrita en el parque?
   - A) Un policía
   - B) **El vendedor de globos**
   - C) Una maestra
   - D) El panadero
5. ¿Dónde la encontró Sofía?
   - A) En la banca del parque
   - B) En el jardín
   - C) Junto a la reja
   - D) **En la panadería**

### Caso 2: La nota misteriosa

**Contraseña:** A-D-B-B-C

Mateo encontró una nota doblada dentro de su mochila. Decía: «Búscame donde guardan los libros, en el tercer estante, junto al libro verde». La letra era cuadrada y estaba escrita con lápiz azul. Mateo pensó en sus compañeros: Ana usa lápiz rosa, Beto escribe siempre con pluma negra y Diego usa lápiz azul. En la biblioteca, en el tercer estante, junto al libro verde, había una cajita con una galleta y otra nota: «Feliz cumpleaños, Mateo. Tu amigo secreto». Mateo sonrió: ya sabía quién era.

1. ¿Dónde encontró Mateo la nota?
   - A) **En su mochila**
   - B) En su pupitre
   - C) En la biblioteca
   - D) En el patio
2. ¿Cómo era la letra de la nota?
   - A) Redonda y rosa
   - B) Con pluma negra
   - C) Muy chiquita
   - D) **Cuadrada y de lápiz azul**
3. ¿En qué estante estaba la cajita?
   - A) En el segundo
   - B) **En el tercero**
   - C) En el cuarto
   - D) En el primero
4. ¿Qué había en la cajita?
   - A) Un dulce
   - B) **Una galleta**
   - C) Un lápiz
   - D) Un libro
5. ¿Quién escribió la nota?
   - A) Ana
   - B) Beto
   - C) **Diego**
   - D) Mateo

### Caso 3: El fantasma del salón

**Contraseña:** D-B-C-A-A

En la escuela decían que un fantasma vivía en el salón de música. Una noche, la maestra Rosa oyó sonar el piano. Entró corriendo y vio una sábana blanca moviéndose junto a la ventana, que estaba abierta. Sobre el piano había migajas de pan y huellas pequeñas de gato. Al levantar la sábana apareció Bigotes, el gato del conserje, que había saltado sobre las teclas y se había enredado.

1. ¿Dónde decían que vivía el fantasma?
   - A) En la biblioteca
   - B) En el patio
   - C) En la cocina
   - D) **En el salón de música**
2. ¿Qué fue lo que sonó?
   - A) Una campana
   - B) **El piano**
   - C) Una flauta
   - D) Un tambor
3. ¿Qué se movía junto a la ventana?
   - A) Una cortina roja
   - B) Un abrigo
   - C) **Una sábana blanca**
   - D) Una bufanda
4. ¿Qué había sobre el piano?
   - A) **Migajas de pan y huellas de gato**
   - B) Papeles
   - C) Flores
   - D) Un sombrero
5. ¿Quién era el «fantasma»?
   - A) **Bigotes, el gato del conserje**
   - B) La maestra Rosa
   - C) Un niño
   - D) Un perro

### Caso 4: El mapa del tesoro

**Contraseña:** B-D-D-A-C

Valeria y su hermano Pablo encontraron un mapa viejo en el ático. Decía que había que ir del columpio al árbol más grande, dar diez pasos hacia el norte y cruzar el puente de madera. Del otro lado había una roca con forma de tortuga. Debajo de la roca, cavando un poco, encontraron un cofre lleno de monedas de chocolate, y se las repartieron.

1. ¿Dónde encontraron el mapa?
   - A) En el sótano
   - B) **En el ático**
   - C) En la cocina
   - D) En el garaje
2. ¿Cómo se llaman los hermanos?
   - A) Lucía y Pablo
   - B) Valeria y Tomás
   - C) Sara y Pablo
   - D) **Valeria y Pablo**
3. ¿Qué tuvieron que cruzar?
   - A) Un río
   - B) Un túnel
   - C) Una cerca
   - D) **Un puente de madera**
4. ¿Qué forma tenía la roca?
   - A) **De tortuga**
   - B) De conejo
   - C) De estrella
   - D) De corazón
5. ¿Qué había en el cofre?
   - A) Joyas
   - B) Libros
   - C) **Monedas de chocolate**
   - D) Llaves

### Caso 5: El dragón de la feria

**Contraseña:** C-A-B-D-B

En la feria del pueblo había un dragón verde de cartón de tres metros, pero alguien le había quitado la cola. Don Ramiro, su dueño, estaba muy triste. Camila notó pintura amarilla en el piso, cerca del puesto de algodón de azúcar, y siguió las manchas hasta la carpa del payaso. Ahí estaba la cola: el payaso la estaba pintando de dorado porque quería darle una sorpresa al dragón para el desfile de la noche. Don Ramiro se puso feliz y todos aplaudieron.

1. ¿De qué color era el dragón?
   - A) Rojo
   - B) Azul
   - C) **Verde**
   - D) Negro
2. ¿Qué le faltaba al dragón?
   - A) **La cola**
   - B) Una pata
   - C) Un ojo
   - D) Un ala
3. ¿De qué color era la pintura que notó Camila?
   - A) Roja
   - B) **Amarilla**
   - C) Azul
   - D) Blanca
4. ¿Quién tenía la cola?
   - A) Don Ramiro
   - B) Camila
   - C) El vendedor de algodón
   - D) **El payaso**
5. ¿Por qué se la llevó?
   - A) Por una travesura
   - B) **Para pintarla de dorado y dar una sorpresa**
   - C) Para jugar
   - D) Porque estaba rota

### Caso 6: La carta sin firma

**Contraseña:** A-B-C-D-C

Una mañana, la maestra Elena encontró una carta sin firma sobre su escritorio. Decía: «Gracias por enseñarme a leer». Estaba escrita en papel azul claro y olía a fresas. En el sobre había una estampa de mariposa. La maestra recordó que solo Lucía ponía estampas de mariposa en sus cuadernos, y siempre con perfume de fresa. Al día siguiente, Lucía llegó con una gran sonrisa y la maestra le dijo: «De nada».

1. ¿Para quién era la carta?
   - A) **Para la maestra Elena**
   - B) Para el director
   - C) Para Lucía
   - D) Para el conserje
2. ¿Qué decía la carta?
   - A) Feliz cumpleaños
   - B) **Gracias por enseñarme a leer**
   - C) Perdón
   - D) Te invito a jugar
3. ¿De qué color era el papel?
   - A) Rosa
   - B) Verde
   - C) **Azul claro**
   - D) Amarillo
4. ¿A qué olía la carta?
   - A) A chocolate
   - B) A flores
   - C) A vainilla
   - D) **A fresas**
5. ¿Quién la escribió?
   - A) Ana
   - B) El director
   - C) **Lucía**
   - D) Diego
