#include <LiquidCrystal.h>

/*
 * Switch
 */

#define EDF_SWITCH_PIN = 8;
int edfSwitchIsEnabled = false;
#define BATTERY_SWITCH_PIN = 13;
int batterySwitchIsEnabled = false;

void enableBatterySwitch()
{
  digitalWrite(BATTERY_SWITCH_PIN, HIGH);
  batterySwitchIsEnabled = true;
}

void disableBatterySwitch()
{
  digitalWrite(BATTERY_SWITCH_PIN, LOW);
  batterySwitchIsEnabled = false;
}

char *getBatterySwitchMode()
{
  return batterySwitchIsEnabled ? "On" : "Off";
}

void enableEdfSwitch()
{
  digitalWrite(EDF_SWITCH_PIN, HIGH);
  EDF_SWITCH_ENABLED = true;
}

void disableEdfSwitch()
{
  digitalWrite(EDF_SWITCH_PIN, LOW);
  EDF_SWITCH_ENABLED = false;
}

char *getEdfSwitchMode()
{
  return EDF_SWITCH_ENABLED ? "On" : "Off";
}

/*
 * Battery
 */

#define POTENTIOMETER_PIN = A1;
#define HIGH_TENSION = 1250;
#define LOW_TENSION = 1200;

int getBatteryVoltage()
{
  int voltage = analogRead(POTENTIOMETER_PIN);
  return map(voltage, 0, 1023, 0, 1500);
}

bool isHighTension()
{
  return getBatteryVoltage() >= HIGH_TENSION;
}

bool isLowTension()
{
  return getBatteryVoltage() <= LOW_TENSION;
}

/*
 * Counter
 */
#define COUNTER_THRESHOLD = 4;
int counter = 0;

void incrementCounter()
{
  counter++;
}

void resetCounter()
{
  counter = 0;
}

bool hasCounterReachedThreshold()
{
  return counter > COUNTER_THRESHOLD;
}

bool hasCounterBegun()
{
  return counter > 0;
}

/*
 * Display
 */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void display(char *line1, char *line2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void display(char *line, )
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line);
}

/*
 * State
 */
#define STATE_INIT = 0;
#define STATE_TO_BATTERY = 1;
#define STATE_BATTERY_MODE = 2;
#define STATE_TO_EDF = 3;
#define STATE_EDF_MODE = 4;

int state = STATE_INIT;

/*
 * Main
 */

void setup()
{
  lcd.begin(16, 2);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(EDF_SWITCH_PIN, OUTPUT);
  pinMode(BATTERY_SWITCH_PIN, OUTPUT);

  disableBatterySwitch();
  disableEdfSwitch();
}

void loop()
{
  switch (state)
  {
    case STATE_INIT:
    
      display("Initialisation");

      if (isHighTension())
      {
        state = STATE_TO_BATTERY;
      }
      else
      {
        state = STATE_TO_EDF;
      }

      break;

    case STATE_TO_BATTERY:

      display(
          "Branchement Bat",
          "EDF:" + getEdfSwitchMode() + " Bat:" + getBatterySwitchMode());

      if (edfSwitchIsEnabled)
      {
        disableEdfSwitch();

        display(
            "Branchement Bat",
            "EDF:" + getEdfSwitchMode() + " Bat:" + getBatterySwitchMode());

        delay(5000);
      }

      if (!batterySwitchIsEnabled)
      {
        enableBatterySwitch();

        display(
            "Branchement Bat",
            "EDF:" + getEdfSwitchMode() + " Bat:" + getBatterySwitchMode());
      }

      state = STATE_BATTERY_MODE;

      break;

    case STATE_TO_EDF:
      display(
          "Branchement EDF",
          "Bat:" + getBatterySwitchMode() + " EDF:" + getEdfSwitchMode());

      if (batterySwitchIsEnabled)
      {
        disableBatterySwitch();

        display(
            "Branchement EDF",
            "Bat:" + getBatterySwitchMode() + " EDF:" + getEdfSwitchMode());

        delay(5000);
      }

      if (!edfSwitchIsEnabled)
      {
        enableEdfSwitch();

        display(
            "Branchement EDF",
            "Bat:" + getBatterySwitchMode() + " EDF:" + getEdfSwitchMode());
      }

      state = STATE_EDF_MODE;

      break;

    case STATE_BATTERY_MODE:

      display(
          "Mode Batterie",
          "Tension: " + getBatteryVoltage() + "V");

      if (isLowTension())
      {
        state = STATE_TO_EDF;
      }

      break;

    case STATE_EDF_MODE:

      display(
          "Mode EDF T = " + getBatteryVoltage() + "V",
          "Compteur: " + counter +);

      if (isHighTension())
      {
        incrementCounter();

        if (hasCounterReachedThreshold())
        {
          state = STATE_TO_BATTERY;
          resetCounter();
          break
        }
      }
      else
      {
        if (hasCounterBegun())
        {
          resetCounter();
        }
      }

      break;
  }

  delay(5000);
}
