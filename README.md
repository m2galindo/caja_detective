# Caja Detective
 
## Instrucciones de uso

### Cómo se juega (alumnos)
1. Lee la historia de la tarjeta y contesta las 5 preguntas.
2. Presiona en la caja el botón de cada respuesta (**A, B, C o D**), en orden. Cada vez que presionas, se enciende un LED amarillo de progreso (de 1 a 5).
3. Si te equivocas de botón, presiona **BORRAR** para empezar de nuevo.
4. Cuando hayas puesto las 5 respuestas, presiona **ENVIAR** y suéltalo.
5. **Si todas son correctas**, la caja se abre: se enciende el LED verde y el solenoide libera la tapa unos 5 segundos. Abre la tapa en ese momento.
6. Para dejar la caja lista para el siguiente caso, presiona **ENVIAR** otra vez. Se apaga el LED verde y el solenoide vuelve a cerrar.

### Si te equivocas
- Se enciende el LED **rojo** unos 3 segundos.
- Los LEDs amarillos muestran **cuántas respuestas acertaste**, no cuáles. Vuelve a leer la historia y corrige.
- Con **3 errores seguidos**, la caja se bloquea **30 segundos**: el LED rojo parpadea y los botones no responden. Pasado ese tiempo puedes volver a intentar.
- Si presionas ENVIAR con menos de 5 respuestas, no pasa nada y no cuenta como intento fallido.

### Qué significan los LEDs
| Señal | Significado |
|---|---|
| LEDs amarillos (1 a 5) | Respuestas que llevas puestas. Tras un error, cuántas acertaste |
| LED rojo fijo | Error. Se apaga solo |
| LED rojo parpadeando | Caja bloqueada 30 segundos |
| LED verde | Caja abierta. Se apaga al cerrarla con ENVIAR |
| Rojo y verde juntos | Modo profesor activo |

### Modo profesor
El profesor puede cambiar la contraseña desde la caja. Los pasos están más abajo, en la sección "Modo profesor".

### Si algo no funciona
- **La caja no responde:** revisa que esté conectada a la corriente (USB o fuente) y que el LED de la placa esté encendido.
- **Un botón no registra:** al presionarlo debe encenderse un LED amarillo. Si no, el botón o su cable están flojos.
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
1. Mantén presionados **ENVIAR + BORRAR** a la vez durante **2 segundos**. Se encienden el LED rojo y el verde juntos. Eso indica que estás en modo profesor.
2. Teclea el PIN **B-B-B-B** (4 botones) y presiona **ENVIAR**.
3. Teclea la **contraseña nueva** (5 respuestas) y presiona **ENVIAR**.
4. Teclea **la misma contraseña otra vez** y presiona **ENVIAR**.
5. Si coinciden, se enciende el LED verde por 1 segundo. La contraseña queda guardada aunque se desconecte la placa.

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

Ayer, justo cuando el cielo empezaba a ponerse gris, Sofía salió al jardín a llamar a su perra para darle de cenar.

—¡Luna! ¡Luna, ven, bonita! —gritó, pero nadie contestó.

Corrió hacia el fondo del jardín y se le encogió el corazón: la reja estaba abierta de par en par. En el piso, sobre la tierra húmeda, había huellas de patas pequeñas y, a un lado, un pedazo de salchicha. «Alguien le dejó comida para que saliera», pensó Sofía. Las huellas seguían por la banqueta y llevaban directo al parque.

Cerca de la banca, el vendedor de globos acomodaba su manojo de colores.

—Disculpe, ¿no vio a una perrita? —preguntó Sofía con la voz temblorosa.

—Sí, hace un rato —dijo el señor, rascándose la barbilla—. Era una perrita café con un listón rojo, y se iba comiendo una salchicha. Corrió hacia la panadería porque empezó a tronar.

Sofía salió disparada. En la panadería olía a pan recién hecho, y la dueña le señaló una mesa del rincón.

—Se metió ahí cuando cayó la tormenta —le contó—. Estaba muy asustada.

Sofía se agachó y, debajo de la mesa, vio dos ojitos brillantes: Luna estaba mojada y temblando. Sofía la abrazó fuerte y por fin pudo sonreír.

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

Esa mañana, Mateo abrió su mochila para sacar el cuaderno y encontró algo que no era suyo: una nota doblada en cuatro partes. La desdobló con cuidado y leyó en voz baja:

«Búscame donde guardan los libros, en el tercer estante, junto al libro verde».

Mateo miró a su alrededor, pero nadie lo estaba viendo. Se fijó en la letra: era cuadrada, firme, y estaba escrita con lápiz azul. «Esta letra la conozco de algún lado», pensó. Repasó mentalmente a sus compañeros: Ana siempre escribía con lápiz rosa, Beto no soltaba su pluma negra y Diego… Diego usaba lápiz azul, y su letra también era cuadrada.

A la hora del recreo, Mateo se escabulló a la biblioteca. Contó los estantes con el dedo: uno, dos, tres. Ahí, justo junto al libro verde, había una cajita atada con un listón. La abrió despacio. Adentro había una galleta con forma de estrella y otra nota:

«Feliz cumpleaños, Mateo. Tu amigo secreto».

Mateo sonrió de oreja a oreja. Ya no tenía ninguna duda de quién había sido.

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

