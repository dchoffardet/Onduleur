#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int CapteurBatterie = 0;
int CaptBatterieHaut = 1023;
int CaptBatterieBas = 0;
int VoltBatterie = 0;
int CompteurOk = 0; // Compteur du nombre de fois batterie > Tension de Seuil
int PinCaptBatterie = A1;
int PinRelaisEDF = 8;
int PinRelaisPV = 13;
int TensionSwitch = 1250;
int TensionSeuil = 1200;
int i = 0;
long Tempo1 = 60000;
int Tempo2 = 5000;

void setup(){
lcd.begin(16, 2);
pinMode(PinCaptBatterie, INPUT);
pinMode(PinRelaisEDF, OUTPUT);
pinMode(PinRelaisPV, OUTPUT);

// initialisation des relais : EDF = 1 et PV = 0

digitalWrite(PinRelaisPV, LOW);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Relais PV = Off");
delay(Tempo2);
digitalWrite(PinRelaisEDF, HIGH);
lcd.setCursor(0, 1);
lcd.print("Relais EDF = On");
delay(Tempo2);
}

void loop(){

Initialisation:

CompteurOk = 0;

// Procédure de controle si possible basculer sur PV

for(i=1; i < 5; i++){
  CapteurBatterie = analogRead(PinCaptBatterie);
  VoltBatterie = (map(CapteurBatterie, 0, 1023, 0, 1500));
  if(VoltBatterie > TensionSwitch){
    CompteurOk = CompteurOk + 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Batterie = ");
      lcd.print(VoltBatterie);
      lcd.setCursor(0, 1);
      lcd.print("i = ");
      lcd.print(i);
      lcd.print("  Ok = ");
      lcd.print(CompteurOk);
    delay(Tempo1);
  }
  else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Batterie = ");
      lcd.print(VoltBatterie);
      lcd.setCursor(0, 1);
      lcd.print("i = ");
      lcd.print(i);
      lcd.print("  Ok = ");
      lcd.print(CompteurOk);
    delay(Tempo1);
  }
}

// controle si le nbre de Ok = 4

if (CompteurOk < 4) {
// retour au départ
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nbre Ok = ");
      lcd.print(CompteurOk);
      lcd.setCursor(0, 1);
      lcd.print("Retour");
delay(Tempo2);

goto Initialisation;

}

// Procédure de bascule sur PV

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Relais EDF = Off");
digitalWrite(PinRelaisEDF, LOW);
delay(Tempo2);
lcd.setCursor(0, 1);
lcd.print("Relais PV = On");
digitalWrite(PinRelaisPV, HIGH);
delay(Tempo1);

// Procédure de surveillance de la tension

Surveillance:

CapteurBatterie = analogRead(PinCaptBatterie);
VoltBatterie = (map(CapteurBatterie, 0, 1023, 0, 1500));
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Surveillance");
lcd.setCursor(0, 1);
lcd.print("Batterie = ");
lcd.print(VoltBatterie);
delay(Tempo2);

if (VoltBatterie > TensionSeuil){
delay(Tempo1);

goto Surveillance;

}

// Procédure de bascule sur EDF

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Relais PV = Off");
digitalWrite(PinRelaisPV, LOW);
delay(Tempo2);
lcd.setCursor(0, 1);
lcd.print("Relais EDF = On");
digitalWrite(PinRelaisEDF, HIGH);
delay(Tempo2);

// Retour au début : initialisation

goto Initialisation;

}
