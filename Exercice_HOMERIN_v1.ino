/*   -------------------------------- Exercice Alstom --------------------------------

     Mesure la différence entre un temps perçu et la reproduction de ce délai

     - HOMERIN Emile
     - 22/04/2022
     - Version 2 : Application fonctionnelle et affichage du résultat sur un écran LCD

     ---------------------------------------------------------------------------------- */

#include <LiquidCrystal.h>   // Bibilothèque nécessaire pour utiliser l'écran LCD

#define LED 32          // Definition des pins correspondant à la LED et au bouton poussoir
#define BUTTON 14

LiquidCrystal lcd(19, 23, 18, 17, 16, 15);   // Définition de l'écran

int delay1, delay2;       // Durées aléatoires nécessaires à la réalisation de l'application
int appui = 0;            // Vaut 0 dans le cas du 1er appui (lancement de l'application), 1 lorsqu'on détecte ensuite un front descendant (l'utilisateur appui sur 
                          // le bouton), puis 2 lorsqu'on detecte un front montant (l'utilisateur relache le bouton)
int timer, user_delay, delta;     // timer sera notre chronomètre, user_delay le temps d'appui de l'utilisateur et delta représente la différence entre la durée de l'appui et le véritable temps d'allumage de la LED


void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);     // Déclaration du bouton poussoir
  pinMode(LED, OUTPUT);              // Déclaration de la LED

  randomSeed(analogRead(0));    // Initialise le générateur de nombres aléatoires par le bruit aléatoire provenant de l'environnement
                                // Ainsi la suite de nombres aléatoires démarre à un endroit arbitraire
  
  lcd.begin(16, 2);             // L’écran dispose de 16 colonnes et 2 lignes
}


void loop()
{
     
  if ( digitalRead(BUTTON) == LOW &&  appui == 0)       // Si l'utilisateur appuie sur le bouton et que appui = 0 alors on lance l'application
  {
    Serial.println("Nouvel essai");

    lcd.clear();    // Efface le résultat précédent sur l'écran
       
    delay1 = random(3000, 6001);   // Renvoie un nombre aléatoire entre 3000 et 6000
    delay2 = random(1000, 5001);   // Renvoie un nombre aléatoire entre 1000 et 5000

    delay(delay1);              // On attend entre 3 et 6 secondes aléatoirement

    digitalWrite(LED, HIGH);    // Puis la LED s'allume
    delay(delay2);              // pendant 1 à 5 secondes aléatoirement

    digitalWrite(LED, LOW);     // Enfin la LED s'éteind à la fin du délai
               
    Serial.print("Temps d'allumage de la LED : ");    // Permet de vérifier le bon fonctionnement de l'application
    Serial.print(delay2);
    Serial.println(" ms");

    appui = 1;     // On attend à présent une action de l'utilisateur
  }

     
  if ( digitalRead(BUTTON) == LOW  &&  appui == 1)       // Si l'utilisateur appuie sur le bouton et que appui = 1 alors on démarre le chronomètre
  {
    digitalWrite(LED, HIGH);        // Lorsque l'utilisateur appui sur le bouton pour reproduire le délai la LED S'allume
    timer = millis();               // On initialise le chronomètre au nombre de millisecondes qui se sont écoulées depuis le début du programme 
    appui = 2;                      // Pour ne pas ré-initialiser le timer 
  }

     
  if (digitalRead(BUTTON) == HIGH  &&  appui == 2)     // Si l'utilisateur relache le bouton et que appui = 2 alors on calcule delta
  {
    digitalWrite(LED, LOW);         // On éteind la LED
       
    user_delay = millis() - timer;     // Calcul du temps d'appui de l'utilisateur
       
    Serial.print("Temps d'appui par l'utilisateur : ");    // Permet de vérifier le bon fonctionnement de l'application
    Serial.print(user_delay);
    Serial.println(" ms\n");
       
    delta = abs(delay2 - user_delay);    // Calcul de delta (on peut enlever la valeur absolue afin de connaitre le signe de l'erreur)
                                         // En effet millis() - timer correspond au temps de pression du bouton par l'utilisateur

    lcd.setCursor(2,0);             // Affichage du résultat sur l'écran
    lcd.print("Resultat : ");   
    lcd.setCursor(3, 1);
    lcd.print(delta);
    lcd.print(" ms");

    timer = 0;         // Réinitialisation du chronomètre
    appui = 0;         // Permet de relancer le processus lors d'un appui sur le bouton poussoir
  }
     
}