En la escuela todos decían que un fantasma vivía en el salón de música. Los niños contaban que en las noches se oían ruidos raros y sombras extrañas. Nadie se atrevía a entrar después de las clases.

Una noche, la maestra Rosa se quedó calificando exámenes. Ya era tarde y el edificio estaba en silencio cuando, de pronto, oyó un sonido que le erizó la piel: alguien estaba tocando el piano. No era una melodía, sino notas sueltas, tropezadas, como si alguien caminara sobre las teclas.

La maestra Rosa tomó su linterna, respiró hondo y se acercó al salón de música. Empujó la puerta con cuidado.

Junto a la ventana, que estaba abierta, una sábana blanca se movía de un lado a otro. «Será el viento», trató de convencerse, pero el viento no hace sonar un piano. Se acercó. Sobre el piano había migajas de pan y, entre ellas, unas huellas pequeñas, como de almohadillas.

—Huellas de gato —murmuró.

Con un solo tirón levantó la sábana. Debajo, enredado y muy asustado, apareció Bigotes, el gato del conserje, que había entrado por la ventana siguiendo el olor del pan y había saltado sobre las teclas. La maestra Rosa soltó una carcajada: el gran fantasma de la escuela tenía bigotes y cola.

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

Un sábado lluvioso, Valeria y su hermano Pablo subieron al ático de la casa de su abuela para buscar algo con qué entretenerse. Entre cajas de fotos y juguetes viejos, Pablo sacó un papel amarillento, doblado y lleno de polvo.

—¡Mira! ¡Es un mapa! —gritó.

Lo extendieron sobre el piso. Estaba dibujado con tinta café y tenía una equis roja en una esquina. Valeria leyó las instrucciones en voz alta:

—«Del columpio, ve al árbol más grande. Da diez pasos hacia el norte y cruza el puente de madera. Del otro lado, busca la roca».

Cuando dejó de llover, los hermanos corrieron al jardín. Empezaron en el columpio, caminaron hasta el árbol más grande y Pablo contó los pasos, moviendo los labios: «…ocho, nueve, diez». Luego cruzaron con cuidado el viejo puente de madera que chirriaba bajo sus pies.

Del otro lado había una roca enorme, redonda y con una forma muy curiosa.

—¡Parece una tortuga! —rió Pablo.

Cavaron con las manos junto a la roca hasta que tocaron algo duro: era un cofre pequeño. Lo abrieron con el corazón latiéndoles fuerte y se quedaron callados un segundo. Estaba lleno de monedas de chocolate envueltas en papel dorado. Su abuela los miraba desde la ventana, riéndose. Los hermanos se las repartieron en partes iguales.

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

Todos los años, la feria del pueblo tenía una gran atracción: un dragón verde de cartón de tres metros de alto, con ojos grandes y una larga cola que se movía con el viento. Los niños hacían fila para tomarse fotos con él.

Pero esa mañana, cuando la feria abrió, el dragón se veía extraño, como si le faltara algo. Don Ramiro, su dueño, dio una vuelta alrededor de él con las manos en la cabeza.

—¡Mi dragón! ¡Le quitaron la cola! —exclamó, con los ojos llenos de lágrimas—. ¿Quién pudo hacer algo así?

Camila, que estaba comiendo un manojo de papas fritas, dejó de masticar y se puso a pensar como detective. Miró el piso con mucha atención y, cerca del puesto de algodón de azúcar, descubrió una mancha de pintura amarilla. Un poco más allá, otra. Y otra más.

—¡Son pistas! —susurró—. Una huella detrás de otra.

Siguió las manchas entre los puestos, esquivando globos y charcos, hasta que llegó a la carpa del payaso. Se asomó por la tela y vio algo increíble: ahí estaba la cola del dragón, con el payaso dándole los últimos toques con una brocha. La estaba pintando de dorado.

—No quería que nadie lo supiera —confesó el payaso, un poco apenado—. Era una sorpresa para el dragón, para el desfile de esta noche.

Camila fue corriendo a contárselo a Don Ramiro, y el viejo se puso tan feliz que abrazó al payaso, a Camila y hasta a un señor que pasaba. Esa noche, todos aplaudieron al dragón de la cola dorada.

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

Una mañana, la maestra Elena llegó temprano al salón, como cada día, y se encontró con una sorpresa sobre su escritorio: una carta sin nombre. Dentro del sobre, que tenía pegada una pequeña estampa de mariposa, había una hoja de papel azul claro. Al abrirla, un aroma dulce le llegó a la nariz: olía a fresas. La carta decía, con letra cuidadosa:

«Gracias por enseñarme a leer».

No tenía firma. La maestra Elena sonrió, pero la curiosidad le empezó a dar vueltas en la cabeza. «¿Quién habrá sido?», pensó.

Empezó a recordar. ¿Ana? No, Ana siempre firmaba sus dibujos. ¿Diego? Diego jamás usaba papel de colores. Entonces se acordó de las libretas de sus alumnos y, de pronto, lo vio claro: solo una niña pegaba estampas de mariposa en sus cuadernos y, además, siempre las perfumaba con un poquito de fresa. Esa niña era Lucía.

Al día siguiente, Lucía entró al salón con una sonrisa enorme, buscando la mirada de su maestra. La maestra Elena se agachó y le dijo bajito, para que solo ella lo oyera:

—De nada.

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
