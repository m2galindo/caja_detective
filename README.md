# Caja Detective
# Tema: Letroescritura

## Equipo 5 
| Nombre | Matrícula |
|--------|-----------|
|José Adrian Rodarte Soto | A00844251|
|Jesús Flores Martinez | A01286933|
|Isabella Alvarez Cadena | A01572337|
|Gerson Josue Ayala Membreño | A00845635|
|Marco Alejandro Galindo de la Cruz | A00845553|
|Jose Alfonso Cantu | A01285484|

| Profesores  |
|---------------------|
|Lic. Eduardo Salazar|
|Rubén Vilchis M.|
 
## Instrucciones de uso

### Cómo se juega (alumnos)
1. Lee la historia de la tarjeta y contesta las 5 preguntas.
2. Presiona en la caja el botón de cada respuesta (**A, B, C o D**), en orden. Cada vez que presionas, suena un pitido corto y se enciende un LED amarillo de progreso (de 1 a 5).
3. Si te equivocas de botón, presiona **BORRAR** para empezar de nuevo.
4. Cuando hayas puesto las 5 respuestas, presiona **ENVIAR** y suéltalo.
5. **Si todas son correctas**, la caja se abre: suenan 3 pitidos, se enciende el LED verde y el solenoide libera la tapa unos 5 segundos. Abre la tapa en ese momento.
6. Para dejar la caja lista para el siguiente caso, presiona **ENVIAR** otra vez. Se apaga el LED verde y el solenoide vuelve a cerrar.

### Si te equivocas
- Suena un pitido largo y se enciende el LED **rojo** unos 3 segundos.
- Los LEDs amarillos muestran **cuántas respuestas acertaste**, no cuáles. Vuelve a leer la historia y corrige.
- Con **3 errores seguidos**, la caja se bloquea **30 segundos**: el LED rojo parpadea y los botones no responden. Pasado ese tiempo puedes volver a intentar.
- Si presionas ENVIAR con menos de 5 respuestas, solo suena un pitido de error y no cuenta como intento fallido.

### Qué significan los LEDs y los sonidos
| Señal | Significado |
|---|---|
| LEDs amarillos (1 a 5) | Respuestas que llevas puestas. Tras un error, cuántas acertaste |
| LED rojo fijo | Error. Se apaga solo |
| LED rojo parpadeando | Caja bloqueada 30 segundos |
| LED verde | Caja abierta. Se apaga al cerrarla con ENVIAR |
| Rojo y verde juntos | Modo profesor activo |
| Pitido corto | Botón de respuesta presionado |
| Pitido largo | Error |
| 3 pitidos (el último largo) | Acierto, la caja se abre |

### Modo profesor
El profesor puede cambiar la contraseña desde la caja. Los pasos están más abajo, en la sección "Modo profesor".

### Si algo no funciona
- **La caja no responde:** revisa que esté conectada a la corriente (USB o fuente) y que el LED de la placa esté encendido.
- **Un botón no registra:** al presionarlo debe encenderse un LED amarillo y sonar un pitido. Si no, el botón o su cable están flojos.
- **No abre aunque la clave es correcta:** suelta ENVIAR rápido (se revisa al soltarlo) y confirma que pusiste exactamente 5 respuestas.
- **El solenoide no se mueve pero se oye el clic del relé:** revisa la fuente del solenoide y que sus cables estén en las terminales COM y NO del relé.

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

## Contraseñas de la caja
Se presiona la secuencia de 5 botones y luego ENVIAR.

| Caso | Contraseña |
|---|---|
| El perro desaparecido | C-C-A-B-D |
| La nota misteriosa | A-D-B-B-C |
| El fantasma del salón | D-B-C-A-A |
| El mapa del tesoro | B-D-D-A-C |
| El dragón de la feria | C-A-B-D-B |
| La carta sin firma | A-B-C-D-C |
| Prueba (quítala al terminar) | B-B-B-B-B |

También abre la clave de fábrica **B-A-D-C-A**, hasta que el maestro guarde una propia.
Los casos fijos están en `include/Config.h` (`CASES`). Con `USE_FIXED_CASES = false` solo abre la clave del maestro.

## Modo profesor (cambiar la contraseña)
El profesor puede guardar su propia contraseña directamente en la caja, sin computadora. En el código este modo se llama "modo maestro".

**PIN del profesor: B-B-B-B** (se cambia en `MASTER_PIN`, `include/Config.h`).

### Cómo entrar y cambiar la contraseña
1. Mantén presionados **ENVIAR + BORRAR** a la vez durante **2 segundos**. Se encienden el LED rojo y el verde, y suenan 2 pitidos. Eso indica que estás en modo profesor.
2. Teclea el PIN **B-B-B-B** (4 botones) y presiona **ENVIAR**. Un pitido medio confirma que el PIN es correcto.
3. Teclea la **contraseña nueva** (5 respuestas) y presiona **ENVIAR**.
4. Teclea **la misma contraseña otra vez** y presiona **ENVIAR**.
5. Si coinciden, se enciende el LED verde por 1 segundo y suena el acierto. La contraseña queda guardada aunque se desconecte la placa.

### Cosas útiles
- **BORRAR** borra lo que llevas tecleado en el paso actual, sin salir del modo profesor.
- Los LEDs de progreso muestran cuántos botones llevas presionados.
- Si el PIN es incorrecto, las dos contraseñas no coinciden, o pasan **30 segundos** sin presionar nada, sale del modo profesor con el LED rojo y **no cambia nada**.
- La caja guarda **una sola** contraseña del profesor. Si la cambia otra vez, reemplaza la anterior.
- Las contraseñas fijas (los 3 casos, B-B-B-B-B y la de fábrica B-A-D-C-A) **siguen abriendo** la caja. Para que abra solo con la del profesor, pon `USE_FIXED_CASES = false` en `include/Config.h` y vuelve a subir el programa.
- Se puede entrar al modo profesor con la caja cerrada o abierta, pero no durante el bloqueo de 30 segundos ni mientras muestra los aciertos tras un error.

## Historias y preguntas
Cada caso tiene 5 preguntas de opción múltiple (A, B, C, D). La secuencia de respuestas correctas es la contraseña. Las respuestas correctas están en **negritas**.

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
