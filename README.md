# Caja Detective

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

| Caso | Dificultad | Contraseña |
|---|---|---|
| La galleta que desapareció | Sencillo | B-A-D-B-C |
| El paraguas amarillo | Medio | B-C-D-B-A |
| El reloj de la plaza | Difícil | B-C-D-A-C |
| Prueba | (quítala al terminar) | B-B-B-B-B |

También abre la clave de fábrica **B-A-D-C-A**, hasta que el maestro guarde una propia.
Los casos fijos están en `include/Config.h` (`CASES`). Con `USE_FIXED_CASES = false` solo abre la clave del maestro.

## Modo profesor (cambiar la contraseña)
El profesor puede guardar su propia contraseña directamente en la caja, sin computadora. En el código este modo se llama "modo maestro".

**PIN del profesor: D-C-B-A** (se cambia en `MASTER_PIN`, `include/Config.h`).

### Cómo entrar y cambiar la contraseña
1. Mantén presionados **ENVIAR + BORRAR** a la vez durante **2 segundos**. Se encienden el LED rojo y el verde, y suenan 2 pitidos. Eso indica que estás en modo profesor.
2. Teclea el PIN **D-C-B-A** (4 botones) y presiona **ENVIAR**. Un pitido medio confirma que el PIN es correcto.
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

### Caso 1 (sencillo): La galleta que desapareció

**Contraseña:** B-A-D-B-C

Tomás llegó de la escuela con mucha hambre. Su mamá había horneado galletas de chocolate y le dejó una sobre la mesa de la cocina, con una nota que decía: «Cómetela después de hacer la tarea». Tomás subió a su cuarto y terminó la tarea de matemáticas en media hora. Cuando bajó, la galleta ya no estaba. En el piso había migas de chocolate que llegaban hasta la sala y se detenían junto al sillón. Tomás se agachó y debajo del sillón encontró a Pelusa, su gato gris, dormido con migas en el bigote. Tomás se rio y la acarició. Como ya había terminado la tarea, su mamá le sirvió un vaso de leche con otra galleta.

1. ¿Dónde dejó la mamá la galleta?
   - A) En el sillón
   - B) **En la mesa de la cocina**
   - C) En su mochila
   - D) En el jardín
2. ¿De qué eran las migas que había en el piso?
   - A) **De chocolate**
   - B) De pan
   - C) De queso
   - D) De fresa
3. ¿Qué hizo Tomás antes de bajar a comer la galleta?
   - A) Jugó con Pelusa
   - B) Vio televisión
   - C) Se durmió
   - D) **Terminó la tarea de matemáticas**
4. ¿Cómo era Pelusa?
   - A) Un perro blanco
   - B) **Un gato gris**
   - C) Un conejo café
   - D) Un gato negro
5. ¿Qué le sirvió su mamá al final?
   - A) Solo una galleta
   - B) Un vaso de agua
   - C) **Un vaso de leche con otra galleta**
   - D) Pan con mantequilla

### Caso 2 (medio): El paraguas amarillo

**Contraseña:** B-C-D-B-A

El lunes llovió toda la mañana. Camila llegó a la escuela con su paraguas amarillo, que tenía una manija de madera y una pequeña etiqueta con su nombre, y lo dejó en el perchero del pasillo. A la hora de la salida, el paraguas ya no estaba. Camila lo buscó en el pasillo, en el salón y en la dirección, pero nadie lo había visto. Su maestra, la señora Paty, le sugirió revisar el patio. Ahí vio a Iván caminando con un paraguas amarillo igual al suyo. Camila miró la manija: tenía una etiqueta con su nombre. Iván se puso rojo y explicó que lo había tomado por error, porque su paraguas también era amarillo y lo había dejado en casa. Se disculpó y se lo devolvió. Como seguía lloviendo mucho, compartieron el paraguas de Camila hasta la salida, y Camila lo invitó a jugar el miércoles.

1. ¿Dónde dejó Camila su paraguas al llegar a la escuela?
   - A) En su salón
   - B) **En el perchero del pasillo**
   - C) En la dirección
   - D) En el patio
2. ¿De qué color era el paraguas de Camila?
   - A) Azul
   - B) Rojo
   - C) **Amarillo**
   - D) Verde
3. ¿Cómo supo Camila que el paraguas era suyo?
   - A) Por una mancha de pintura
   - B) Por su tamaño
   - C) Porque Iván se lo dijo
   - D) **Por la etiqueta con su nombre en la manija**
4. ¿Por qué tenía Iván el paraguas?
   - A) Para molestarla
   - B) **Lo tomó por error**
   - C) Lo encontró en la calle
   - D) Se lo prestó la maestra
5. ¿Qué hicieron al final?
   - A) **Compartieron el paraguas de Camila hasta la salida**
   - B) Fueron a la dirección
   - C) Se pelearon
   - D) Esperaron a que parara de llover

### Caso 3 (difícil): El reloj de la plaza

**Contraseña:** B-C-D-A-C

Cada mañana, el reloj de la plaza marcaba las ocho, aunque ya fuera mediodía. Los vecinos pensaban que estaba descompuesto y, como llevaba años así, nadie se molestaba en arreglarlo; algunos incluso habían dejado de mirarlo. Una tarde, Inés, una joven relojera recién llegada al pueblo, subió a la torre a revisarlo. Descubrió que las manecillas no estaban rotas: alguien las detenía a propósito con un alambre fino. Además, en los escalones había huellas de aceite, como si alguien hubiera subido muchas veces con una aceitera en la mano. Siguiendo esas huellas, Inés llegó a la casa de Don Fausto, un anciano callado que vivía solo junto a la plaza. Don Fausto confesó que lo hacía porque a las ocho de la mañana, hacía cincuenta años, había conocido a su esposa en esa plaza, y le dolía que el tiempo siguiera avanzando después de ese momento. Inés no lo regañó ni avisó a nadie. Le propuso instalar una campana que sonara a las ocho cada mañana en honor a ese recuerdo, y a cambio el reloj volvería a marcar la hora real. Don Fausto aceptó con los ojos llenos de lágrimas. Desde entonces, cada día a las ocho suena la campana y el anciano sonríe desde su ventana.

1. ¿Por qué los vecinos no arreglaban el reloj?
   - A) Porque no tenían dinero
   - B) **Porque creían que estaba descompuesto desde hacía años y no conocían la verdadera causa**
   - C) Porque a todos les gustaba así
   - D) Porque nadie sabía subir a la torre
2. ¿Qué descubrió Inés sobre las manecillas?
   - A) Estaban rotas
   - B) Les faltaban piezas
   - C) **Alguien las detenía a propósito con un alambre**
   - D) Estaban oxidadas
3. ¿Cómo llegó Inés a la casa de Don Fausto?
   - A) Preguntó a los vecinos
   - B) Lo vio en la plaza
   - C) Por una carta
   - D) **Siguiendo las huellas de aceite de la escalera**
4. ¿Por qué detenía Don Fausto el reloj?
   - A) **Quería conservar el recuerdo de cuando conoció a su esposa y le dolía que pasara el tiempo**
   - B) Odiaba el ruido
   - C) Quería molestar a los vecinos
   - D) Estaba cansado de la plaza
5. ¿Qué describe mejor lo que hizo Inés?
   - A) Lo castigó
   - B) Lo ignoró
   - C) **Encontró una solución que respetaba su recuerdo**
   - D) Se lo contó a todo el pueblo
