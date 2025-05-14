//#include ""//acrescentar biblioteca WIFi

//#define F  (x > 0.1) receber variavel como funcao

#define pin1 2 //modificar os numeros consoante os pins
#define pin2 3
#define pin3 26
#define pin4 25

//BluetoothSerial SerialBT; será que o WIFI tmb tem de ter uma definição destas?
char comando;

void setup() { // inicia o serial
 Serial.begin(9600); //atencao a este numereo, pode ser preciso igualar ao acelerometro e ao wifi
  
 //btSerial.begin(9600);
 //SerialBT.begin("Carrinho"); //inicia o serial bluetooth com nome carrinho

 pinMode(pin1, OUTPUT); //definição dos pins como saida
 pinMode(pin2, OUTPUT);
 pinMode(pin3, OUTPUT);
 pinMode(pin4, OUTPUT);
 Serial.println("Fim Setup");
 delay(2000); //é preciso definir um atraso para que a comunicação seja estabelecida de maneira correta, sem perda de dados.
}

void loop() { //função loop() identifica a ocorrência de uma ação, para isso,sempre que o dispositivo estiver disponível, iremos armazenar os dados enviados por ele na variável comando, para na sequência decidir quais tratamentos deverão ser realizados.

 if (SerialBT.available()) { //funciona quando o dispositivo está está disponivel 
 comando = SerialBT.read(); //ler o comando
 }
 
 switch (comando) { //correspondencia entre a letra e o estado dos pinos (mudar as letras)
 case 'F': { //frente
   digitalWrite(pin1, HIGH);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, HIGH);
   digitalWrite(pin4, LOW);
   break;
 }
 case 'FD': {//frente direita
   digitalWrite(pin1, HIGH);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, HIGH);
   break;
 }
 case 'FE': {//frente esquerda
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, HIGH);
   digitalWrite(pin3, HIGH);
   digitalWrite(pin4, LOW);
   break;
 }
 case 'D': {//direita
   digitalWrite(pin1, HIGH);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, LOW);
 break;
 }
 case 'E': {//esquerda
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, HIGH);
   digitalWrite(pin4, LOW);
   break;
 }
 case 'T': {//tras
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, HIGH);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, HIGH);
   break;
 }
 case 'TE': {//tras esquerda
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, HIGH);
   break;
 }
 case 'TD': {//tras direita
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, HIGH);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, LOW);
   break;
 }
 default: { //parado
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, LOW);
   digitalWrite(pin3, LOW);
   digitalWrite(pin4, LOW);
   break;
 }
 }
}