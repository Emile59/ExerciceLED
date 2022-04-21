/*   -------------------------------- Exercice Alstom --------------------------------

     Mesure la différence entre un temps perçu et la reproduction de ce délai

     - HOMERIN Emile
     - 21/04/2022
     - Version 1 (sans matériel permettant de tester)

     ---------------------------------------------------------------------------------- */


#define LED 2          // Definition des pins correspondant à la LED et au bouton poussoir
#define BUTTON 4

int delay1, delay2;       // Durées aléatoires nécessaires à la réalisation de l'application
int appui = 0;            // Vaut 0 dans le cas du 1er appui (lancement de l'application), 1 lorsqu'on détecte ensuite un front descendant (l'utilisateur appui sur 
                          // le bouton), puis 2 lorsqu'on detecte un front montant (l'utilisateur relache le bouton)
int timer = 0, delta;     // timer sera notre chronomètre et delta représente la différence entre la durée de l'appui et le véritable temps d'allumage de la LED


void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON, INPUT);     // Déclaration du bouton poussoir sur la pin 2
  pinMode(LED, OUTPUT);       // Déclaration de la LED

  randomSeed(analogRead(0));    // Initialise le générateur de nombres aléatoires par le bruit aléatoire provenant de l'environnement
                                // Ainsi la suite de nombres aléatoires démarre à un endroit arbitraire
}


void loop()
{
  if ( digitalRead(BUTTON) == LOW &&  appui == 0)       // Si l'utilisateur appuie sur le bouton et que appui = 0 alors on lance l'application
  {
    delay1 = random(3000, 6001);   // Renvoie un nombre aléatoire entre 3000 et 6000
    delay2 = random(1000, 5001);   // Renvoie un nombre aléatoire entre 1000 et 5000

    delay(delay1);              // On attend entre 3 et 6 secondes aléatoirement

    digitalWrite(LED, HIGH);    // Puis la LED s'allume
    delay(delay2);              // pendant 1 à 5 secondes aléatoirement

    digitalWrite(LED, LOW);     // Enfin la LED s'éteind à la fin du délai

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
    delta = abs(delay2 - (millis() - timer));    // Calcul de delta (on peut enlever la valeur absolue afin de connaitre le signe de l'erreur)
                                                 // En effet millis() - timer correspond au temps de pression du bouton par l'utilisateur

    Serial.print(" Résultat : ");   // Affichage du résultat sur le port série
    Serial.print(delta);
    Serial.println(" ms \n");

    timer = 0;         // Réinitialisation du chronomètre
    appui = 0;         // Permet de relancer le processus lors d'un appui sur le bouton poussoir
  }
}
